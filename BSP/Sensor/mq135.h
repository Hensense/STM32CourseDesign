#ifndef __MQ135_H
#define __MQ135_H
#include "adcx.h"
#include "sys.h"
#include "math.h"

//模式选择    
//模拟AO:    1
//数字DO:    0
#define    MQ135_MODE     1

/***************根据自己需求更改****************/
// MQ-135 GPIO宏定义
#if MQ135_MODE
#define     MQ135_AO_GPIO_CLK                           RCC_APB2Periph_GPIOA
#define     MQ135_AO_GPIO_PORT                          GPIOA
#define     MQ135_AO_GPIO_PIN                           GPIO_Pin_0
#define     MQ135_ADC_CHANNEL                           ADC_Channel_0    // ADC 通道宏定义

#else
#define     MQ135_DO_GPIO_CLK                           RCC_APB2Periph_GPIOA
#define     MQ135_DO_GPIO_PORT                           GPIOA
#define     MQ135_DO_GPIO_PIN                            GPIO_Pin_1            

#endif
/*********************END**********************/


void MQ135_Init(void);
float MQ135_GetData_PPM(void);

#endif /* __ADC_H */

