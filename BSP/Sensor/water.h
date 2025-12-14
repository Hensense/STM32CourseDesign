#ifndef __WATER_H
#define    __WATER_H
#include "adcx.h"
#include "sys.h"

/**
 *  模拟AO:    1    接触式水位传感器使用
 *  数字DO:    0    非接触式水位传感器使用
*/
#define    WATER_MODE     1

/***************根据自己需求更改****************/
// WATER GPIO宏定义
#if WATER_MODE
#define     WATER_AO_GPIO_CLK                                   RCC_APB2Periph_GPIOA
#define     WATER_AO_GPIO_PORT                                  GPIOA
#define     WATER_AO_GPIO_PIN                                   GPIO_Pin_0
#define     WATER_ADC_CHANNEL                                   ADC_Channel_0    // ADC 通道宏定义

#else
#define     WATER_DO_GPIO_CLK                                   RCC_APB2Periph_GPIOA
#define     WATER_DO_GPIO_PORT                                  GPIOA
#define     WATER_DO_GPIO_PIN                                   GPIO_Pin_1            

#endif
/*********************END**********************/


void WATER_Init(void);
uint16_t WATER_GetValue(void);

#endif /* __WATER_H */

