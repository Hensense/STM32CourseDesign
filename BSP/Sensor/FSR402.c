#include "FSR402.h"
/* 薄膜压力传感器 */
void FSR402_Init(void)
{
    #if FSR402_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (FSR402_AO_GPIO_CLK, ENABLE );           // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = FSR402_AO_GPIO_PIN;               // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                   // 设置为模拟输入
        
        GPIO_Init(FSR402_AO_GPIO_PORT, &GPIO_InitStructure);            // 初始化 ADC IO

    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (FSR402_DO_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = FSR402_DO_GPIO_PIN;        // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
        
        GPIO_Init(FSR402_DO_GPIO_PORT, &GPIO_InitStructure);     // 初始化 
        
    }
    #endif
    
}

uint16_t FSR402_GetStress(void)
{
    
    #if FSR402_MODE
    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[FSR402_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    return 4095 - (uint16_t)adc_sum;
    
    #else
    uint16_t tempData;
    tempData = !GPIO_ReadInputDataBit(FSR402_DO_GPIO_PORT, FSR402_DO_GPIO_PIN);
    return tempData;
    #endif
}



