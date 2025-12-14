#ifndef __GATE_GUARD_H
#define __GATE_GUARD_H
#include "sys.h"


// 门磁模块 GPIO宏定义

#define     GATE_GUARD_GPIO_CLK                             RCC_APB2Periph_GPIOB
#define     GATE_GUARD_GPIO_PORT                            GPIOB
#define     GATE_GUARD_GPIO_PIN                             GPIO_Pin_15

#define GATE_GUARD  GPIO_ReadInputDataBit(GATE_GUARD_GPIO_PORT,GATE_GUARD_GPIO_PIN)        //读取按键1


void GATE_GUARD_Init(void);
uint8_t GATE_GUARD_GetState(void);

#endif /* __ADC_H */

