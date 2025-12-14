#ifndef __SW420_H
#define __SW420_H

#include "sys.h"

#define     SW420_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     SW420_GPIO_PORT                               GPIOA
#define     SW420_GPIO_PIN                                GPIO_Pin_0            

extern uint8_t sw420_run_state;

void SW420_Init(void);
uint8_t SW420_GetFlag(void);

#endif

