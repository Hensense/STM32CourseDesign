#include "mq135.h"

void MQ135_Init(void)
{
    #if MQ135_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (MQ135_AO_GPIO_CLK, ENABLE );    // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ135_AO_GPIO_PIN;                    // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 设置为模拟输入
        
        GPIO_Init(MQ135_AO_GPIO_PORT, &GPIO_InitStructure);                // 初始化 ADC IO

    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (MQ135_DO_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ135_DO_GPIO_PIN;            // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
        
        GPIO_Init(MQ135_DO_GPIO_PORT, &GPIO_InitStructure);                // 初始化 
        
    }
    #endif
    
}

float MQ135_GetData_PPM(void)
{
    #if MQ135_MODE
    float adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[MQ135_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    float Vol = (adc_sum*5/4096);
    float RS = (5-Vol)/(Vol*0.5);
    float R0=6.64;
    
    float ppm = pow(11.5428*R0/RS, 0.6549f);
    
    return ppm;
    #endif
}
