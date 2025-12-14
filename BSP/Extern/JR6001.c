#include "JR6001.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//底层串口初始化
void JR6001_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(JR6001_GPIO_RCC, ENABLE);
    if (JR6001_USARTx == 1)
        RCC_APB2PeriphClockCmd(JR6001_RCC_APBPeriph, ENABLE);
    else
        RCC_APB1PeriphClockCmd(JR6001_RCC_APBPeriph, ENABLE);

    // 2. 配置GPIO
    GPIO_InitStruct.GPIO_Pin = JR6001_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JR6001_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = JR6001_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(JR6001_GPIO_RX_PORT, &GPIO_InitStruct);

    // 3. 配置USART
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(JR6001_USART_UX, &USART_InitStruct);

    USART_Cmd(JR6001_USART_UX, ENABLE);
    
    JR6001_SetVolume(24);   /* 默认音量 */
}
// 发送一个字节数据
void JR6001_SendByte(uint8_t data)
{
    USART_SendData(JR6001_USART_UX, data);       //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(JR6001_USART_UX, USART_FLAG_TXE) == RESET);   //等待发送完成
}
void JR6001_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        JR6001_SendByte(String[i]);       //依次调用Serial_SendByte发送每个字节数据
    }
}
void JR6001_Printf(char *format, ...)
{
    char String[100];               //定义字符数组
    va_list arg;                    //定义可变参数列表数据类型的变量arg
    va_start(arg, format);          //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                    //结束变量arg
    JR6001_SendString(String);        //串口发送字符数组（字符串）
}
/* 设置音量 volume：0~30 */
void JR6001_SetVolume(uint8_t volume)
{
    JR6001_Printf("AF:%02d", volume);
}
/* 停止播放 */
void JR6001_StopPlay(void)
{
    JR6001_Printf("AE");
}
/* 播放语音 num:00001~99999 */
void JR6001_PlayMusic(uint32_t num)
{
    JR6001_Printf("A7:%05d", num);
}

