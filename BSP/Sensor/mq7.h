#ifndef __MQ7_H
#define __MQ7_H
#include "adcx.h"
#include "sys.h"
#include "math.h"

//模式选择    
//模拟AO:    1
//数字DO:    0
#define    MQ7_MODE     1

/***************根据自己需求更改****************/
// MQ-7 GPIO宏定义
#if MQ7_MODE
#define     MQ7_AO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MQ7_AO_GPIO_PORT                  GPIOA
#define     MQ7_AO_GPIO_PIN                   GPIO_Pin_0
#define     MQ7_ADC_CHANNEL                   ADC_Channel_0    // ADC 通道宏定义

#else
#define     MQ7_DO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MQ7_DO_GPIO_PORT                  GPIOA
#define     MQ7_DO_GPIO_PIN                   GPIO_Pin_1            

#endif
/*********************END**********************/


void MQ7_Init(void);
float MQ7_GetData_PPM(void);

#endif /* __ADC_H */

