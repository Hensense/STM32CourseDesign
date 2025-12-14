#ifndef __MS1100_H
#define __MS1100_H
#include "sys.h"
#include "adcx.h"

/* 模式选择 模拟输出:1 数字输出:0 */
#define    MS1100_MODE     1
#if MS1100_MODE
#define     MS1100_AO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MS1100_AO_GPIO_PORT                  GPIOA
#define     MS1100_AO_GPIO_PIN                   GPIO_Pin_0
#define     MS1100_ADC_CHANNEL                   ADC_Channel_0    // ADC 通道宏定义

#else
#define     MS1100_DO_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     MS1100_DO_GPIO_PORT                  GPIOA
#define     MS1100_DO_GPIO_PIN                   GPIO_Pin_1            

#endif
void MS1100_Init(void);
uint16_t MS1100_GetValue(void);

#endif

