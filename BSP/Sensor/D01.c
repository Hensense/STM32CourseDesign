#include "D01.h"

#if D01_START_STATE
volatile uint8_t ucTemp;            // 用于存储接收到的字节
volatile uint8_t u1_number = 0;     // 用于记录接收到的字节数量
volatile uint16_t DATAH = 0;        // 存储数据字节1
volatile uint8_t DATAL = 0;         // 存储数据字节2
volatile uint8_t CHECKSUM = 0;      // 存储校验字节
volatile uint16_t PM2_5_Value;      // 存储粉尘浓度值
/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void D01_Init(void)
{
    /*开启时钟*/
    #if D01_USARTx == 1
        RCC_APB2PeriphClockCmd(D01_RCC_APBPeriph, ENABLE);    //开启USART1的时钟
    #else
        RCC_APB1PeriphClockCmd(D01_RCC_APBPeriph, ENABLE);    //开启USART2/3的时钟
    #endif
    RCC_APB2PeriphClockCmd(D01_GPIO_RCC, ENABLE);            //开启GPIO的时钟
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = D01_GPIO_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(D01_GPIO_TX_PORT, &GPIO_InitStructure);         //将TX引脚初始化为复用推挽输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = D01_GPIO_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(D01_GPIO_RX_PORT, &GPIO_InitStructure);         //将RX引脚初始化为上拉输入
    
    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;                    //定义结构体变量
    USART_InitStructure.USART_BaudRate = 9600;                //波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //模式，发送模式和接收模式均选择
    USART_InitStructure.USART_Parity = USART_Parity_No;        //奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //停止位，选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //字长，选择8位
    USART_Init(D01_USART_UX, &USART_InitStructure);           //将结构体变量交给USART_Init，配置USART
    
    /*中断输出配置*/
    USART_ITConfig(D01_USART_UX, USART_IT_RXNE, ENABLE);      //开启串口接收数据的中断
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;                      //定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = D01_USART_IRQn;      //选择配置NVIC的USART线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //指定NVIC线路的抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);                           //将结构体变量交给NVIC_Init，配置NVIC外设
    
    /*USART使能*/
    USART_Cmd(D01_USART_UX, ENABLE);                          //使能USART，串口开始运行
}
 
uint16_t D01_GetValue(void)
{
    return PM2_5_Value;
}

/**
  * 函    数：USART中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void D01_USART_IRQHandler(void)
{
    if(USART_GetITStatus(D01_USART_UX, USART_IT_RXNE) != RESET)
    {
        ucTemp = USART_ReceiveData(D01_USART_UX); // 读取接收到的数据
 
        if(u1_number == 0 && ucTemp == 0xA5) // 如果是第一个字节且为特征字节
        {
            u1_number++;
        }
        else if(u1_number > 0 && u1_number < 4) // 如果已经接收到特征字节，继续接收数据
        {
            if(u1_number == 1)
            {
                DATAH = ucTemp;     // 存储数据字节1
                u1_number++;
            }
            else if(u1_number == 2)
            {
                DATAL = ucTemp;     // 存储数据字节2
                u1_number++;
            }
            else if(u1_number == 3)
            {
                CHECKSUM = ucTemp;  // 存储校验字节
 
                uint8_t sum = 0xA5 + DATAH + DATAL; // 计算校验和
                sum = sum ^ 0x80;   // ^ 异或，得到低7位数据
 
                if(sum != CHECKSUM) // 如果最高位为1，则校验失败
                {
                    // 可以在这里添加错误处理代码
                    u1_number = 0; //从新计数
                }
                else
                {
                    // 校验成功，计算浓度值
                    uint16_t concentration = ((DATAH & 0x7F) << 7) | (DATAL & 0x7F);
                    // 这里可以添加代码将浓度值发送给其他模块或显示
                    PM2_5_Value = concentration;
                }
                u1_number = 0; // 重置计数器，准备接收下一帧数据
            }
        }
        else
        {
            u1_number = 0;  // 如果接收到的数据不是特征字节，重置计数器
        }
    }
}

#endif
