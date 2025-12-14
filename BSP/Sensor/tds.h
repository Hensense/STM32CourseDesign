#ifndef __WATER_H
#define __WATER_H
#include "adcx.h"
#include "sys.h"

#define TDS_READ_TIMES    10  //TDS传感器ADC循环读取次数

#define     TDS_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     TDS_GPIO_PORT                               GPIOA
#define     TDS_GPIO_PIN                                GPIO_Pin_0
#define     TDS_ADC_CHANNEL                             ADC_Channel_0    // ADC 通道宏定义

void TDS_Init(void);
float TDS_GetData_PPM(void);

#endif /* __TDS_H */

