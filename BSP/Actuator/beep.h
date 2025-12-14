#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0,OLED_8X16,"Beep:%01d", BEEP_Ctrler.RunState());
#define BEEP_ON_STATE   1
#define BEEP_OFF_STATE  0
/* 引脚配置 */
#define BEEP_CLK                        RCC_APB2Periph_GPIOA
#define BEEP_GPIO_PIN                   GPIO_Pin_4
#define BEEP_GPIO_PROT                  GPIOA
/* 低电平触发 */
#define BEEP_ON                         GPIO_ResetBits(BEEP_GPIO_PROT,BEEP_GPIO_PIN)
#define BEEP_OFF                        GPIO_SetBits(BEEP_GPIO_PROT,BEEP_GPIO_PIN)
/* END */
void BEEP_Init(void);
void BEEP_On(void);
void BEEP_Off(void);
uint8_t BEEP_GetState(void);
#endif



