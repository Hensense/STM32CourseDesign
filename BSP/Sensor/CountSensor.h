#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H
#include "stm32f10x.h"
/* 引脚宏定义 可自由切换引脚 */
#define         COUNT_SENSOR_INT_PIN               GPIO_Pin_1
#define         COUNT_SENSOR_INT_GPIO_PORT         GPIOA
#define         COUNT_SENSOR_INT_EXTI_IRQn         EXTI1_IRQn
#define         COUNT_SENSOR_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)

#define         COUNT_SENSOR_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define         COUNT_SENSOR_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define         COUNT_SENSOR_INT_EXTI_LINE         EXTI_Line1
#define         COUNT_SENSOR_INT_IRQHandler        EXTI1_IRQHandler

void CountSensor_Init(void);
void CountSensor_Clear(void);
uint16_t CountSensor_Get(void);

#endif
