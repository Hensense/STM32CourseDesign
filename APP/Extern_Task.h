#ifndef __EXTERN_TASK_H
#define __EXTERN_TASK_H

/* ***************************头文件引入 开始******************************* */
#include "main.h"
/* ***************************头文件引入 结束******************************* */

/* ***************************宏定义 开始*********************************** */
/* _enable_:使能某个模块 _disable_:失能某个模块 */
#define DS1302_ENABLE       _disable_    /* DS1302时钟模块：需要任意三个IO引脚 */
#define JQ8900_ENABLE       _disable_    /* JQ8900语音模块：仅需一个IO引脚即可驱动 */
#define JQ6500_ENABLE       _disable_    /* JQ6500语音模块：需要任意一个串口 USART1/2/3均可 */
#define JR6001_ENABLE       _disable_    /* JR6001语音模块：需要任意一个串口 USART1/2/3均可 */
#define ASRPRO_ENABLE       _disable_    /* ASRPRO离线语音识别模块：需要任意一个串口 USART1/2/3均可 */
/* ***************************宏定义 结束*********************************** */

/* ***************************预编译 开始*********************************** */
#if DS1302_ENABLE == _enable_
#include "ds1302.h"
#define DS1302_RUN_SPACE    5           /* 数据更新间隔 时间单位为Extern_Task执行间隔 */
typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint8_t  week;
}DS1302_DataStruct;
extern DS1302_DataStruct DS1302_DateInfo;
#endif
#if JQ6500_ENABLE == _enable_
#include "jq6500.h"
#define JQ6500_RUN_SPACE    10          /* 执行间隔 时间单位为Extern_Task执行间隔 */
typedef struct
{
    void (*Music_Play)(uint8_t);
}JQ6500_Operations;
extern JQ6500_Operations JQ6500_Ctrler;
#endif
#if JR6001_ENABLE == _enable_
#include "JR6001.h"
#define JR6001_RUN_SPACE    10          /* 执行间隔 时间单位为Extern_Task执行间隔 */
typedef struct
{
    void (*Set_Volume)(uint8_t);    /* 设置音量 */
    void (*Music_Stop)(void);       /* 停止播放 */
    void (*Music_Play)(uint32_t);   /* 开始播放 */
}JR6001_Operations;
extern JR6001_Operations JR6001_Ctrler;
#endif
#if JQ8900_ENABLE == _enable_
#include "JQ8900.h"
#define JQ8900_RUN_SPACE    10          /* 执行间隔 时间单位为Extern_Task执行间隔 */
typedef struct
{
    void (*Set_Volume)(uint8_t);    /* 设置音量 */
    void (*Music_Stop)(void);       /* 停止播放 */
    void (*Music_Play)(uint16_t);   /* 开始播放 */
}JQ8900_Operations;
extern JQ8900_Operations JQ8900_Ctrler;
#endif
#if ASRPRO_ENABLE == _enable_
#include "ASRPRO.h"
#define ASRPRO_RUN_SPACE    10          /* 执行间隔 时间单位为Extern_Task执行间隔 */
typedef struct
{
    /* 接收部分 */
    __IO uint8_t Update_Flag;       /* ASRPRO接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Led_State;         /* 接收ASRPRO发送的控制信息 */
    __IO uint8_t Led_State_Flag;    /* LED控制状态更新标志 */
    //..........后续信息需要根据需求定义
    /* 发送部分 */
    void (*Send)(char*, ...);
}ASRPRO_Operations;
extern ASRPRO_Operations ASRPRO_Ctrler;
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
void Extern_Init(void);         /* 扩展模块驱动初始化 */
void Extern_Execute(void);      /* 扩展任务 */
/* ***************************函数接口声明 结束***************************** */

#endif
