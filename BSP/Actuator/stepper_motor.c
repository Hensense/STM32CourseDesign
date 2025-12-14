#include "stepper_motor.h"
#if STEPMOTOR_START_STATE
// 内部变量
static uint16_t motorSpeed = 1000; // 默认速度 1000us = 1ms
static MotorDirection motorDirection = CLOCKWISE;
static uint8_t currentStep = 0;
static volatile uint8_t isRunning = 0; // 加volatile保证中断访问安全
static uint16_t stepsRemaining = 0;    // 用于步数控制

// 步进序列 (全步进)
static const uint8_t fullStepSequence[4] = {
    0x01,  // IN1 (0001)
    0x02,  // IN2 (0010)
    0x04,  // IN3 (0100)
    0x08   // IN4 (1000)
};

// 初始化GPIO和定时器
void StepperMotor_Init(void) {
    // 1. 初始化 GPIO（每个引脚单独配置）
    RCC_APB2PeriphClockCmd(MOTOR_GPIO1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_GPIO2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_GPIO3_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_GPIO4_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // 初始化 PIN1
    GPIO_InitStruct.GPIO_Pin = MOTOR_GPIO1_PIN;
    GPIO_Init(MOTOR_GPIO1_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(MOTOR_GPIO1_PORT, MOTOR_GPIO1_PIN);

    // 初始化 PIN2
    GPIO_InitStruct.GPIO_Pin = MOTOR_GPIO2_PIN;
    GPIO_Init(MOTOR_GPIO2_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(MOTOR_GPIO2_PORT, MOTOR_GPIO2_PIN);

    // 初始化 PIN3
    GPIO_InitStruct.GPIO_Pin = MOTOR_GPIO3_PIN;
    GPIO_Init(MOTOR_GPIO3_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(MOTOR_GPIO3_PORT, MOTOR_GPIO3_PIN);

    // 初始化 PIN4
    GPIO_InitStruct.GPIO_Pin = MOTOR_GPIO4_PIN;
    GPIO_Init(MOTOR_GPIO4_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(MOTOR_GPIO4_PORT, MOTOR_GPIO4_PIN);

    // 2. 初始化TIM定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 根据宏定义选择时钟使能
    #if STEPMOTOR_TIMx == 1
        RCC_APB2PeriphClockCmd(STEPMOTOR_TIM_RCC, ENABLE);
    #elif STEPMOTOR_TIMx == 2 || STEPMOTOR_TIMx == 3 || STEPMOTOR_TIMx == 4
        RCC_APB1PeriphClockCmd(STEPMOTOR_TIM_RCC, ENABLE);
    #endif
    
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;          // 72MHz/72 = 1MHz (1us分辨率)
    TIM_TimeBaseStructure.TIM_Period = motorSpeed;         // 初始周期
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(STEPMOTOR_TIM, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(STEPMOTOR_TIM, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = STEPMOTOR_TIM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

// TIM中断服务函数
void STEPMOTOR_TIM_IRQHandler(void) {
    if(TIM_GetITStatus(STEPMOTOR_TIM, TIM_IT_Update)) {
        TIM_ClearITPendingBit(STEPMOTOR_TIM, TIM_IT_Update);
        
        if(isRunning) {
            // 执行单步
            if(motorDirection == CLOCKWISE) {
                currentStep = (currentStep + 1) % 4;    /* 顺时针 */
            } else {
                currentStep = (currentStep == 0) ? 3 : (currentStep - 1);   /* 逆时针 */
            }
            
            // 更新线圈状态
            GPIO_WriteBit(MOTOR_GPIO1_PORT, MOTOR_GPIO1_PIN, (fullStepSequence[currentStep] & 0x01) ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(MOTOR_GPIO2_PORT, MOTOR_GPIO2_PIN, (fullStepSequence[currentStep] & 0x02) ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(MOTOR_GPIO3_PORT, MOTOR_GPIO3_PIN, (fullStepSequence[currentStep] & 0x04) ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(MOTOR_GPIO4_PORT, MOTOR_GPIO4_PIN, (fullStepSequence[currentStep] & 0x08) ? Bit_SET : Bit_RESET);
            
            // 步数控制
            if(stepsRemaining > 0) {
                stepsRemaining--;
                if(stepsRemaining == 0) {
                    StepperMotor_Stop();
                }
            }
        }
    }
}

// 设置速度 (单位us)
void StepperMotor_SetSpeed(uint16_t speed_us) {
    if(speed_us < MIN_SPEED) speed_us = MIN_SPEED;
    if(speed_us > MAX_SPEED) speed_us = MAX_SPEED;
    
    speed_us = (MAX_SPEED + MIN_SPEED) - speed_us;
    STEPMOTOR_TIM->ARR = speed_us;  // 动态更新定时器周期
    
    // 如果定时器已启用，需要手动触发更新
    if(STEPMOTOR_TIM->CR1 & TIM_CR1_CEN) {
        STEPMOTOR_TIM->EGR = TIM_PSCReloadMode_Immediate;
    }
}

// 设置方向
void StepperMotor_SetDirection(MotorDirection direction) {
    motorDirection = direction;
}

// 移动指定步数 -360~360 正值顺时针 负值逆时针
void StepperMotor_Step(uint16_t speed, int16_t steps) {
    
    
    if(steps == 0)
    {
        return ;    /* 不执行动作 */
    }
    /* 1.设置旋转方向 */
    else if(steps > 0)
    {
        motorDirection = CLOCKWISE;     /* 顺时针 */
    }
    else
    {
        motorDirection = ANTICLOCKWISE; /* 逆时针 */
    }
    /* 2.设置为指定步数旋转 */
    if(steps < 0)
    {
        stepsRemaining = -steps;
    }
    else
    {
        stepsRemaining = steps;
    }
    
    isRunning = 1;
    TIM_Cmd(STEPMOTOR_TIM, ENABLE);
    /* 3.设置旋转速度 */
    StepperMotor_SetSpeed(speed);
}

// 连续旋转
void StepperMotor_RotateContinuous(void) {
    stepsRemaining = 0;  // 0表示无限旋转
    isRunning = 1;
    TIM_Cmd(STEPMOTOR_TIM, ENABLE);
}

// 停止电机
void StepperMotor_Stop(void) {
    isRunning = 0;
    TIM_Cmd(STEPMOTOR_TIM, DISABLE);
    GPIO_ResetBits(MOTOR_GPIO1_PORT, MOTOR_GPIO1_PIN);
    GPIO_ResetBits(MOTOR_GPIO2_PORT, MOTOR_GPIO2_PIN);
    GPIO_ResetBits(MOTOR_GPIO3_PORT, MOTOR_GPIO3_PIN);
    GPIO_ResetBits(MOTOR_GPIO4_PORT, MOTOR_GPIO4_PIN);
}

// 检查是否运行
uint8_t StepperMotor_GetState(void) {
    return isRunning;
}
/**
 * @brief   启动步进电机旋转
 * @param   direction   :    CLOCKWISE（顺时针） ANTICLOCKWISE（逆时针）
 * @param   speed_us    :    500~5000步进电机运转速度
 * @retval  返回运动状态
 */
void StepperMotor_Start(MotorDirection direction, uint16_t speed_us)
{
    /* 1.设置旋转方向 */
    motorDirection = direction;
    /* 2.设置为无限旋转 */
    stepsRemaining = 0;
    isRunning = 1;
    TIM_Cmd(STEPMOTOR_TIM, ENABLE);
    /* 3.设置旋转速度 */
    StepperMotor_SetSpeed(speed_us);
}
#endif
