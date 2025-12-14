#ifndef __ACTUATOR_TASK_H
#define __ACTUATOR_TASK_H

/* ***************************头文件引入 开始******************************* */
#include "main.h"
/* ***************************头文件引入 结束******************************* */

/* ***************************宏定义 开始*********************************** */
/* _enable_:使能某个执行器 _disable_:失能某个执行器 */
#define RELAY_ENABLE            _disable_    /* 继电器驱动：根据继电器数量分配引脚 1个继电器需要一个IO引脚 */
#define BEEP_ENABLE             _disable_    /* 蜂鸣器驱动：需要任意一个IO引脚 */
#define FAN_ENABLE              _disable_    /* 无刷风扇驱动：需要一个IO引脚和一个继电器 */
#define BUMP_ENABLE             _disable_    /* 水泵驱动：需要一个IO引脚和一个继电器/专用驱动板 */
#define JSQ_ENABLE              _disable_    /* 加湿器模块驱动：需要一个IO引脚和一个继电器/专用驱动板 */
#define PTC_ENABLE              _disable_    /* PTC加热器模块驱动：需要一个IO引脚和一个继电器 */
#define TEC_ENABLE              _disable_    /* TEC制冷片模块驱动：需要一个IO引脚和一个继电器 */
#define LED_ARRAY_ENABLE        _disable_    /* LED阵列驱动：根据LED数量分配引脚 1个LED灯需要一个IO引脚 最多支持四个IO引脚 */
#define LED_MODULE_ENABLE       _disable_    /* 大功率LED模块驱动：需要一个IO引脚 或者 使用一个带有PWM通道的引脚 */
#define LAMP_ENABLE             _enable_     /* 外接灯驱动：需要一个IO引脚 */
#define LAMP_GPIO_CLK           RCC_APB2Periph_GPIOB
#define LAMP_GPIO_PORT          GPIOB
#define LAMP_GPIO_PIN           GPIO_Pin_13
#define LAMP_RUN_SPACE          20
#define LDR_LAMP_THRESHOLD      200
#define LDR_LAMP_HYSTERESIS     20
#define SERVO_ENABLE            _disable_    /* 模拟/数字舵机驱动：需要一个带有PWM通道的引脚 */
#define STEPMOTOR_ENABLE        _disable_    /* 28BYJ-48步进电机驱动：需要四个任意IO引脚和一个TIMx，可为TIM1/2/3/4 */
#define TB6612_ENABLE           _disable_    /* TB6612电机驱动模块：需要两个IO引脚和PWM输出引脚 / 需要四个IO引脚和两个PWM输出引脚 */
/* ***************************宏定义 结束*********************************** */

/* ***************************预编译 开始*********************************** */
/* 注意：标志类变量使用后需要清除 */
#if STEPMOTOR_ENABLE == _enable_
#include    "stepper_motor.h"
#define     STEPMOTOR_RUN_SPACE         40      /* 步进电机修改运动状态间隔 时间单位为Actuator_Task执行间隔 40 * 5 = 200ms */
typedef struct
{
    uint8_t Run_Mode;                           /* 电机运行模式：0 无限旋转 1 指定步数旋转 */
    int16_t SetRun_Steps;                       /* 设置运行的步数 -360 ~ 360 正值顺时针 负值逆时针 */
    MotorDirection Run_Direction;               /* 电机运转方向：CLOCKWISE顺时针 ANTICLOCKWISE逆时针 */
    uint16_t Run_Speed;                         /* 电机运行速度 */
    uint8_t (*RunState)(void);                  /* 运动状态 0x00（停止状态） 0x01(运动状态) */
    void (*Stop)(void);
    void (*Start)(MotorDirection, uint16_t);
    void (*Set_Step)(uint16_t, int16_t);
}StepMotor_Operations;
extern StepMotor_Operations StepMotor_Ctrler;   /* 步进电机控制器，控制步进电机运转方向，速度，启停 */
#endif

#if RELAY_ENABLE == _enable_
#include "relay.h"
#define     RELAY_RUN_SPACE             50
typedef struct
{
    uint8_t (*RunState)(RELAY_Number);          /* 需要输入指定继电器编号 返回运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(RELAY_Number);
    void (*Start)(RELAY_Number);
}Relay_Operations;
extern Relay_Operations Relay_Ctrler;           /* 继电器控制器，控制继电器开启/关闭 */
#endif

#if BEEP_ENABLE == _enable_
#include "beep.h"
#define     BEEP_RUN_SPACE              30
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}BEEP_Operations;
extern BEEP_Operations BEEP_Ctrler;             /* 蜂鸣器控制器，控制蜂鸣器开启/关闭 */
#endif

#if FAN_ENABLE == _enable_
#include "fan.h"
#define     FAN_RUN_SPACE               20
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}FAN_Operations;
extern FAN_Operations FAN_Ctrler;               /* 风扇控制器，控制风扇开启/关闭 */
#endif

#if BUMP_ENABLE == _enable_
#include "bump.h"
#define     BUMP_RUN_SPACE              35
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}BUMP_Operations;
extern BUMP_Operations BUMP_Ctrler;             /* 水泵控制器，控制水泵开启/关闭 */
#endif

#if LED_MODULE_ENABLE == _enable_
#include "led_module.h"
#define     LED_MODULE_RUN_SPACE        25
typedef struct
{
#if LED_MODE == LED_MODE_DATA
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
#else
    void (*SetDuty)(uint8_t);
#endif
}LEDM_Operations;
extern LEDM_Operations LEDM_Ctrler;             /* 大功率LED控制器，控制灯光开启/关闭 */
#endif

#if JSQ_ENABLE == _enable_
#include "jsq.h"
#define     JSQ_RUN_SPACE               15
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}JSQ_Operations;
extern JSQ_Operations JSQ_Ctrler;               /* 加湿器控制器，控制加湿器开启/关闭 */
#endif

#if SERVO_ENABLE == _enable_
#include "servo.h"
#define     SERVO_RUN_SPACE             45
typedef struct
{
    float Servo_Angle;
    void (*SetAngle)(float);
}SERVO_Operations;
extern SERVO_Operations SERVO_Ctrler;           /* 舵机控制器，控制舵机旋转角度 */
#endif

#if PTC_ENABLE == _enable_
#include "ptc.h"
#define     PTC_RUN_SPACE               10
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}PTC_Operations;
extern PTC_Operations PTC_Ctrler;               /* PTC加热片控制器，控制加热片开启/关闭 */
#endif

#if TEC_ENABLE == _enable_
#include "tec.h"
#define     TEC_RUN_SPACE               5
typedef struct
{
    uint8_t (*RunState)(void);                  /* 运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Stop)(void);
    void (*Start)(void);
}TEC_Operations;
extern TEC_Operations TEC_Ctrler;               /* TEC制冷片控制器，控制制冷片开启/关闭 */
#endif
#if TB6612_ENABLE == _enable_
#include "tb6612.h"
#define     TB6612_RUN_SPACE            15      /* 电机状态修改间隔 单位为Actuator_Task任务执行间隔 */
typedef struct
{
    void (*SetSpeed)(Motor_Nmber, int8_t);      /* 电机编号 电机旋转速度（0停止 speed>0 顺时针 speed<0 逆时针） */
}TB6612_Operations;
extern TB6612_Operations TB6612_Ctrler;         /* 电机驱动控制器，控制电机运动状态 */
#endif
#if LED_ARRAY_ENABLE == _enable_
#include "led_array.h"
#define     LED_ARRAY_RUN_SPACE         10
typedef struct
{
    uint8_t (*RunState)(LED_Number);          /* 需要输入指定LED编号 返回运行状态 0x00（关闭状态） 0x01(开启状态) */
    void (*Off)(LED_Number);
    void (*On)(LED_Number);
}LED_Array_Operations;
extern LED_Array_Operations LED_Array_Ctrler; /* LED控制器，控制LED开启/关闭 */
#endif
/* ***************************预编译 结束*********************************** */

/* ***************************枚举类型定义 开始***************************** */

/* ***************************枚举类型定义 结束***************************** */

/* ***************************共用体类型定义 开始*************************** */

/* ***************************共用体类型定义 结束*************************** */

/* ***************************结构体类型定义 开始*************************** */

/* ***************************结构体类型定义 结束*************************** */

/* ***************************外部变量声明 开始***************************** */

/* ***************************外部变量声明 结束***************************** */

/* ***************************函数接口声明 开始***************************** */
void Actuator_Init(void);       /* 执行器底层接口初始化 */
void Actuator_Execute(void);    /* 执行器动作任务 */
/* ***************************函数接口声明 结束***************************** */

#endif
