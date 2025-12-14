#include "mq7.h"

void MQ7_Init(void)
{
    #if MQ7_MODE
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (MQ7_AO_GPIO_CLK, ENABLE );              // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ7_AO_GPIO_PIN;                  // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                   // 设置为模拟输入
        
        GPIO_Init(MQ7_AO_GPIO_PORT, &GPIO_InitStructure);               // 初始化 ADC IO

    }
    #else
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd (MQ7_DO_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ7_DO_GPIO_PIN;            // 配置连接 传感器DO 的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
        
        GPIO_Init(MQ7_DO_GPIO_PORT, &GPIO_InitStructure);                // 初始化 
        
    }
    #endif
    
}

#if MQ7_MODE
uint16_t MQ7_ADC_Read(void)
{
    //设置指定ADC的规则组通道，采样时间
    return ADC_GetValue(MQ7_ADC_CHANNEL, ADC_SampleTime_55Cycles5);
}
#endif

float MQ7_GetData_PPM(void)
{
    #if MQ7_MODE
    float adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[MQ7_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    float Vol = (adc_sum*5/4096);
    float RS = (5-Vol)/(Vol*0.5);
    float R0=6.64;
    
    float ppm = pow(11.5428*R0/RS, 0.6549f);
    
    return ppm;
    #endif
}
