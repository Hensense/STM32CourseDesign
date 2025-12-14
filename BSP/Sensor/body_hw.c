#include "body_hw.h"

void BODY_HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (BODY_HW_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = BODY_HW_GPIO_PIN;        // 配置连接 传感器DO 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          // 设置为下拉输入
    
    GPIO_Init(BODY_HW_GPIO_PORT, &GPIO_InitStructure);     // 初始化 
    
}

/* 本函数每次状态改变 - 只会返回一次结果（有人/无人），若状态不改变 则返回0x00，可不做处理 */
uint8_t BODY_HW_GetValue(void)
{
    return GPIO_ReadInputDataBit(BODY_HW_GPIO_PORT, BODY_HW_GPIO_PIN);
}



