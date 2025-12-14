#include "jq6500.h"
#include <stdarg.h>

void JQ6500_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(JQ6500_GPIO_RCC, ENABLE);
    if (JQ6500_USARTx == 1)
        RCC_APB2PeriphClockCmd(JQ6500_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(JQ6500_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = JQ6500_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JQ6500_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = JQ6500_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(JQ6500_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(JQ6500_USART_UX, &USART_InitStruct);

    USART_Cmd(JQ6500_USART_UX, ENABLE);
}

 /*发送一个字节数据*/
void UARTxSendByte(unsigned char SendData)
{       
    USART_SendData(JQ6500_USART_UX,SendData);
    while(USART_GetFlagStatus(JQ6500_USART_UX, USART_FLAG_TXE) == RESET);        
} 

void JQ6500_Play(uint8_t dat)
{
    unsigned char  music_code[6] = {0X7E,0X04,0X03,0X00,0X01,0XEF};    
    int i;
    music_code[4] = dat;
    for(i=0;i<6;i++)
    {
        UARTxSendByte(music_code[i]);
    }
}

