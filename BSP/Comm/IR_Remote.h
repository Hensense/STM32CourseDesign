#ifndef __IR_Remote_H
#define __IR_Remote_H
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0,OLED_8X16, "Address:%02X",IR_REMOTE_DataInfo.Address);
//OLED_Printf(0,16,OLED_8X16, "Command:%02X",IR_REMOTE_DataInfo.Command);
#include "sys.h"
#define IR_REMOTE_START_STATE       0   /* 0:禁用文件 1:启动文件 */
#if IR_REMOTE_START_STATE
/*************** 用户配置区 - 根据实际硬件连接修改 ***************/
#define IR_REMOTE_CLK           RCC_APB2Periph_GPIOA
#define IR_REMOTE_PORT          GPIOA
#define IR_REMOTE_PIN           GPIO_Pin_1
#define IR_REMOTE_PORT_SOURCE   GPIO_PortSourceGPIOA
#define IR_REMOTE_PIN_SOURCE    GPIO_PinSource1
#define IR_REMOTE_EXTI_LINE     EXTI_Line1
#define IR_REMOTE_EXTI_IRQn     EXTI1_IRQn
#define ID_REMOTE_IRQHandler    EXTI1_IRQHandler
// 定时器配置
#define IR_TIM                  TIM4
#define IR_TIM_RCC              RCC_APB1Periph_TIM4
/*************** END ***************/

// 函数声明
void IR_Remote_Init(void);
uint8_t IR_Remote_GetAddress(void);
uint8_t IR_Remote_GetCommand(void);
uint8_t IR_Remote_GetDataFlag(void);
uint8_t IR_Remote_GetRepeatFlag(void);

#endif

#endif
