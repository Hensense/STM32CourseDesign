#include "ph.h"

void PH_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (PH_GPIO_CLK, ENABLE );    // 打开 ADC IO端口时钟
    GPIO_InitStructure.GPIO_Pin = PH_GPIO_PIN;                    // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 设置为模拟输入
    
    GPIO_Init(PH_GPIO_PORT, &GPIO_InitStructure);                // 初始化 ADC IO
}

float PH_GetValue(void)
{
    float PH_DAT;
    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[PH_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    PH_DAT = (adc_sum/4096.0)*3.3;
    PH_DAT = -5.7541*PH_DAT+16.654;
    
    if(PH_DAT>14.0)PH_DAT=14.0;
    if(PH_DAT<0)PH_DAT=0.0;
    
    return PH_DAT;

}



