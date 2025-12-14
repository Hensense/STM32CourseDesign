#ifndef __TEC_H
#define __TEC_H
#include "sys.h"

#define TEC_CLK                     RCC_APB2Periph_GPIOA
#define TEC_GPIO_PIN                GPIO_Pin_0
#define TEC_GPIO_PORT               GPIOA

#define TEC_ON                      GPIO_SetBits(TEC_GPIO_PORT,TEC_GPIO_PIN)
#define TEC_OFF                     GPIO_ResetBits(TEC_GPIO_PORT,TEC_GPIO_PIN)

void TEC_Init(void);
void TEC_On(void);
void TEC_Off(void);
uint8_t TEC_GetState(void);
#endif



