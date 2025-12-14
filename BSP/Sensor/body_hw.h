#ifndef __BODY_HW_H
#define __BODY_HW_H
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0, 0, OLED_8X16, "Body:%1d",BodyHw_State);
#include "sys.h"

#define     BODY_HW_GPIO_CLK                                RCC_APB2Periph_GPIOA
#define     BODY_HW_GPIO_PORT                               GPIOA
#define     BODY_HW_GPIO_PIN                                GPIO_Pin_1            

void BODY_HW_Init(void);
uint8_t BODY_HW_GetValue(void);

#endif

