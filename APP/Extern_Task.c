#include "Extern_Task.h"
__IO uint32_t Extern_TimeCnt = 0;   /* 数据更新任务运行时间 单位为Extern_Task任务运行间隔；如运行间隔为1ms 单位则为1ms */
/******************************* 数据更新 开始 ************************************/
#if DS1302_ENABLE == _enable_
    DS1302_DataStruct DS1302_DateInfo;
    static void DS1302_GetData(void)
    {
        /* 获取时间 */
        DS1302_read_realTime();
        /* 提取时间 */
        DS1302_DateInfo.year = TimeData.year;
        DS1302_DateInfo.month = TimeData.month;
        DS1302_DateInfo.day = TimeData.day;
        DS1302_DateInfo.week = TimeData.week;
        DS1302_DateInfo.hour = TimeData.hour;
        DS1302_DateInfo.minute = TimeData.minute;
        DS1302_DateInfo.second = TimeData.second;
    }
#endif
#if JQ6500_ENABLE == _enable_
    JQ6500_Operations JQ6500_Ctrler = {
        .Music_Play = JQ6500_Play,
    };
#endif
#if JR6001_ENABLE == _enable_
    JR6001_Operations JR6001_Ctrler = 
    {
        .Set_Volume = JR6001_SetVolume,
        .Music_Stop = JR6001_StopPlay,
        .Music_Play = JR6001_PlayMusic
    };
#endif
#if JQ8900_ENABLE == _enable_
    JQ8900_Operations JQ8900_Ctrler = 
    {
        .Set_Volume = JQ8900_SetVolume,
        .Music_Stop = JQ8900_StopMusic,
        .Music_Play = JQ8900_PlayMusic
    };
#endif
#if ASRPRO_ENABLE  ==  _enable_
    ASRPRO_Operations ASRPRO_Ctrler = {
        .Send = ASRPRO_Printf,
    };
    static void ASRPRO_DataProcess(void)
    {
        if(ASRPRO_Ctrler.Update_Flag == 1)
        {
            ASRPRO_Ctrler.Update_Flag = 0;    /* 清空标志 允许接收新的数据帧 */
            if(ASRPRO_Ctrler.Led_State_Flag == 1)
            {
                ASRPRO_Ctrler.Led_State_Flag = 0;
                /* 执行控制命令 */
                //ASRPRO_Ctrler.Led_State;
            }
            //.............根据需求修改添加
        }
    }
#endif
/******************************* 数据更新 结束 ************************************/
/**
 * @brief   扩展模块驱动初始化
 * @param   none
 * @retval  none
 */
void Extern_Init(void)
{
#if DS1302_ENABLE == _enable_
    DS1302_Init();
#endif
#if JQ6500_ENABLE == _enable_
    JQ6500_Init();
#endif
#if JR6001_ENABLE == _enable_
    JR6001_Init();
#endif
#if JQ8900_ENABLE == _enable_
    JQ8900_Init();
#endif
#if ASRPRO_ENABLE == _enable_
    ASRPRO_Init();
#endif
}
/**
 * @brief   扩展任务
 * @param   none
 * @retval  none
 */
void Extern_Execute(void)
{
    Extern_TimeCnt++;
#if DS1302_ENABLE == _enable_
    if(Extern_TimeCnt % DS1302_RUN_SPACE == 0)
    {
        DS1302_GetData();
    }
#endif
#if JQ6500_ENABLE == _enable_
    if(Extern_TimeCnt % JQ6500_RUN_SPACE == 0)
    {
//        JQ6500_Ctrler.Music_Play(001); //wav语音格式
    }
#endif
#if JR6001_ENABLE == _enable_
    if(Extern_TimeCnt % JR6001_RUN_SPACE == 0)
    {
//        JR6001_Ctrler.Set_Volume(24);   /* 设置音量为最大 0~30 */
//        JR6001_Ctrler.Music_Play(1);    /* 播放序列为00001的音乐 */
    }
#endif
#if JQ8900_ENABLE == _enable_
    if(Extern_TimeCnt % JQ8900_RUN_SPACE == 0)
    {
//        JQ8900_Ctrler.Set_Volume(2);    /* 设置音量为最大 1~9 */
//        JQ8900_Ctrler.Music_Stop();     /* 停止播放 */
//        JQ8900_Ctrler.Music_Play(1);    /* 播放序列为00001的音乐 */
    }
#endif
#if ASRPRO_ENABLE == _enable_
    if(Extern_TimeCnt % ASRPRO_RUN_SPACE == 0)
    {
        /* 数据接收处理 */
        ASRPRO_DataProcess();
        /* 发送至ASRPRO 可播报特定语音 */
        //ASRPRO_Ctrler.Send("Hello ASRPRO!!!");
    }
#endif
}
