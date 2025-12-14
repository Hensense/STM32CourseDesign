#ifndef __SERVO_H
#define __SERVO_H
/* TIM1_CH1/2/3/4无问题 */
/* TIM3_CH1/2/3/4无问题 重映射1/2无问题 */
/* TIM2_CH1/2/3/4无问题 重映射1/2/3/4无问题 */
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0,OLED_8X16, "Servo_Angle:%.0f",SERVO_Ctrler.Servo_Angle);
#include "sys.h"
/* 用户配置宏定义 开始：可自由切换除已使用的任意IO引脚 */
// 选择定时器 (1=TIM1, 2=TIM2, 3=TIM3)
#define SERVO_PWM_TIMER_SELECT          3
// 选择通道 (1-4)
#define SERVO_PWM_CHANNEL_SELECT        3
// 重映射配置 (0=禁用, 1=启用) TIM1只能选0
#define SERVO_PWM_REMAP_ENABLE          0
/* 用户配置宏定义 结束 */

/* 自动配置区 (勿修改) ------------------------------------------*/
// 定时器映射
#if (SERVO_PWM_TIMER_SELECT == 1)
    #define SERVO_PWM_TIM               TIM1
    #define SERVO_PWM_TIM_CLK           RCC_APB2Periph_TIM1
    #define SERVO_PWM_TIM_APB_CLK       RCC_APB2PeriphClockCmd
    #define SERVO_IS_ADVANCED_TIM       1     /* 高级定时器 */
#elif (SERVO_PWM_TIMER_SELECT == 2)
    #define SERVO_PWM_TIM               TIM2
    #define SERVO_PWM_TIM_CLK           RCC_APB1Periph_TIM2
    #define SERVO_PWM_TIM_APB_CLK       RCC_APB1PeriphClockCmd
    #define SERVO_IS_ADVANCED_TIM       0
#elif (SERVO_PWM_TIMER_SELECT == 3)
    #define SERVO_PWM_TIM               TIM3
    #define SERVO_PWM_TIM_CLK           RCC_APB1Periph_TIM3
    #define SERVO_PWM_TIM_APB_CLK       RCC_APB1PeriphClockCmd
    #define SERVO_IS_ADVANCED_TIM       0
#endif

// 引脚配置
typedef struct {
    uint16_t pin;
    GPIO_TypeDef* port;
    uint32_t clk;
} ServoPinConfig;

#if (SERVO_PWM_TIMER_SELECT == 1) // TIM1
    #if (SERVO_PWM_REMAP_ENABLE)
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            /* C8T6的TIM1无重定义引脚，有互补输出重定义引脚 */
        };
    #else
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            {GPIO_Pin_8,  GPIOA, RCC_APB2Periph_GPIOA},  // CH1
            {GPIO_Pin_9,  GPIOA, RCC_APB2Periph_GPIOA},  // CH2
            {GPIO_Pin_10, GPIOA, RCC_APB2Periph_GPIOA},  // CH3
            {GPIO_Pin_11, GPIOA, RCC_APB2Periph_GPIOA}   // CH4
        };
    #endif
#elif (SERVO_PWM_TIMER_SELECT == 2) // TIM2
    #if (SERVO_PWM_REMAP_ENABLE)
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            {GPIO_Pin_15, GPIOA, RCC_APB2Periph_GPIOA},  // CH1
            {GPIO_Pin_3,  GPIOB, RCC_APB2Periph_GPIOB},  // CH2
            {GPIO_Pin_10, GPIOB, RCC_APB2Periph_GPIOB},  // CH3
            {GPIO_Pin_11, GPIOB, RCC_APB2Periph_GPIOB}   // CH4
        };
    #else
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            {GPIO_Pin_0, GPIOA, RCC_APB2Periph_GPIOA},    // CH1
            {GPIO_Pin_1, GPIOA, RCC_APB2Periph_GPIOA},    // CH2
            {GPIO_Pin_2, GPIOA, RCC_APB2Periph_GPIOA},    // CH3
            {GPIO_Pin_3, GPIOA, RCC_APB2Periph_GPIOA}     // CH4
        };
    #endif
#elif (SERVO_PWM_TIMER_SELECT == 3) // TIM3
    #if (SERVO_PWM_REMAP_ENABLE)
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            {GPIO_Pin_4, GPIOB, RCC_APB2Periph_GPIOB},    // CH1
            {GPIO_Pin_5, GPIOB, RCC_APB2Periph_GPIOB},    // CH2
            /* CH3/4无重定义引脚 */
        };
    #else
        static const ServoPinConfig SERVO_PWM_PIN_MAP[] = {
            {GPIO_Pin_6, GPIOA, RCC_APB2Periph_GPIOA},    // CH1
            {GPIO_Pin_7, GPIOA, RCC_APB2Periph_GPIOA},    // CH2
            {GPIO_Pin_0, GPIOB, RCC_APB2Periph_GPIOB},    // CH3
            {GPIO_Pin_1, GPIOB, RCC_APB2Periph_GPIOB}     // CH4
        };
    #endif
#endif

// 当前通道配置
#define SERVO_PWM_PIN_CFG         SERVO_PWM_PIN_MAP[SERVO_PWM_CHANNEL_SELECT-1]

// 函数指针映射
typedef void (*SERVO_TIM_OC_InitType)(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*SERVO_TIM_OC_PreloadType)(TIM_TypeDef*, uint16_t);
typedef void (*SERVO_TIM_SetCompareType)(TIM_TypeDef*, uint16_t);

static const SERVO_TIM_OC_InitType SERVO_OC_Init[] = {
    TIM_OC1Init, TIM_OC2Init, TIM_OC3Init, TIM_OC4Init
};

static const SERVO_TIM_OC_PreloadType SERVO_OC_Preload[] = {
    TIM_OC1PreloadConfig, TIM_OC2PreloadConfig, 
    TIM_OC3PreloadConfig, TIM_OC4PreloadConfig
};

static const SERVO_TIM_SetCompareType SERVO_SetCompare[] = {
    TIM_SetCompare1, TIM_SetCompare2, 
    TIM_SetCompare3, TIM_SetCompare4
};


void Servo_Init(void);
void Servo_SetAngle(float Angle);

#endif



