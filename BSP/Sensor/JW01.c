#include "JW01.h"
#if JW01_START_STATE
uint8_t Jw01_Usart_RxPacket[6] = {0};
uint8_t Jw01_Usart_RxFlag = 0;

void JW01_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(JW01_GPIO_RCC, ENABLE);
    if (JW01_USARTx == 1)
        RCC_APB2PeriphClockCmd(JW01_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(JW01_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = JW01_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JW01_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = JW01_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(JW01_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(JW01_USART_UX, &USART_InitStruct);

    // 4. 配置中断
    USART_ITConfig(JW01_USART_UX, USART_IT_RXNE, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = JW01_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(JW01_USART_UX, ENABLE);
}

 void JW01_USART_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    uint8_t Res;

    if (USART_GetITStatus(JW01_USART_UX, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(JW01_USART_UX);
        
        if (Jw01_Usart_RxFlag == 1) return; // 等待主程序处理

        switch (RxState)
        {
            case 0: // 等待帧头 0x2C
                if (Res == 0x2C)
                {
                    Jw01_Usart_RxPacket[pRxPacket++] = Res;
                    RxState = 1;
                }
                break;

            case 1: // 接收数据部分
                Jw01_Usart_RxPacket[pRxPacket++] = Res;
                if (pRxPacket >= 6)
                {
                    pRxPacket = 0;
                    RxState = 2;
                }
                break;

            case 2: // 校验数据
                if (Jw01_Usart_RxPacket[5] == (uint8_t)(Jw01_Usart_RxPacket[0] + Jw01_Usart_RxPacket[1] + 
                                                 Jw01_Usart_RxPacket[2] + Jw01_Usart_RxPacket[3] + Jw01_Usart_RxPacket[4]))
                {
                    Jw01_Usart_RxFlag = 1;
                }
                RxState = 0;
                break;
        }
        USART_ClearITPendingBit(JW01_USART_UX, USART_IT_RXNE);
    }
}

void JW01_GetCo2(uint16_t *data)
{
    if (Jw01_Usart_RxFlag)
    {
        Jw01_Usart_RxFlag = 0;
        *data = Jw01_Usart_RxPacket[1] * 256 + Jw01_Usart_RxPacket[2];
    }
}
#endif
