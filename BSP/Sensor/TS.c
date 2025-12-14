#include "TS.h"
/* 土壤湿度传感器 */
void TS_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd (TS_GPIO_CLK, ENABLE );              // 打开 ADC IO端口时钟
    GPIO_InitStructure.GPIO_Pin = TS_GPIO_PIN;                  // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;               // 设置为模拟输入

    GPIO_Init(TS_GPIO_PORT, &GPIO_InitStructure);               // 初始化 ADC
}

uint16_t TS_GetValue(void)
{
    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[TS_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    return 100 - (float)adc_sum/40.96;
}



