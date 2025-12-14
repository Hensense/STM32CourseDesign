#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H
#define STEPMOTOR_START_STATE   0   /* 0:禁用文件 1:启用文件 */
#if STEPMOTOR_START_STATE
#include "sys.h"

// 电机 GPIO 配置（引脚1）
#define MOTOR_GPIO1_RCC         RCC_APB2Periph_GPIOA
#define MOTOR_GPIO1_PORT        GPIOA
#define MOTOR_GPIO1_PIN         GPIO_Pin_1

// 电机 GPIO 配置（引脚2）
#define MOTOR_GPIO2_RCC         RCC_APB2Periph_GPIOA
#define MOTOR_GPIO2_PORT        GPIOA
#define MOTOR_GPIO2_PIN         GPIO_Pin_2

// 电机 GPIO 配置（引脚3）
#define MOTOR_GPIO3_RCC         RCC_APB2Periph_GPIOA
#define MOTOR_GPIO3_PORT        GPIOA
#define MOTOR_GPIO3_PIN         GPIO_Pin_3

// 电机 GPIO 配置（引脚4）
#define MOTOR_GPIO4_RCC         RCC_APB2Periph_GPIOA
#define MOTOR_GPIO4_PORT        GPIOA
#define MOTOR_GPIO4_PIN         GPIO_Pin_4


#define STEPMOTOR_TIMx    4  // 可选：TIM1, TIM2, TIM3, TIM4

// 根据宏定义自动选择对应的TIM和GPIO
#if STEPMOTOR_TIMx == 1
    #define STEPMOTOR_TIM               TIM1
    #define STEPMOTOR_TIM_IRQn          TIM1_UP_IRQn
    #define STEPMOTOR_TIM_IRQHandler    TIM1_UP_IRQHandler
    #define STEPMOTOR_TIM_RCC           RCC_APB2Periph_TIM1
#elif STEPMOTOR_TIMx == 2
    #define STEPMOTOR_TIM               TIM2
    #define STEPMOTOR_TIM_IRQn          TIM2_IRQn
    #define STEPMOTOR_TIM_IRQHandler    TIM2_IRQHandler
    #define STEPMOTOR_TIM_RCC           RCC_APB1Periph_TIM2
#elif STEPMOTOR_TIMx == 3
    #define STEPMOTOR_TIM               TIM3
    #define STEPMOTOR_TIM_IRQn          TIM3_IRQn
    #define STEPMOTOR_TIM_IRQHandler    TIM3_IRQHandler
    #define STEPMOTOR_TIM_RCC           RCC_APB1Periph_TIM3
#elif STEPMOTOR_TIMx == 4
    #define STEPMOTOR_TIM               TIM4
    #define STEPMOTOR_TIM_IRQn          TIM4_IRQn
    #define STEPMOTOR_TIM_IRQHandler    TIM4_IRQHandler
    #define STEPMOTOR_TIM_RCC           RCC_APB1Periph_TIM4
#else
    #error "Unsupported TIM port! Please choose TIM1/2/3/4."
#endif

// 旋转方向
typedef enum {
    CLOCKWISE = 0,          /* 顺时针 */
    ANTICLOCKWISE = 1       /* 逆时针 */
} MotorDirection;

// 速度范围 (us)
#define MIN_SPEED 2000   // 2000us = 2ms
#define MAX_SPEED 5000   // 5000us = 5ms

// 函数声明
void StepperMotor_Init(void);
void StepperMotor_SetSpeed(uint16_t speed_us); // 参数改为微秒
void StepperMotor_SetDirection(MotorDirection direction);
void StepperMotor_Step(uint16_t speed, int16_t steps);
void StepperMotor_RotateContinuous(void);
void StepperMotor_Stop(void);
uint8_t StepperMotor_GetState(void);
void StepperMotor_Start(MotorDirection direction, uint16_t speed_us);
#endif

#endif /* __STEPPER_MOTOR_H */
