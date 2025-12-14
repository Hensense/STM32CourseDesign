#include "water.h"
/* 水位传感器 */
void WATER_Init(void)
{
    #if WATER_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (WATER_AO_GPIO_CLK, ENABLE );    // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = WATER_AO_GPIO_PIN;        // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // 设置为模拟输入
        
        GPIO_Init(WATER_AO_GPIO_PORT, &GPIO_InitStructure);     // 初始化 ADC IO

    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (WATER_DO_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = WATER_DO_GPIO_PIN;        // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 设置为上拉输入
        
        GPIO_Init(WATER_DO_GPIO_PORT, &GPIO_InitStructure);     // 初始化 
    }
    #endif
    
}

uint16_t WATER_GetValue(void)
{
    
    #if WATER_MODE
    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[WATER_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    return adc_sum;
    
    #else
    uint16_t tempData;
    tempData = GPIO_ReadInputDataBit(WATER_DO_GPIO_PORT, WATER_DO_GPIO_PIN);
    return tempData;
    #endif
}



