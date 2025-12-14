#include "LORA.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#if LORA_START_STATE

#include "Comm_Task.h"
LORA_Rec LORA;
//底层串口初始化
void LORA_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(LORA_GPIO_RCC, ENABLE);
    if (LORA_USARTx == 1)
        RCC_APB2PeriphClockCmd(LORA_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(LORA_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = LORA_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LORA_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = LORA_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(LORA_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(LORA_USART_UX, &USART_InitStruct);

    // 4. 配置中断
    USART_ITConfig(LORA_USART_UX, USART_IT_RXNE, ENABLE);
    USART_ITConfig(LORA_USART_UX, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = LORA_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(LORA_USART_UX, ENABLE);
}
// 发送一个字节数据
void LORA_SendByte(uint8_t data)
{
    USART_SendData(LORA_USART_UX, data);       //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(LORA_USART_UX, USART_FLAG_TXE) == RESET);   //等待发送完成
}
void LORA_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        LORA_SendByte(String[i]);       //依次调用Serial_SendByte发送每个字节数据
    }
}
void LORA_Printf(char *format, ...)
{
    char String[100];               //定义字符数组
    va_list arg;                    //定义可变参数列表数据类型的变量arg
    va_start(arg, format);          //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                    //结束变量arg
    LORA_SendString(String);        //串口发送字符数组（字符串）
}

void LORA_DataAnalysis(void)
{
#if LORA_ENABLE  ==  _enable_
    /* "Temp:34,Humi:86" */
    char *str;
    if((str = strstr((char*)LORA.rxbuff, "Temp")) != NULL)
    {
        // 跳过"Temp:"部分（5个字符）
        char *temp_start = str + 5;
        // 找到温度值的结束位置（逗号或字符串结束）
        char *temp_end = temp_start;
        while(*temp_end && *temp_end != ',' && isdigit(*temp_end)) {
            temp_end++;
        }
        // 提取温度值
        char temp_str[10] = {0};
        strncpy(temp_str, temp_start, temp_end - temp_start);
        LORA_Ctrler.Temp = (uint8_t)atoi(temp_str);
    }
    if((str = strstr((char*)LORA.rxbuff, "Humi")) != NULL)
    {
        // 跳过"Humi:"部分（5个字符）
        char *humi_start = str + 5;
        // 找到湿度值的结束位置（逗号或字符串结束）
        char *humi_end = humi_start;
        while(*humi_end && *humi_end != ',' && isdigit(*humi_end)) {
            humi_end++;
        }
        // 提取湿度值
        char humi_str[10] = {0};
        strncpy(humi_str, humi_start, humi_end - humi_start);
        LORA_Ctrler.Humi = (uint8_t)atoi(humi_str);
        
    }
    //........
    LORA.rxcount = 0;
    memset(LORA.rxbuff, 0, LORA.rxcount);
    LORA_Ctrler.Update_Flag = 1;
#endif
}
void LORA_USART_IRQHandler(void)
{
    if(USART_GetITStatus(LORA_USART_UX, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(LORA_USART_UX, USART_IT_RXNE);
        LORA.rxbuff[LORA.rxcount++] = USART_ReceiveData(LORA_USART_UX);
    }
    if(USART_GetITStatus(LORA_USART_UX, USART_IT_IDLE))
    {
        (void)LORA_USART_UX->DR; // 清除空闲中断标志
        
        LORA_DataAnalysis();
    }
    
}
#endif

