#include "stm32f10x.h"                  // Device header
#include <time.h>
//OLED_Printf(0,0,OLED_8X16, "%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
uint16_t MyRTC_Time[] = {2025, 9, 4, 21, 40, 55};    //定义全局的时间数组，数组内容分别为年、月、日、时、分、秒

void MyRTC_SetTime(void);                //函数声明
/* PC14/PC15已被按键占用，不要开启LSE，否则按键会失效 */
void MyRTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    PWR_BackupAccessCmd(ENABLE);
    
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        RCC_LSICmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        
        RTC_SetPrescaler(40000 - 1);
        RTC_WaitForLastTask();
        
        MyRTC_SetTime();
        
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        RCC_LSICmd(ENABLE);                //即使不是第一次配置，也需要再次开启LSI时钟
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }
}

/**
  * 函    数：RTC设置时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，全局数组里时间值将刷新到RTC硬件电路
  */
void MyRTC_SetTime(void)
{
    time_t time_cnt;        //定义秒计数器数据类型
    struct tm time_date;    //定义日期时间数据类型
    
    time_date.tm_year = MyRTC_Time[0] - 1900;        //将数组的时间赋值给日期时间结构体
    time_date.tm_mon = MyRTC_Time[1] - 1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];
    
    time_cnt = mktime(&time_date) - 8 * 60 * 60;    //调用mktime函数，将日期时间转换为秒计数器格式
                                                    //- 8 * 60 * 60为东八区的时区调整
    
    RTC_SetCounter(time_cnt);                        //将秒计数器写入到RTC的CNT中
    RTC_WaitForLastTask();                            //等待上一次操作完成
}

/**
  * 函    数：RTC读取时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，RTC硬件电路里时间值将刷新到全局数组
  */
void MyRTC_ReadTime(void)
{
    time_t time_cnt;        //定义秒计数器数据类型
    struct tm time_date;    //定义日期时间数据类型
    
    time_cnt = RTC_GetCounter() + 8 * 60 * 60;        //读取RTC的CNT，获取当前的秒计数器
                                                    //+ 8 * 60 * 60为东八区的时区调整
    
    time_date = *localtime(&time_cnt);                //使用localtime函数，将秒计数器转换为日期时间格式
    
    MyRTC_Time[0] = time_date.tm_year + 1900;        //将日期时间结构体赋值给数组的时间
    MyRTC_Time[1] = time_date.tm_mon + 1;
    MyRTC_Time[2] = time_date.tm_mday;
    MyRTC_Time[3] = time_date.tm_hour;
    MyRTC_Time[4] = time_date.tm_min;
    MyRTC_Time[5] = time_date.tm_sec;
}
