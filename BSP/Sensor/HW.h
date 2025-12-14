#ifndef __HW_H
#define __HW_H
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0, OLED_8X16, "HW0:%01d", HW_State[0]);
#include "sys.h"
#include "stdio.h"
/* 引脚宏定义 开始：可自由切换除已使用的任意IO引脚 */
#define     HW_SENSOR_NUMBER    1   /* 设置启用的红外传感器的数量 - 修改后记得修改引脚宏定义 1~4 */

#if HW_SENSOR_NUMBER == 1
#define     HW1_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW1_GPIO_PORT                            GPIOA
#define     HW1_GPIO_PIN                             GPIO_Pin_7
#elif HW_SENSOR_NUMBER == 2
#define     HW1_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW1_GPIO_PORT                            GPIOA
#define     HW1_GPIO_PIN                             GPIO_Pin_1
#define     HW2_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW2_GPIO_PORT                            GPIOA
#define     HW2_GPIO_PIN                             GPIO_Pin_2
#elif HW_SENSOR_NUMBER == 3
#define     HW1_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW1_GPIO_PORT                            GPIOA
#define     HW1_GPIO_PIN                             GPIO_Pin_0
#define     HW2_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW2_GPIO_PORT                            GPIOA
#define     HW2_GPIO_PIN                             GPIO_Pin_0
#define     HW3_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW3_GPIO_PORT                            GPIOA
#define     HW3_GPIO_PIN                             GPIO_Pin_0
#elif HW_SENSOR_NUMBER == 4
#define     HW1_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW1_GPIO_PORT                            GPIOA
#define     HW1_GPIO_PIN                             GPIO_Pin_0
#define     HW2_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW2_GPIO_PORT                            GPIOA
#define     HW2_GPIO_PIN                             GPIO_Pin_0
#define     HW3_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW3_GPIO_PORT                            GPIOA
#define     HW3_GPIO_PIN                             GPIO_Pin_0
#define     HW4_GPIO_CLK                             RCC_APB2Periph_GPIOA
#define     HW4_GPIO_PORT                            GPIOA
#define     HW4_GPIO_PIN                             GPIO_Pin_0
#endif
/* 引脚宏定义结束 */


void HW_Init(void);
uint8_t* HW_GetFlag(void);

#endif

