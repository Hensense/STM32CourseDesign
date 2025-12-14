#ifndef     __IR_H
#define     __IR_H

#include "sys.h"
#include "adcx.h"

#define     IR_DATA_MODE     0              /* 数字输入模式 */
#define     IR_ADC_MOEDE     1              /* 模拟输入模式 */
/******************************** 移植修改宏定义 开始 *********************************************/
#define     IR_MODE          IR_ADC_MOEDE   /* 确定工作模式 */

#if IR_MODE
#define     IR_AO_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     IR_AO_GPIO_PORT                               GPIOA
#define     IR_AO_GPIO_PIN                                GPIO_Pin_1
#define     IR_ADC_CHANNEL                                ADC_Channel_1    // ADC 通道宏定义

#else
#define     IR_DO_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     IR_DO_GPIO_PORT                               GPIOA
#define     IR_DO_GPIO_PIN                                GPIO_Pin_1            

#endif
/******************************** 移植修改宏定义 结束 *********************************************/

void IR_Init(void);
uint16_t IR_FireData(void);

#endif /* __ADC_H */

