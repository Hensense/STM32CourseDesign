#ifndef __BUMP_H
#define __BUMP_H

#include "sys.h"

#define BUMP_CLK                        RCC_APB2Periph_GPIOA
#define BUMP_GPIO_PIN                   GPIO_Pin_0
#define BUMP_GPIO_PROT                  GPIOA

#define BUMP_ON                         GPIO_SetBits(BUMP_GPIO_PROT,BUMP_GPIO_PIN)
#define BUMP_OFF                        GPIO_ResetBits(BUMP_GPIO_PROT,BUMP_GPIO_PIN)

void BUMP_Init(void);
void BUMP_On(void);
void BUMP_Off(void);
uint8_t BUMP_GetState(void);
#endif



