#include "rain.h"

void RAIN_Init(void)
{
    #if RAIN_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (RAIN_AO_GPIO_CLK, ENABLE );     // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = RAIN_AO_GPIO_PIN;         // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // 设置为模拟输入
        
        GPIO_Init(RAIN_AO_GPIO_PORT, &GPIO_InitStructure);      // 初始化 ADC IO

    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (RAIN_DO_GPIO_CLK, ENABLE );     // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = RAIN_DO_GPIO_PIN;         // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 设置为上拉输入
        
        GPIO_Init(RAIN_DO_GPIO_PORT, &GPIO_InitStructure);      // 初始化 
        
    }
    #endif
    
}

uint16_t RAIN_GetValue(void)
{
    
    #if RAIN_MODE
    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[RAIN_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    return 4095 - adc_sum;
    
    #else
    uint16_t tempData;
    tempData = !GPIO_ReadInputDataBit(RAIN_DO_GPIO_PORT, RAIN_DO_GPIO_PIN);
    return tempData;
    #endif
}

