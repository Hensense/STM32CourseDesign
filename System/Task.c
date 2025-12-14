#include "Task.h"
#include "adcx.h"
#include "Action_Task.h"
void RealTime_Task(void);       /* 实时任务 */
void Task_HardwareInit(void);   /* 底层硬件初始化 */

TaskScheduler taskScheduler = {0};
/**
 * @brief   任务调度器初始化，初始化任务执行间隔，初始化硬件底层接口
 * @param   none
 * @retval  none
 */
void Task_Init(void)
{
    /* 1.底层接口初始化 */
    Task_HardwareInit();
    /* 2.初始化任务时间戳 */
    taskScheduler.time_cnt = 0;
    /* 3.传感器数据获取任务 */
    taskScheduler.Sensor_Task.eventStatus = false;      /* 任务状态 */
    taskScheduler.Sensor_Task.eventSpace = 1;           /* 任务执行间隔 - 1ms */
    /* 4.执行器动作任务 */
    taskScheduler.Actuator_Task.eventStatus = false;    /* 任务状态 */
    taskScheduler.Actuator_Task.eventSpace = 5;         /* 任务执行间隔 - 5ms */
    /* 5.通信任务 */
    taskScheduler.Comm_Task.eventStatus = false;        /* 任务状态 */
    taskScheduler.Comm_Task.eventSpace = 100;           /* 任务执行间隔 - 100ms */
    /* 6.扩展任务 */
    taskScheduler.Extern_Task.eventStatus = false;      /* 任务状态 */
    taskScheduler.Extern_Task.eventSpace = 50;          /* 任务执行间隔 - 50ms */
    /* 7.动作任务 */
    taskScheduler.Action_Task.eventStatus = false;      /* 任务状态 */
    taskScheduler.Action_Task.eventSpace = 1;           /* 任务执行间隔 - 1ms */
    /* 8.初始化传感器数据 */
    Sensor_Execute();
}

/**
 * @brief   任务调度器执行函数，需要放在while(1)中执行
 * @param   none
 * @retval  none
 */
void Task_Scheduler(void)
{
    /* 1.实时任务 */
    RealTime_Task();
    /* 2.传感器数据获取任务 */
    if(taskScheduler.Sensor_Task.eventStatus == true)
    {
        taskScheduler.Sensor_Task.eventStatus = false;
        Sensor_Execute();       /* 传感器数据获取 */
    }
    /* 3.执行器任务 */
    if(taskScheduler.Actuator_Task.eventStatus == true)
    {
        taskScheduler.Actuator_Task.eventStatus = false;
        Actuator_Execute();     /* 执行器任务执行 */
    }
    /* 4.通信模块交互任务 */
    if(taskScheduler.Comm_Task.eventStatus == true)
    {
        taskScheduler.Comm_Task.eventStatus = false;
        Comm_Execute();         /* 通信任务执行 */
    }
    /* 5.扩展模块驱动任务 */
    if(taskScheduler.Extern_Task.eventStatus == true)
    {
        taskScheduler.Extern_Task.eventStatus = false;
        Extern_Execute();       /* 扩展模块任务执行 */
    }
    /* 6.动作任务：负责处理各个任务获取的数据 执行动作 */
    if(taskScheduler.Action_Task.eventStatus == true)
    {
        taskScheduler.Action_Task.eventStatus = false;
        Action_Execute();
    }
}
/**
 * @brief   底层硬件初始化函数，需要在任务调度前执行
 * @param   none
 * @retval  none
 */
void Task_HardwareInit(void)
{
    Sensor_Init();      /* 传感器底层接口初始化 */
    Actuator_Init();    /* 执行器底层接口初始化 */ 
    Comm_Init();        /* 通信接口初始化 */
    Extern_Init();      /* 扩展模块驱动初始化 */
    /* ******************** STM32外设初始化 ************************* */
    ADCx_AdvancedInit();
}

/**
 * @brief   实时任务 对实时性要求较高的功能部分需要放在此函数
 * @param   none
 * @retval  none
 */
void RealTime_Task(void)
{
    MyRTC_ReadTime();           /* 获取实时时间 */
#if WIFI_ENABLE  ==  _enable_
    if(WIFI_GetRxCompleteFlag() == WIFI_OK)
    {
        WIFI_ParseRxData();         /* 解析数据 发送响应 */
    }
    if(Wifi_Handler.Response_Flag)
    {
        Wifi_Handler.Response_Flag = 0;
        WIFI_SendOnenetCommandResponse(Wifi_Handler.Response_Id, 200, "success");
    }
#endif
}
