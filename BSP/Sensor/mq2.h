#ifndef  __MQ2_H
#define  __MQ2_H
#include "stm32f10x.h"
#include "adcx.h"
#include "sys.h"
#include <math.h>
/* 本文件功能测试完成 功能一切正常 */
//OLED_Printf(0,0,OLED_8X16, "MQ2:%.1lfppm",MQ2_Value);
/***************根据自己需求更改****************/
#define    MQ2_MODE     1   /* 0:数字输入 1:模拟输入 */
#if MQ2_MODE
#define     MQ2_AO_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define     MQ2_AO_GPIO_PORT                      GPIOA
#define     MQ2_AO_GPIO_PIN                       GPIO_Pin_1
#define     MQ2_ADC_CHANNEL                       ADC_Channel_1    // ADC 通道宏定义

#else
#define     MQ2_DO_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define     MQ2_DO_GPIO_PORT                      GPIOA
#define     MQ2_DO_GPIO_PIN                       GPIO_Pin_1            

#endif
/*********************END**********************/


void MQ2_Init(void);
float MQ2_GetData_PPM(void);

#endif

