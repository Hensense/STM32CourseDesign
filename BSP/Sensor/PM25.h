#ifndef __PM25_H
#define __PM25_H
#include "adcx.h"
#include "sys.h"
#include "math.h"
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0, 0, OLED_8X16, "PM2.5:%03d ug/m3",PM25_Value);
/***************根据自己需求更改****************/
#define     PM25_LED_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define     PM25_LED_GPIO_PORT                      GPIOA
#define     PM25_LED_GPIO_PIN                       GPIO_Pin_0
/*********************END**********************/
/* 如正常无法获取数据，请将LED极性反向 */
#define     PM25_LED_H                              GPIO_ResetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);
#define     PM25_LED_L                              GPIO_SetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);

//#define     PM25_LED_H                              GPIO_SetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);
//#define     PM25_LED_L                              GPIO_ResetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);

// ADC 通道宏定义
#define     PM25_VO_GPIO_CLK                        RCC_APB2Periph_GPIOA
#define     PM25_VO_GPIO_PORT                       GPIOA
#define     PM25_VO_GPIO_PIN                        GPIO_Pin_2
#define     PM25_ADC_CHANNEL                        ADC_Channel_2

void PM25_Init(void);
uint16_t PM25_GetValue(void);

#endif

