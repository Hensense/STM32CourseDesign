#include "mq3.h"

void MQ3_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
float MQ3_GetValue(void)
{
    float adc_sum = 0;
    float adc_value;
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[MQ3_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    adc_value =adc_sum * 125/4095.0;
    return adc_value;

}
