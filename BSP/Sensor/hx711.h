#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

// HX711 GPIO∫Í∂®“Â
#define     HX711_SCK_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define     HX711_SCK_GPIO_PORT                  GPIOA
#define     HX711_SCK_GPIO_PIN                   GPIO_Pin_0

#define     HX711_DT_GPIO_CLK                    RCC_APB2Periph_GPIOA
#define     HX711_DT_GPIO_PORT                   GPIOA
#define     HX711_DT_GPIO_PIN                    GPIO_Pin_1

#define     HX711_SCK_H                          GPIO_SetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);
#define     HX711_SCK_L                          GPIO_ResetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);
                       
#define     HX711_DT                             GPIO_ReadInputDataBit(HX711_DT_GPIO_PORT, HX711_DT_GPIO_PIN)

void HX711_Init(void);
float HX711_GetWeight(void);
uint32_t HX711_GetCount(void);

#endif /* __ADC_H */

