#ifndef __FSR402_H
#define __FSR402_H

#include "adcx.h"
#include "sys.h"
#include "math.h"

/**
 *  模拟AO:    1
 *  数字DO:    0
*/
#define    FSR402_MODE     1

/***************根据自己需求更改****************/
// FSR402 GPIO宏定义
#if FSR402_MODE
#define     FSR402_AO_GPIO_CLK                        RCC_APB2Periph_GPIOA
#define     FSR402_AO_GPIO_PORT                       GPIOA
#define     FSR402_AO_GPIO_PIN                        GPIO_Pin_0
#define     FSR402_ADC_CHANNEL                        ADC_Channel_0    // ADC 通道宏定义

#else
#define     FSR402_DO_GPIO_CLK                        RCC_APB2Periph_GPIOA
#define     FSR402_DO_GPIO_PORT                       GPIOA
#define     FSR402_DO_GPIO_PIN                        GPIO_Pin_1            

#endif
/*********************END**********************/


void FSR402_Init(void);
uint16_t FSR402_GetStress(void);

#endif /* __ADC_H */

