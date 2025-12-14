#include "JW013.h"
#include <stdbool.h>
#if JW013_START_STATE

unsigned char Jw013RecBuf[JW013_RXBUFF_SIZE];//串口2接收数据缓存
unsigned int  Jw013Counter = 0;   //串口2收到数据标志位
bool rev_start  = 0;     //接收开始标志
bool rev_stop   = 0;     //接收停止标志

void JW013_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(JW013_GPIO_RCC, ENABLE);
    if (JW013_USARTx == 1)
        RCC_APB2PeriphClockCmd(JW013_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(JW013_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = JW013_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JW013_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = JW013_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(JW013_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(JW013_USART_UX, &USART_InitStruct);

    // 4. 配置中断
    USART_ITConfig(JW013_USART_UX, USART_IT_RXNE, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = JW013_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(JW013_USART_UX, ENABLE);
}

void JW013_USART_IRQHandler(void)
{
    uint8_t Res;

    if(USART_GetITStatus(JW013_USART_UX, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(JW013_USART_UX);//接收模块的数据
        if ((Res == 0x2C)&&(rev_stop==0))  //如果收到0x2C，便开始接收
        {
                rev_start = 1;
        }
        if (rev_start == 1)  //标志位为1，开始接收
        {
            Jw013RecBuf[Jw013Counter] = Res;  //字符存到数组中
            Jw013Counter ++;
            if (Jw013Counter >= 9)              //一帧数据接收完成
            {
                    rev_start = 0;
                    rev_stop  = 1;
                    Jw013Counter = 0;
            }
        }
    }

    USART_ClearITPendingBit(JW013_USART_UX, USART_IT_RXNE);
} 


float JW013_GetCH2O(void)  //获取甲醛
{
    char i = 0;
    u16 sum1=0;
    u8  sum2=0;
    float ch20 = 0;

    if(rev_stop == 1)                   //数据接收完成了
    {
        for(i = 0; i < 8; i++)
        {
            sum1 += Jw013RecBuf[i];   //累加和
        }
        sum2=(unsigned char)sum1;
        if(sum2 == Jw013RecBuf[8])  //校验数据是否正确: 校验和(B8)=unit_8(B0+B1+B2+B3+B4+B5+B6+B7)
        {
            ch20 = (Jw013RecBuf[4]*256 + Jw013RecBuf[5]); //计算甲醛值 
        }
        rev_stop = 0;
    }
    return ch20/1000;
}
#endif
