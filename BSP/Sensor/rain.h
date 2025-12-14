#ifndef __RAIN_H
#define    __RAIN_H
#include "sys.h"
#include "adcx.h"

#define RAIN_READ_TIMES    10  //雨滴传感器ADC循环读取次数

//模式选择    
//模拟AO:    1
//数字DO:    0
#define    RAIN_MODE        1

#if RAIN_MODE
#define     RAIN_AO_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     RAIN_AO_GPIO_PORT                               GPIOA
#define     RAIN_AO_GPIO_PIN                                GPIO_Pin_0
#define     RAIN_ADC_CHANNEL                                ADC_Channel_0    // ADC 通道宏定义

#else
#define     RAIN_DO_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     RAIN_DO_GPIO_PORT                               GPIOA
#define     RAIN_DO_GPIO_PIN                                GPIO_Pin_1            

#endif

void RAIN_Init(void);
uint16_t RAIN_GetValue(void);

#endif /* __RAIN_H */

