#ifndef __JSQ_H
#define __JSQ_H
#include "sys.h"

#define JSQ_CLK                         RCC_APB2Periph_GPIOA
#define JSQ_GPIO_PIN                    GPIO_Pin_0
#define JSQ_GPIO_PORT                   GPIOA

#define JSQ_ON                          GPIO_SetBits(JSQ_GPIO_PORT,JSQ_GPIO_PIN)
#define JSQ_OFF                         GPIO_ResetBits(JSQ_GPIO_PORT,JSQ_GPIO_PIN)

void JSQ_Init(void);
void JSQ_On(void);
void JSQ_Off(void);
uint8_t JSQ_GetState(void);
#endif



