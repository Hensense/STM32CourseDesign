#include "servo.h"
/* SG90舵机 */
void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    /* 1. 使能时钟 */
    SERVO_PWM_TIM_APB_CLK(SERVO_PWM_TIM_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | SERVO_PWM_PIN_CFG.clk, ENABLE);
    
    /* 2. 配置重映射 */
    #if (SERVO_PWM_TIMER_SELECT == 1)
        if(SERVO_PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    #elif (SERVO_PWM_TIMER_SELECT == 2)
        if(SERVO_PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    #elif (SERVO_PWM_TIMER_SELECT == 3)
        if(SERVO_PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    #endif
    
    /* 3. 配置GPIO */
    GPIO_InitStruct.GPIO_Pin = SERVO_PWM_PIN_CFG.pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PWM_PIN_CFG.port, &GPIO_InitStruct);
    
    /* 4. 定时器时基配置 */
    TIM_TimeBaseStruct.TIM_Period = 20000 - 1;
    TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(SERVO_PWM_TIM, &TIM_TimeBaseStruct);
    
    /* 5. PWM通道配置 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse = 0;
    SERVO_OC_Init[SERVO_PWM_CHANNEL_SELECT-1](SERVO_PWM_TIM, &TIM_OCInitStruct);
    
    /* 6. 使能预装载 */
    SERVO_OC_Preload[SERVO_PWM_CHANNEL_SELECT-1](SERVO_PWM_TIM, TIM_OCPreload_Enable);
    
    /* 7. 启动定时器 */
    TIM_Cmd(SERVO_PWM_TIM, ENABLE);
    
    /* 8. 高级定时器需要额外配置 */
    #if (SERVO_IS_ADVANCED_TIM == 1)
        TIM_CtrlPWMOutputs(SERVO_PWM_TIM, ENABLE);
    #endif
}

/* 0~180 */
void Servo_SetAngle(float Angle)
{
    /* 设置占空比 将角度线性变换，对应到舵机要求的占空比范围上 */
    SERVO_SetCompare[SERVO_PWM_CHANNEL_SELECT-1](SERVO_PWM_TIM, Angle / 180 * 2000 + 500);
}


