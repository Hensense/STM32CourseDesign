#include "tb6612.h"

/*******************************在这里修改引脚定义 start ******************************************/
TB6612_Config motor_cfg = {
    // 电机A控制引脚 (PB13,PB12)
    .AIN1_Port = GPIOB, .AIN1_Pin = GPIO_Pin_13,
    .AIN2_Port = GPIOB, .AIN2_Pin = GPIO_Pin_12,
    .PWMA = {
        .TIMx = TIM1,
        .TIM_Channel = TIM_Channel_1,
        .PWM_Port = GPIOA,
        .PWM_Pin = GPIO_Pin_8,
        .TIM_CLK = RCC_APB2Periph_TIM1,
        .GPIO_CLK = RCC_APB2Periph_GPIOA,
    },
#if TB6612_MOTOR_NUMBER == 2
    // 电机B控制引脚 (PB1,PB0)
    .BIN1_Port = GPIOA, .BIN1_Pin = GPIO_Pin_4,
    .BIN2_Port = GPIOA, .BIN2_Pin = GPIO_Pin_5,
    .PWMB = {
        .TIMx = TIM2,
        .TIM_Channel = TIM_Channel_3,
        .PWM_Port = GPIOA,
        .PWM_Pin = GPIO_Pin_2,
        .TIM_CLK = RCC_APB1Periph_TIM2,
        .GPIO_CLK = RCC_APB2Periph_GPIOA,
    }
#endif
};
/*******************************在这里修改引脚定义 end ******************************************/
static void PWM_Init(PWM_Config* pwm, uint16_t arr, uint16_t psc) {
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(pwm->GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    if(pwm->TIMx == TIM1) {
        RCC_APB2PeriphClockCmd(pwm->TIM_CLK, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(pwm->TIM_CLK, ENABLE);
    }
    
    // 配置PWM引脚为复用功能
    GPIO_InitStruct.GPIO_Pin = pwm->PWM_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(pwm->PWM_Port, &GPIO_InitStruct);
    
    // 定时器基础配置
    TIM_TimeBaseStruct.TIM_Period = arr;
    TIM_TimeBaseStruct.TIM_Prescaler = psc;
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(pwm->TIMx, &TIM_TimeBaseStruct);
    
    // PWM通道配置
    if(pwm->TIMx == TIM1)
    {
        TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
    }
    else
    {
        TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    }
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse = 0;
    
    switch(pwm->TIM_Channel) {
        case TIM_Channel_1: 
            TIM_OC1Init(pwm->TIMx, &TIM_OCInitStruct);
            TIM_OC1PreloadConfig(pwm->TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_2:
            TIM_OC2Init(pwm->TIMx, &TIM_OCInitStruct);
            TIM_OC2PreloadConfig(pwm->TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_3:
            TIM_OC3Init(pwm->TIMx, &TIM_OCInitStruct);
            TIM_OC3PreloadConfig(pwm->TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_4:
            TIM_OC4Init(pwm->TIMx, &TIM_OCInitStruct);
            TIM_OC4PreloadConfig(pwm->TIMx, TIM_OCPreload_Enable);
            break;
    }
    
    // 高级定时器特殊配置
    if(pwm->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(TIM1, ENABLE);
    }
    
    TIM_Cmd(pwm->TIMx, ENABLE);
}

void TB6612_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 初始化方向控制引脚
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 电机A方向引脚
    GPIO_InitStruct.GPIO_Pin = motor_cfg.AIN1_Pin;
    GPIO_Init(motor_cfg.AIN1_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = motor_cfg.AIN2_Pin;
    GPIO_Init(motor_cfg.AIN2_Port, &GPIO_InitStruct);
    
    GPIO_ResetBits(motor_cfg.AIN1_Port, motor_cfg.AIN1_Pin);    //电机停止旋转
    GPIO_ResetBits(motor_cfg.AIN2_Port, motor_cfg.AIN2_Pin);
    // 初始化PWM
    PWM_Init(&motor_cfg.PWMA, 100-1, 72-1);

#if TB6612_MOTOR_NUMBER == 2
    // 电机B方向引脚
    GPIO_InitStruct.GPIO_Pin = motor_cfg.BIN1_Pin;
    GPIO_Init(motor_cfg.BIN1_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = motor_cfg.BIN2_Pin;
    GPIO_Init(motor_cfg.BIN2_Port, &GPIO_InitStruct);
    
    GPIO_ResetBits(motor_cfg.BIN1_Port, motor_cfg.BIN1_Pin);    //电机停止旋转
    GPIO_ResetBits(motor_cfg.BIN2_Port, motor_cfg.BIN2_Pin);
    // 初始化PWM（20kHz频率，72MHz/(360 * 2000)）
    PWM_Init(&motor_cfg.PWMB, 100-1, 72-1);
#endif
}
/**
 * @brief   设置指定电机的旋转速度
 * @param   number：电机编号 MOTOR_A | MOTOR_B
 * @param   speed：电机旋转速度设置 -100~100
 *  @note       speed == 0  停止
 *              speed > 0   顺时针
 *              speed < 0   逆时针
 * @retval  
 */
void TB6612_SetSpeed(Motor_Nmber number, int8_t speed) {
    switch(number) {
        case MOTOR_A:
            if(speed == 0)
            {
                TB6612_SetDirection(MOTOR_A, MOTOR_STOP);
            }
            else if(speed > 0)
            {
                TB6612_SetDirection(MOTOR_A, MOTOR_FORWARD);
            }
            else if(speed < 0)
            {
                TB6612_SetDirection(MOTOR_A, MOTOR_REVERSE);
            }
            switch(motor_cfg.PWMA.TIM_Channel) {
                case TIM_Channel_1: TIM_SetCompare1(motor_cfg.PWMA.TIMx, speed); break;
                case TIM_Channel_2: TIM_SetCompare2(motor_cfg.PWMA.TIMx, speed); break;
                case TIM_Channel_3: TIM_SetCompare3(motor_cfg.PWMA.TIMx, speed); break;
                case TIM_Channel_4: TIM_SetCompare4(motor_cfg.PWMA.TIMx, speed); break;
            }
            break;
#if TB6612_MOTOR_NUMBER == 2
        case MOTOR_B:
            if(speed == 0)
            {
                TB6612_SetDirection(MOTOR_B, MOTOR_STOP);
            }
            else if(speed > 0)
            {
                TB6612_SetDirection(MOTOR_B, MOTOR_FORWARD);
            }
            else if(speed < 0)
            {
                TB6612_SetDirection(MOTOR_B, MOTOR_REVERSE);
            }
            switch(motor_cfg.PWMB.TIM_Channel) {
                case TIM_Channel_1: TIM_SetCompare1(motor_cfg.PWMB.TIMx, speed); break;
                case TIM_Channel_2: TIM_SetCompare2(motor_cfg.PWMB.TIMx, speed); break;
                case TIM_Channel_3: TIM_SetCompare3(motor_cfg.PWMB.TIMx, speed); break;
                case TIM_Channel_4: TIM_SetCompare4(motor_cfg.PWMB.TIMx, speed); break;
            }
            break;
#endif
        default:{
            
        }
    }
}

void TB6612_SetDirection(Motor_Nmber number, Motor_Direction dir) {
    switch(number) {
        case MOTOR_A:
            switch(dir) {
                case MOTOR_STOP:
                    GPIO_ResetBits(motor_cfg.AIN1_Port, motor_cfg.AIN1_Pin);
                    GPIO_ResetBits(motor_cfg.AIN2_Port, motor_cfg.AIN2_Pin);
                    break;
                case MOTOR_FORWARD:
                    GPIO_SetBits(motor_cfg.AIN1_Port, motor_cfg.AIN1_Pin);
                    GPIO_ResetBits(motor_cfg.AIN2_Port, motor_cfg.AIN2_Pin);
                    break;
                case MOTOR_REVERSE:
                    GPIO_ResetBits(motor_cfg.AIN1_Port, motor_cfg.AIN1_Pin);
                    GPIO_SetBits(motor_cfg.AIN2_Port, motor_cfg.AIN2_Pin);
                    break;
            }
            break;
#if TB6612_MOTOR_NUMBER == 2
        case MOTOR_B:
            switch(dir) {
                case MOTOR_STOP:
                    GPIO_ResetBits(motor_cfg.BIN1_Port, motor_cfg.BIN1_Pin);
                    GPIO_ResetBits(motor_cfg.BIN2_Port, motor_cfg.BIN2_Pin);
                    break;
                case MOTOR_FORWARD:
                    GPIO_SetBits(motor_cfg.BIN1_Port, motor_cfg.BIN1_Pin);
                    GPIO_ResetBits(motor_cfg.BIN2_Port, motor_cfg.BIN2_Pin);
                    break;
                case MOTOR_REVERSE:
                    GPIO_ResetBits(motor_cfg.BIN1_Port, motor_cfg.BIN1_Pin);
                    GPIO_SetBits(motor_cfg.BIN2_Port, motor_cfg.BIN2_Pin);
                    break;
            }
            break;
#endif
        default:{

        }
    }
}
