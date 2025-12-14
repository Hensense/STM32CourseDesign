#include "LDR.h"
/* 光敏传感器 */
void LDR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (LDR_GPIO_CLK, ENABLE );             // 打开 ADC IO端口时钟
    GPIO_InitStructure.GPIO_Pin = LDR_GPIO_PIN;                 // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;               // 设置为模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LDR_GPIO_PORT, &GPIO_InitStructure);              // 初始化 ADC IO
    
}

uint16_t LDR_GetValue(void)
{
    uint16_t adc_sum = 0;
    float voltage = 0; 
    float R = 0;
    uint16_t Lux = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[LDR_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    voltage = adc_sum;
    voltage  = voltage / 4096 * 3.3f;
    
    R = voltage / (3.3f - voltage) * 10000;
        
    Lux = 40000 * pow(R, -0.6021);
    
    if (Lux > 999)
    {
        Lux = 999;
    }
    return Lux;
}

