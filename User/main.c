#include "main.h"   /* 引入头文件 */
#include "Task.h"
#include "Serial.h"

int main(void)
{
    SystemInit();       /* 系统初始化 */
    SysTick_Init();     /* 系统滴答定时器初始化 */
    Key_Init();         /* KEY初始化 */
    OLED_Init();        /* OLED初始化 */
    MyRTC_Init();       /* RTC初始化 */
    NVICConfig_Init();  /* NVIC配置 */
    Peripheral_Init();  /* 系统外设初始化 */
    Task_Init();        /* 任务调度初始化 */
	Serial_Init();
	
    while(1)
    {
        Task_Scheduler();   /* 任务调度器运行 */
		
    }
}
