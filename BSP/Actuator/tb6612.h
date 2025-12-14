#ifndef __TB6612_H
#define __TB6612_H
/* 测试情况：本文件已完成测试，功能一切正常 修改引脚需要在.c文件中修改结构体 */
#include "sys.h"

#define TB6612_MOTOR_NUMBER     2   /* TB6612驱动电机的个数 1/2 */

typedef enum {
    MOTOR_A = 0,
    MOTOR_B
} Motor_Nmber;

typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_REVERSE
} Motor_Direction;

// PWM通道配置结构体
typedef struct {
    TIM_TypeDef* TIMx;
    uint16_t TIM_Channel;
    GPIO_TypeDef* PWM_Port;
    uint16_t PWM_Pin;
    uint32_t TIM_CLK;
    uint32_t GPIO_CLK;
    uint8_t AlternateFunc;
} PWM_Config;

// 电机完整配置
typedef struct {
    // 电机A控制引脚
    GPIO_TypeDef* AIN1_Port;
    uint16_t AIN1_Pin;
    GPIO_TypeDef* AIN2_Port;
    uint16_t AIN2_Pin;
    PWM_Config PWMA;
    
    // 电机B控制引脚
    GPIO_TypeDef* BIN1_Port;
    uint16_t BIN1_Pin;
    GPIO_TypeDef* BIN2_Port;
    uint16_t BIN2_Pin;
    PWM_Config PWMB;
} TB6612_Config;

void TB6612_Init(void);
void TB6612_SetSpeed(Motor_Nmber number, int8_t speed);
void TB6612_SetDirection(Motor_Nmber number, Motor_Direction dir);

#endif
