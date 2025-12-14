#ifndef __TS_H
#define __TS_H
#include "adcx.h"
#include "sys.h"

#define TS_READ_TIMES    10  //TS浊度传感器ADC循环读取次数
#define TS_K 2047.19

#define     TS_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     TS_GPIO_PORT                            GPIOA
#define     TS_GPIO_PIN                             GPIO_Pin_0
#define     TS300B_ADC_CHANNEL                     ADC_Channel_0    // ADC 通道宏定义

void TS300B_Init(void);
float TS300B_GetValue(void);

#endif /* __TS_H */

