#include "Actuator_Task.h"
#include "Sensor_Task.h"
__IO uint32_t Actuator_TimeCnt = 0;   /* 执行器动作任务运行时间 单位为Sensor_Task任务运行间隔；如运行间隔为5ms 单位则为5ms */

/******************************* 执行器控制参数赋值 开始 ************************************/
#if STEPMOTOR_ENABLE == _enable_
    StepMotor_Operations StepMotor_Ctrler = {
        .Run_Mode = 0,
        .SetRun_Steps = 500,
        .Run_Direction = CLOCKWISE,         /* 默认顺时针 */
        .Run_Speed = 2000,                  /* 转速设置为2000 */
        .RunState = StepperMotor_GetState,  /* 运动状态 */
        .Stop = StepperMotor_Stop,          /* 停止步进电机运动 */
        .Start = StepperMotor_Start,        /* 启动步进电机运动 */
        .Set_Step = StepperMotor_Step       /* 指定步进电机转动步数 */
    };
#endif
#if RELAY_ENABLE == _enable_
    Relay_Operations Relay_Ctrler = {
        .RunState = RELAY_GetState, /* 参数：继电器编号 RELAY_ONE | RELAY_TWO | RELAY_THREE | RELAY_FOUR*/
        .Stop = RELAY_Off,          /* 参数：继电器编号 */
        .Start = RELAY_On           /* 参数：继电器编号 */
    };
#endif
#if BEEP_ENABLE == _enable_
    BEEP_Operations BEEP_Ctrler = {
        .RunState = BEEP_GetState,
        .Stop = BEEP_Off,
        .Start = BEEP_On
    };
#endif
#if FAN_ENABLE == _enable_
    FAN_Operations FAN_Ctrler = {
        .RunState = FAN_GetState,
        .Stop = FAN_Off,
        .Start = FAN_On
    };
#endif
#if BUMP_ENABLE == _enable_
    BUMP_Operations BUMP_Ctrler = {
        .RunState = BUMP_GetState,
        .Stop = BUMP_Off,
        .Start = BUMP_On
    };
#endif
#if LED_MODULE_ENABLE == _enable_
    LEDM_Operations LEDM_Ctrler = {
#if LED_MODE == LED_MODE_DATA
        .RunState = LED_MOUDLE_GetState,
        .Stop = LED_MOUDLE_Off,
        .Start = LED_MOUDLE_On,
#else
        .SetDuty = LED_MODULE_SetDuty
#endif
    };
#endif
#if JSQ_ENABLE == _enable_
    JSQ_Operations JSQ_Ctrler = {
        .RunState = JSQ_GetState,
        .Stop = JSQ_Off,
        .Start = JSQ_On
    };
#endif
#if SERVO_ENABLE == _enable_
    SERVO_Operations SERVO_Ctrler = {
        .Servo_Angle = 0,
        .SetAngle = Servo_SetAngle
    };
#endif
#if PTC_ENABLE == _enable_
    PTC_Operations PTC_Ctrler = {
        .RunState = PTC_GetState,
        .Stop = PTC_Off,
        .Start = PTC_On
    };
#endif
#if TEC_ENABLE == _enable_
    TEC_Operations TEC_Ctrler = {
        .RunState = TEC_GetState,
        .Stop = TEC_Off,
        .Start = TEC_On
    };
#endif
#if TB6612_ENABLE == _enable_
    TB6612_Operations TB6612_Ctrler = {
        .SetSpeed = TB6612_SetSpeed,
    };
#endif
#if LED_ARRAY_ENABLE == _enable_
    LED_Array_Operations LED_Array_Ctrler = {
        .RunState = LED_Array_GetState,
        .Off = LED_Array_Off,
        .On = LED_Array_On,
    };
#endif
/******************************* 执行器控制参数赋值 结束 ************************************/

/**
 * @brief   执行器底层接口初始化
 * @param   none
 * @retval  none
 */
void Actuator_Init(void)
{
#if LAMP_ENABLE == _enable_
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LAMP_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = LAMP_GPIO_PIN;
    GPIO_Init(LAMP_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(LAMP_GPIO_PORT, LAMP_GPIO_PIN);
#endif
#if STEPMOTOR_ENABLE == _enable_
    StepperMotor_Init();
#endif
#if RELAY_ENABLE == _enable_
    RELAY_Init();
#endif
#if BEEP_ENABLE == _enable_
    BEEP_Init();
#endif
#if FAN_ENABLE == _enable_
    FAN_Init();
#endif
#if BUMP_ENABLE == _enable_
    BUMP_Init();
#endif
#if LED_MODULE_ENABLE == _enable_
    LED_MODULE_Init();
#endif
#if JSQ_ENABLE == _enable_
    JSQ_Init();
#endif
#if SERVO_ENABLE == _enable_
    Servo_Init();
#endif
#if PTC_ENABLE == _enable_
    PTC_Init();
#endif
#if TEC_ENABLE == _enable_
    TEC_Init();
#endif
#if TB6612_ENABLE == _enable_
    TB6612_Init();
#endif
#if LED_ARRAY_ENABLE == _enable_
    LED_Array_Init();
#endif
}

/**
 * @brief   执行器动作任务
 * @param   none
 * @retval  none
 */
void Actuator_Execute(void)
{
    Actuator_TimeCnt++;
#if (LAMP_ENABLE == _enable_) && (LDR_ENABLE == _enable_)
    static uint8_t lamp_state = 0;
    if (Actuator_TimeCnt % LAMP_RUN_SPACE == 0)
    {
        if (lamp_state == 0)
        {
            if ((int16_t)LDR_Lux < (int16_t)(LDR_LAMP_THRESHOLD - LDR_LAMP_HYSTERESIS))
            {
                GPIO_SetBits(LAMP_GPIO_PORT, LAMP_GPIO_PIN);
                lamp_state = 1;
            }
        }
        else
        {
            if ((int16_t)LDR_Lux > (int16_t)(LDR_LAMP_THRESHOLD + LDR_LAMP_HYSTERESIS))
            {
                GPIO_ResetBits(LAMP_GPIO_PORT, LAMP_GPIO_PIN);
                lamp_state = 0;
            }
        }
    }
#endif
#if STEPMOTOR_ENABLE == _enable_
    if(Actuator_TimeCnt % STEPMOTOR_RUN_SPACE == 0)
    {
        /* 1.步进电机停止驱动 */
        //StepMotor_Ctrler.Stop();
        /* 2.步进电机启动运动 顺时针 */
        //StepMotor_Ctrler.Start(CLOCKWISE, 3000);
        /* 3.步进电机启动运动 逆时针 */
        //StepMotor_Ctrler.Start(ANTICLOCKWISE, 3000);
        /* 4.指定步进电机转动步数 正值顺时针 负值逆时针 */
        //StepMotor_Ctrler.Set_Step(90);  /* -360~360 */
    }
#endif
#if RELAY_ENABLE == _enable_
    if(Actuator_TimeCnt % RELAY_RUN_SPACE == 0)
    {
        Relay_Ctrler.RunState(RELAY_ONE);
//        Relay_Ctrler.Stop(RELAY_ONE);
//        Relay_Ctrler.Start(RELAY_ONE);
    }
#endif
#if BEEP_ENABLE == _enable_
    if(Actuator_TimeCnt % BEEP_RUN_SPACE == 0)
    {
//        BEEP_Ctrler.RunState();
//        BEEP_Ctrler.Stop();
//        BEEP_Ctrler.Start();
    }
#endif
#if FAN_ENABLE == _enable_
    if(Actuator_TimeCnt % FAN_RUN_SPACE == 0)
    {
//        FAN_Ctrler.RunState();
//        FAN_Ctrler.Stop();
//        FAN_Ctrler.Start();
    }
#endif
#if BUMP_ENABLE == _enable_
    if(Actuator_TimeCnt % BUMP_RUN_SPACE == 0)
    {
//        BUMP_Ctrler.RunState();
//        BUMP_Ctrler.Stop();
//        BUMP_Ctrler.Start();
    }
#endif
#if LED_MODULE_ENABLE == _enable_
    if(Actuator_TimeCnt % LED_MODULE_RUN_SPACE == 0)
    {
//        LEDM_Ctrler.RunState();
//        LEDM_Ctrler.Stop();
//        LEDM_Ctrler.Start();
    }
#endif
#if JSQ_ENABLE == _enable_
    if(Actuator_TimeCnt % JSQ_RUN_SPACE == 0)
    {
//        JSQ_Ctrler.RunState();
//        JSQ_Ctrler.Stop();
//        JSQ_Ctrler.Start();
    }
#endif
#if SERVO_ENABLE == _enable_
    if(Actuator_TimeCnt % SERVO_RUN_SPACE == 0)
    {
        SERVO_Ctrler.SetAngle(SERVO_Ctrler.Servo_Angle);
    }
#endif
#if PTC_ENABLE == _enable_
    if(Actuator_TimeCnt % PTC_RUN_SPACE == 0)
    {
//        PTC_Ctrler.RunState();
//        PTC_Ctrler.Stop();
//        PTC_Ctrler.Start();
    }
#endif
#if TEC_ENABLE == _enable_
    if(Actuator_TimeCnt % TEC_RUN_SPACE == 0)
    {
//        TEC_Ctrler.RunState();
//        TEC_Ctrler.Stop();
//        TEC_Ctrler.Start();
    }
#endif
#if TB6612_ENABLE == _enable_
    if(Actuator_TimeCnt % TB6612_RUN_SPACE == 0)
    {
        /* -100~100 注：设置速度过小电机可能不会运动 */
//        TB6612_Ctrler.SetSpeed(MOTOR_A, 0);
//        TB6612_Ctrler.SetSpeed(MOTOR_B, 0);
    }
#endif
#if LED_ARRAY_ENABLE == _enable_
    if(Actuator_TimeCnt % LED_ARRAY_RUN_SPACE == 0)
    {
//        LED_Array_Ctrler.RunState(LED_ONE);
//        LED_Array_Ctrler.On(LED_ONE);
//        LED_Array_Ctrler.Off(LED_ONE);
    }
#endif
}
