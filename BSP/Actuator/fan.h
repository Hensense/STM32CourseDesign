#ifndef __FAN_H
#define __FAN_H
#include "sys.h"

#define FAN_CLK                     RCC_APB2Periph_GPIOA
#define FAN_GPIO_PIN                GPIO_Pin_0
#define FAN_GPIO_PROT               GPIOA

#define FAN_ON                      GPIO_SetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)
#define FAN_OFF                     GPIO_ResetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)

void FAN_Init(void);
void FAN_On(void);
void FAN_Off(void);
uint8_t FAN_GetState(void);
#endif



