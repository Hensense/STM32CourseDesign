#ifndef __PTC_H
#define __PTC_H
#include "sys.h"

#define PTC_CLK                     RCC_APB2Periph_GPIOA
#define PTC_GPIO_PIN                GPIO_Pin_0
#define PTC_GPIO_PORT               GPIOA

#define PTC_ON                      GPIO_SetBits(PTC_GPIO_PORT,PTC_GPIO_PIN)
#define PTC_OFF                     GPIO_ResetBits(PTC_GPIO_PORT,PTC_GPIO_PIN)

void PTC_Init(void);
void PTC_On(void);
void PTC_Off(void);
uint8_t PTC_GetState(void);
#endif



