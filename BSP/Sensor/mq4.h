#ifndef __MQ4_H
#define __MQ4_H
#include "sys.h"
#include "adcx.h"

#define     MQ4_AO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MQ4_AO_GPIO_PORT                  GPIOA
#define     MQ4_AO_GPIO_PIN                   GPIO_Pin_0
#define     MQ4_ADC_CHANNEL                   ADC_Channel_0    // ADC 通道宏定义

void MQ4_Init(void);
float MQ4_GetPPM(void);

#endif

