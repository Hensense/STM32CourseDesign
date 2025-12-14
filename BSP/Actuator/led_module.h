#ifndef __LED_MODULE_H
#define __LED_MODULE_H
#include "sys.h"
#include "stm32f10x_tim.h"
#define LED_MODE_PWM        1               /* 脉宽调制模式 可使用PWM调光，需要使用带有TIM输出通道的引脚 */
#define LED_MODE_DATA       2               /* 高低电平控制模式 仅可控制亮灭 可使用任意引脚 */
#define LED_MODE            LED_MODE_DATA

#if LED_MODE == LED_MODE_DATA
#define LED_MODULE_CLK                      RCC_APB2Periph_GPIOA
#define LED_MODULE_GPIO_PIN                 GPIO_Pin_0
#define LED_MODULE_GPIO_PORT                GPIOA

#define LED_MODULE_ON                       GPIO_SetBits(LED_MODULE_GPIO_PORT,LED_MODULE_GPIO_PIN)
#define LED_MODULE_OFF                      GPIO_ResetBits(LED_MODULE_GPIO_PORT,LED_MODULE_GPIO_PIN)

void LED_MODULE_Init(void);
void LED_MOUDLE_On(void);
void LED_MOUDLE_Off(void);
uint8_t LED_MOUDLE_GetState(void);
#elif LED_MODE == LED_MODE_PWM
/* 用户配置区 ---------------------------------------------------*/
// 选择定时器 (1=TIM1, 2=TIM2, 3=TIM3)
#define PWM_TIMER_SELECT          3

// 选择通道 (1-4)
#define PWM_CHANNEL_SELECT        2

// 重映射配置 (0=禁用, 1=启用)
#define PWM_REMAP_ENABLE          1

/* 自动配置区 (勿修改) ------------------------------------------*/
// 定时器映射
#if (PWM_TIMER_SELECT == 1)
    #define PWM_TIM               TIM1
    #define PWM_TIM_CLK           RCC_APB2Periph_TIM1
    #define PWM_TIM_APB_CLK       RCC_APB2PeriphClockCmd
    #define IS_ADVANCED_TIM       1     /* 高级定时器 */
#elif (PWM_TIMER_SELECT == 2)
    #define PWM_TIM               TIM2
    #define PWM_TIM_CLK           RCC_APB1Periph_TIM2
    #define PWM_TIM_APB_CLK       RCC_APB1PeriphClockCmd
    #define IS_ADVANCED_TIM       0
#elif (PWM_TIMER_SELECT == 3)
    #define PWM_TIM               TIM3
    #define PWM_TIM_CLK           RCC_APB1Periph_TIM3
    #define PWM_TIM_APB_CLK       RCC_APB1PeriphClockCmd
    #define IS_ADVANCED_TIM       0
#endif

// 引脚配置
typedef struct {
    uint16_t pin;
    GPIO_TypeDef* port;
    uint32_t clk;
} PinConfig;

#if (PWM_TIMER_SELECT == 1) // TIM1
    #if (PWM_REMAP_ENABLE)
        static const PinConfig PWM_PIN_MAP[] = {
            /* C8T6的TIM1无重定义引脚，有互补输出重定义引脚 */
        };
    #else
        static const PinConfig PWM_PIN_MAP[] = {
            {GPIO_Pin_8,  GPIOA, RCC_APB2Periph_GPIOA},  // CH1
            {GPIO_Pin_9,  GPIOA, RCC_APB2Periph_GPIOA},  // CH2
            {GPIO_Pin_10, GPIOA, RCC_APB2Periph_GPIOA},  // CH3
            {GPIO_Pin_11, GPIOA, RCC_APB2Periph_GPIOA}   // CH4
        };
    #endif
#elif (PWM_TIMER_SELECT == 2) // TIM2
    #if (PWM_REMAP_ENABLE)
        static const PinConfig PWM_PIN_MAP[] = {
            {GPIO_Pin_15, GPIOA, RCC_APB2Periph_GPIOA},  // CH1
            {GPIO_Pin_3,  GPIOB, RCC_APB2Periph_GPIOB},  // CH2
            {GPIO_Pin_10, GPIOB, RCC_APB2Periph_GPIOB},  // CH3
            {GPIO_Pin_11, GPIOB, RCC_APB2Periph_GPIOB}   // CH4
        };
    #else
        static const PinConfig PWM_PIN_MAP[] = {
            {GPIO_Pin_0, GPIOA, RCC_APB2Periph_GPIOA},    // CH1
            {GPIO_Pin_1, GPIOA, RCC_APB2Periph_GPIOA},    // CH2
            {GPIO_Pin_2, GPIOA, RCC_APB2Periph_GPIOA},    // CH3
            {GPIO_Pin_3, GPIOA, RCC_APB2Periph_GPIOA}     // CH4
        };
    #endif
#elif (PWM_TIMER_SELECT == 3) // TIM3
    #if (PWM_REMAP_ENABLE)
        static const PinConfig PWM_PIN_MAP[] = {
            {GPIO_Pin_4, GPIOB, RCC_APB2Periph_GPIOB},    // CH1
            {GPIO_Pin_5, GPIOB, RCC_APB2Periph_GPIOB},    // CH2
            /* CH3/4无重定义引脚 */
        };
    #else
        static const PinConfig PWM_PIN_MAP[] = {
            {GPIO_Pin_6, GPIOA, RCC_APB2Periph_GPIOA},    // CH1
            {GPIO_Pin_7, GPIOA, RCC_APB2Periph_GPIOA},    // CH2
            {GPIO_Pin_0, GPIOB, RCC_APB2Periph_GPIOB},    // CH3
            {GPIO_Pin_1, GPIOB, RCC_APB2Periph_GPIOB}     // CH4
        };
    #endif
#endif

// 当前通道配置
#define PWM_PIN_CFG         PWM_PIN_MAP[PWM_CHANNEL_SELECT-1]

// 函数指针映射
typedef void (*TIM_OC_InitType)(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*TIM_OC_PreloadType)(TIM_TypeDef*, uint16_t);
typedef void (*TIM_SetCompareType)(TIM_TypeDef*, uint16_t);

static const TIM_OC_InitType OC_Init[] = {
    TIM_OC1Init, TIM_OC2Init, TIM_OC3Init, TIM_OC4Init
};

static const TIM_OC_PreloadType OC_Preload[] = {
    TIM_OC1PreloadConfig, TIM_OC2PreloadConfig, 
    TIM_OC3PreloadConfig, TIM_OC4PreloadConfig
};

static const TIM_SetCompareType SetCompare[] = {
    TIM_SetCompare1, TIM_SetCompare2, 
    TIM_SetCompare3, TIM_SetCompare4
};

void LED_MODULE_Init(void);
void LED_MODULE_SetDuty(uint8_t duty);
#endif


#endif



