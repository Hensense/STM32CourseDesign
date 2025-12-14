#ifndef __MQ3_H
#define __MQ3_H
#include "sys.h"
#include "adcx.h"

#define     MQ3_READ_TIMES    10            //MQ-3酒精浓度传感器ADC循环读取次数

#define     MQ3_AO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MQ3_AO_GPIO_PORT                  GPIOA
#define     MQ3_AO_GPIO_PIN                   GPIO_Pin_0
#define     MQ3_ADC_CHANNEL                   ADC_Channel_0    // ADC 通道宏定义

void MQ3_Init(void);
float MQ3_GetValue(void);

#endif

