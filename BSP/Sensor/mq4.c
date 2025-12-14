#include "mq4.h"

#define MAX_CONVERTED_VALUE   4095    /* Max converted value */
#define VREF                  3300

void MQ4_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

 
float MQ4_GetPPM(void)
{
    float ppm = 0;
    float adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[MQ4_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    ppm =adc_sum * VREF / (MAX_CONVERTED_VALUE * 1.0) ;
    return ppm;
}
