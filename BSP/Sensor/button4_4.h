#ifndef _BUTTON_H 
#define _BUTTON_H
#include "sys.h"
/* 本文件已测试完成，功能一切正常 */
#define     BUTTON_GPIO_CLK                                     RCC_APB2Periph_GPIOA
#define     BUTTON_ROW1_GPIO_PORT                               GPIOA
#define     BUTTON_ROW1_GPIO_PIN                                GPIO_Pin_1
#define     BUTTON_ROW2_GPIO_PORT                               GPIOA
#define     BUTTON_ROW2_GPIO_PIN                                GPIO_Pin_2
#define     BUTTON_ROW3_GPIO_PORT                               GPIOA
#define     BUTTON_ROW3_GPIO_PIN                                GPIO_Pin_3
#define     BUTTON_ROW4_GPIO_PORT                               GPIOA
#define     BUTTON_ROW4_GPIO_PIN                                GPIO_Pin_4

#define     BUTTON_COL1_GPIO_PORT                               GPIOA
#define     BUTTON_COL1_GPIO_PIN                                GPIO_Pin_5
#define     BUTTON_COL2_GPIO_PORT                               GPIOA
#define     BUTTON_COL2_GPIO_PIN                                GPIO_Pin_6
#define     BUTTON_COL3_GPIO_PORT                               GPIOA
#define     BUTTON_COL3_GPIO_PIN                                GPIO_Pin_7
#define     BUTTON_COL4_GPIO_PORT                               GPIOB
#define     BUTTON_COL4_GPIO_PIN                                GPIO_Pin_0

void Button4_4_Init(void);
char Button4_4_Scan(void);
 
#endif



