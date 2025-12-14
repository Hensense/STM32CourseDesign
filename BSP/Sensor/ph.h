#ifndef __PH_H
#define __PH_H
#include "sys.h"
#include "adcx.h"

#define PH_READ_TIMES    10  //PH传感器ADC循环读取次数

#define     PH_GPIO_CLK                            RCC_APB2Periph_GPIOA
#define     PH_GPIO_PORT                           GPIOA
#define     PH_GPIO_PIN                            GPIO_Pin_0
#define     PH_ADC_CHANNEL                         ADC_Channel_0    // ADC 通道宏定义

void PH_Init(void);
float PH_GetValue(void);

#endif /* __PH_H */

