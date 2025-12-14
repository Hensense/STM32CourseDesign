#include "HC05.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#if HC05_START_STATE
#include "Comm_Task.h"
HC05_Rec hc05;
//底层串口初始化
void HC05_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(HC05_GPIO_RCC, ENABLE);
    if (HC05_USARTx == 1)
        RCC_APB2PeriphClockCmd(HC05_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(HC05_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = HC05_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HC05_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = HC05_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HC05_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(HC05_USART_UX, &USART_InitStruct);

    // 4. 配置中断
    USART_ITConfig(HC05_USART_UX, USART_IT_RXNE, ENABLE);
    USART_ITConfig(HC05_USART_UX, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = HC05_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(HC05_USART_UX, ENABLE);
}
// 发送一个字节数据
void HC05_SendByte(uint8_t data)
{
    USART_SendData(HC05_USART_UX, data);       //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(HC05_USART_UX, USART_FLAG_TXE) == RESET);   //等待发送完成
}
void HC05_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        HC05_SendByte(String[i]);       //依次调用Serial_SendByte发送每个字节数据
    }
}
void HC05_Printf(char *format, ...)
{
    char String[100];               //定义字符数组
    va_list arg;                    //定义可变参数列表数据类型的变量arg
    va_start(arg, format);          //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                    //结束变量arg
    HC05_SendString(String);        //串口发送字符数组（字符串）
}

void HC05_DataAnalysis(void)
{
#if HC05_ENABLE  ==  _enable_
    if(strcmp((char*)hc05.rxbuff, "A") == 0)
    {
        HC05_DataInfo.Led_State = 1;
        HC05_DataInfo.Led_State_Flag = 1;
    }
    //........
    hc05.rxcount = 0;
    memset(hc05.rxbuff, 0, hc05.rxcount);
    HC05_DataInfo.Update_Flag = 1;
#endif
}
void HC05_USART_IRQHandler(void)
{
    if(USART_GetITStatus(HC05_USART_UX, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(HC05_USART_UX, USART_IT_RXNE);
        hc05.rxbuff[hc05.rxcount++] = USART_ReceiveData(HC05_USART_UX);
    }
    if(USART_GetITStatus(HC05_USART_UX, USART_IT_IDLE))
    {
        (void)HC05_USART_UX->DR; // 清除空闲中断标志
        
        HC05_DataAnalysis();
    }
    
}
#endif

