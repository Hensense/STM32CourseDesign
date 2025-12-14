#include "ASRPRO.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#if ASRPRO_START_STATE

#include "Extern_Task.h"
ASRPRO_Rec ASRPRO;
//底层串口初始化
void ASRPRO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(ASRPRO_GPIO_RCC, ENABLE);
    if (ASRPRO_USARTx == 1)
        RCC_APB2PeriphClockCmd(ASRPRO_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(ASRPRO_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = ASRPRO_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ASRPRO_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASRPRO_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ASRPRO_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(ASRPRO_USART_UX, &USART_InitStruct);

    // 4. 配置中断
    USART_ITConfig(ASRPRO_USART_UX, USART_IT_RXNE, ENABLE);
    USART_ITConfig(ASRPRO_USART_UX, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = ASRPRO_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(ASRPRO_USART_UX, ENABLE);
}
// 发送一个字节数据
void ASRPRO_SendByte(uint8_t data)
{
    USART_SendData(ASRPRO_USART_UX, data);       //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(ASRPRO_USART_UX, USART_FLAG_TXE) == RESET);   //等待发送完成
}
void ASRPRO_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        ASRPRO_SendByte(String[i]);       //依次调用Serial_SendByte发送每个字节数据
    }
}
void ASRPRO_Printf(char *format, ...)
{
    char String[100];               //定义字符数组
    va_list arg;                    //定义可变参数列表数据类型的变量arg
    va_start(arg, format);          //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                    //结束变量arg
    ASRPRO_SendString(String);        //串口发送字符数组（字符串）
}

void ASRPRO_DataAnalysis(void)
{
#if ASRPRO_ENABLE  ==  _enable_
    /* 获取控制命令 */
    if(strstr((char*)ASRPRO.rxbuff, "Led_On"))  
    {
        ASRPRO_Ctrler.Led_State = 1;
        ASRPRO_Ctrler.Led_State_Flag = 1;
    }
    else if(strstr((char*)ASRPRO.rxbuff, "Led_Off"))
    {
        ASRPRO_Ctrler.Led_State = 0;
        ASRPRO_Ctrler.Led_State_Flag = 1;
    }
    //........
    ASRPRO.rxcount = 0;
    memset(ASRPRO.rxbuff, 0, ASRPRO.rxcount);
    ASRPRO_Ctrler.Update_Flag = 1;
#endif
}
void ASRPRO_USART_IRQHandler(void)
{
    if(USART_GetITStatus(ASRPRO_USART_UX, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(ASRPRO_USART_UX, USART_IT_RXNE);
        ASRPRO.rxbuff[ASRPRO.rxcount++] = USART_ReceiveData(ASRPRO_USART_UX);
    }
    if(USART_GetITStatus(ASRPRO_USART_UX, USART_IT_IDLE))
    {
        (void)ASRPRO_USART_UX->DR; // 清除空闲中断标志
        
        ASRPRO_DataAnalysis();
    }
    
}
#endif

