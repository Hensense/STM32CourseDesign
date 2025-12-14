#include "IR.h"
/* 火焰传感器 */
void IR_Init(void)
{
    #if IR_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (IR_AO_GPIO_CLK, ENABLE );    // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = IR_AO_GPIO_PIN;        // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 设置为模拟输入
        
        GPIO_Init(IR_AO_GPIO_PORT, &GPIO_InitStructure);     // 初始化 ADC IO
    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (IR_DO_GPIO_CLK, ENABLE );           // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = IR_DO_GPIO_PIN;               // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               // 设置为上拉输入
        
        GPIO_Init(IR_DO_GPIO_PORT, &GPIO_InitStructure);            // 初始化 
    }
    #endif
    
}

#if IR_MODE
uint16_t IR_ADC_Read(void)
{
    //设置指定ADC的规则组通道，采样时间
    return ADC_GetValue(IR_ADC_CHANNEL, ADC_SampleTime_55Cycles5);
}
#endif

uint16_t IR_FireData(void)
{
    uint16_t  tempData = 0;
    #if IR_MODE

    tempData = IR_ADC_Read();
    
    return 4095 - (uint16_t)tempData;
    
    #else
    tempData = !GPIO_ReadInputDataBit(IR_DO_GPIO_PORT, IR_DO_GPIO_PIN);
    return tempData;
    #endif
}



