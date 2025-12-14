#ifndef     __TS_H
#define     __TS_H
#include    "adcx.h"
#include    "sys.h"

#define     TS_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     TS_GPIO_PORT                            GPIOA
#define     TS_GPIO_PIN                             GPIO_Pin_2
#define     TS_ADC_CHANNEL                          ADC_Channel_2    // ADC 通道宏定义


void TS_Init(void);
uint16_t TS_GetValue(void);

#endif 

