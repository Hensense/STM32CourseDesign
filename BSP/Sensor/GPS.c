#include "gps.h"
#if GPS_START_STATE 
// 全局变量定义
char GPS_RX_BUF[GPS_USART_REC_LEN]; 
uint16_t GPS_RX_PTR = 0;
GPS_SaveData GPS_Data = {0};
void GPS_CLR_Buf(void);
#if EN_GPS_RX

void GPS_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能USART和GPIO时钟
    #if GPS_USARTx == 1
        RCC_APB2PeriphClockCmd(GPS_RCC_APBPeriph | GPS_GPIO_RCC, ENABLE);
    #else
        RCC_APB1PeriphClockCmd(GPS_RCC_APBPeriph, ENABLE);
        RCC_APB2PeriphClockCmd(GPS_GPIO_RCC, ENABLE);
    #endif

    // 配置TX引脚为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPS_GPIO_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPS_GPIO_TX_PORT, &GPIO_InitStructure);

    // 配置RX引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPS_GPIO_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPS_GPIO_RX_PORT, &GPIO_InitStructure);

    // USART初始化设置
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(GPS_USART_UX, &USART_InitStructure);
    
    // NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = GPS_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 使能串口接收中断
    USART_ITConfig(GPS_USART_UX, USART_IT_RXNE, ENABLE);
    USART_Cmd(GPS_USART_UX, ENABLE);
    
    GPS_CLR_Buf(); // 清空缓存
}

// 串口中断服务函数
void GPS_USART_IRQHandler(void)
{
    if(USART_GetITStatus(GPS_USART_UX, USART_IT_RXNE) != RESET)
    {
        char Res = USART_ReceiveData(GPS_USART_UX);
        
        // 检测帧起始符
        if(Res == '$') {
            GPS_RX_PTR = 0;
        }
        
        // 存储接收到的数据
        if(GPS_RX_PTR < GPS_USART_REC_LEN) {
            GPS_RX_BUF[GPS_RX_PTR++] = Res;
        }
        
        // 检测完整帧
        if(GPS_RX_BUF[0] == '$' && GPS_RX_BUF[4] == 'M' && GPS_RX_BUF[5] == 'C')
        {
            if(Res == '\n') 
            {
                // 保存有效数据
                memcpy(GPS_Data.GPS_Buffer, GPS_RX_BUF, GPS_RX_PTR);
                GPS_Data.isGetData = true;
                
                // 重置接收状态
                GPS_CLR_Buf();
            }
        }
    }
    USART_ClearITPendingBit(GPS_USART_UX, USART_IT_RXNE);
}

// 命令识别函数
uint8_t GPS_Hand(char *a)
{
    return (strstr(GPS_RX_BUF, a) != NULL) ? 1 : 0;
}

// 清空接收缓冲区
void GPS_CLR_Buf(void)
{
    memset(GPS_RX_BUF, 0, GPS_USART_REC_LEN);
    GPS_RX_PTR = 0;
}

// 重置数据结构
void GPS_clrStruct(void)
{
    GPS_Data.isGetData = false;
    GPS_Data.isParseData = false;
    GPS_Data.isUsefull = false;
    
    memset(GPS_Data.GPS_Buffer, 0, GPS_Buffer_Length);
    memset(GPS_Data.UTCTime, 0, UTCTime_Length);
    memset(GPS_Data.latitude, 0, latitude_Length);
    memset(GPS_Data.N_S, 0, N_S_Length);
    memset(GPS_Data.longitude, 0, longitude_Length);
    memset(GPS_Data.E_W, 0, E_W_Length);
}
void GPS_GetValue(void)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    if (GPS_Data.isGetData)
    {
        GPS_Data.isGetData = false;
        
        for (i = 0 ; i <= 6 ; i++)
        {
            if (i == 0)
            {
                if ((subString = strstr(GPS_Data.GPS_Buffer, ",")) == NULL)
                {
                    //解析错误
                    return ;
                }    
            }
            else
            {
                subString++;
                if ((subStringNext = strstr(subString, ",")) != NULL)
                {
                    char usefullBuffer[2]; 
                    switch(i)
                    {
                        case 1:memcpy(GPS_Data.UTCTime, subString, subStringNext - subString);break;        //获取UTC时间
                        case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;           //获取数据是否有效
                        case 3:memcpy(GPS_Data.latitude, subString, subStringNext - subString);break;       //获取纬度信息
                        case 4:memcpy(GPS_Data.N_S, subString, subStringNext - subString);break;            //获取N/S
                        case 5:memcpy(GPS_Data.longitude, subString, subStringNext - subString);break;      //获取经度信息
                        case 6:memcpy(GPS_Data.E_W, subString, subStringNext - subString);break;            //获取E/W

                        default:break;
                    }

                    subString = subStringNext;
                    GPS_Data.isParseData = true;
                    if(usefullBuffer[0] == 'A')
                        GPS_Data.isUsefull = true;
                    else if(usefullBuffer[0] == 'V')
                        GPS_Data.isUsefull = false;
                }
                else
                {
                    //解析错误
                }
            }
        }
    }
}
/**
 * @brief   获取经度和维度
 * @param   latitude：维度
 * @param   longitude：精度
 * @retval  none
 */
void GPS_GetPosition(float * latitude, float *longitude)
{
    GPS_GetValue();
    if (GPS_Data.isParseData)
    {
        GPS_Data.isParseData = false;
        
        if(GPS_Data.isUsefull)
        {
            GPS_Data.isUsefull = false;
            *latitude = atof(GPS_Data.latitude);    //获取纬度
            *longitude = atof(GPS_Data.longitude);  //获取经度
        }
    }
}

#endif

#endif
