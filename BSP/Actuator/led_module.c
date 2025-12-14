#include "led_module.h"
__IO uint8_t LED_MOUDLE_State = 0;
void LED_MODULE_Init(void)
{
#if LED_MODE == LED_MODE_DATA
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_MODULE_CLK, ENABLE ); //配置时钟
    
    GPIO_InitStructure.GPIO_Pin = LED_MODULE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_MODULE_GPIO_PORT,&GPIO_InitStructure);

    LED_MODULE_OFF;
#elif LED_MODE == LED_MODE_PWM
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    /* 1. 使能时钟 */
    PWM_TIM_APB_CLK(PWM_TIM_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | PWM_PIN_CFG.clk, ENABLE);
    
    /* 2. 配置重映射 */
    #if (PWM_TIMER_SELECT == 1)
        if(PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    #elif (PWM_TIMER_SELECT == 2)
        if(PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    #elif (PWM_TIMER_SELECT == 3)
        if(PWM_REMAP_ENABLE) GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    #endif
    
    /* 3. 配置GPIO */
    GPIO_InitStruct.GPIO_Pin = PWM_PIN_CFG.pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM_PIN_CFG.port, &GPIO_InitStruct);
    
    /* 4. 定时器时基配置 */
    TIM_TimeBaseStruct.TIM_Period = 100 - 1;
    TIM_TimeBaseStruct.TIM_Prescaler = 0;
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStruct);
    
    /* 5. PWM通道配置 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse = 0;
    OC_Init[PWM_CHANNEL_SELECT-1](PWM_TIM, &TIM_OCInitStruct);
    
    /* 6. 使能预装载 */
    OC_Preload[PWM_CHANNEL_SELECT-1](PWM_TIM, TIM_OCPreload_Enable);
    
    /* 7. 启动定时器 */
    TIM_Cmd(PWM_TIM, ENABLE);
    
    /* 8. 高级定时器需要额外配置 */
    #if (IS_ADVANCED_TIM == 1)
        TIM_CtrlPWMOutputs(PWM_TIM, ENABLE);
    #endif
#endif
}
#if LED_MODE == LED_MODE_PWM
void LED_MODULE_SetDuty(uint8_t duty)
{
    if(duty > 100) duty = 100;
    SetCompare[PWM_CHANNEL_SELECT-1](PWM_TIM, duty);
}
#else

void LED_MOUDLE_On(void)
{
    LED_MODULE_ON;
    LED_MOUDLE_State = 1;
}
void LED_MOUDLE_Off(void)
{
    LED_MODULE_OFF;
    LED_MOUDLE_State = 0;
}
uint8_t LED_MOUDLE_GetState(void)
{
    return LED_MOUDLE_State;
}
#endif
