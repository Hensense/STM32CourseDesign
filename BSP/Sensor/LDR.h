#ifndef __LDR_H
#define __LDR_H
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0, OLED_8X16,"Light Lux:%03d", LDR_Lux);
#include "adcx.h"
#include "sys.h"
#include <math.h>
/* 引脚宏定义 开始：可自由切换为带有ADC通道的引脚 （PA1~PA7 PB0~1）*/
#define     LDR_GPIO_CLK                            RCC_APB2Periph_GPIOA
#define     LDR_GPIO_PORT                           GPIOA
#define     LDR_GPIO_PIN                            GPIO_Pin_6
#define     LDR_ADC_CHANNEL                         ADC_Channel_6
/* 引脚宏定义结束 */

void LDR_Init(void);
uint16_t LDR_GetValue(void);

#endif

