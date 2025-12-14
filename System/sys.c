#include "sys.h"
#include "KEY.h"
#include "Task.h"

#include "adcx.h"
__IO uint32_t SysTimeStamp = 0;         /* 系统时间戳 - ms */
uint8_t Count_Base;                     /* 延时函数时间基准 */
//进入睡眠模式
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//关闭所有中断
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//设置栈顶地址
//__asm void MSR_MSP(u32 addr) 
//{
//    MSR MSP, r0    // 直接使用 r0（调用约定中 addr 通过 r0 传递）
//    BX lr          // 返回（lr 即 r14）
//}
void MSR_MSP(uint32_t addr) {
    __set_MSP(addr);  
}
/**
 * @brief   系统滴答定时器初始化
 * @param   无
 * @retval  无
 */
void SysTick_Init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    while((SystemCoreClock/1000 - 1UL) > SysTick_LOAD_RELOAD_Msk);
    
    SysTick->LOAD  = (uint32_t)(SystemCoreClock/1000 - 1UL);
    NVIC_SetPriority (SysTick_IRQn, 0);     /* 将系统滴答定时器中断优先级设置为最高 */
    
    SysTick->VAL   = 0UL;
    SysTick->CTRL  =    SysTick_CTRL_CLKSOURCE_Msk |
                        SysTick_CTRL_TICKINT_Msk   |
                        SysTick_CTRL_ENABLE_Msk;   
    Count_Base = SystemCoreClock / 1000000;
}
/**
 * @brief   阻塞式延时nus
 * @param   无
 * @retval  无
 */
void delay_us(uint32_t us)
{
    uint32_t Time_Count  = Count_Base * us;
    uint32_t Count_Old = SysTick->VAL;
    uint32_t Count_New = 0;
    uint32_t Count = 0;
    uint32_t Reload = SysTick -> LOAD;
    while(1){
        Count_New = SysTick->VAL;
        if(Count_New != Count_Old){
            if(Count_New < Count_Old){
                Count += Count_Old - Count_New;
            }else{
                Count += Reload - Count_New + Count_Old;
            }
            Count_Old = Count_New;
            if(Count >= Time_Count){
                break;
            }
        }
    }
}

/**
 * @brief   阻塞式延时nms
 * @param   无
 * @retval  无
 */
void delay_ms(uint32_t ms)
{
    while(ms--)
    {
        delay_us(1000);
    }
}
/**
 * @brief   系统滴答定时器中断处理函数 1ms触发一次
 * @param   无
 * @retval  无
 */
void SysTick_Handler(void)
{
    /* 1.按键扫描 */
    Key_Tick();
    /* 2.任务调度 */
    taskScheduler.time_cnt++;
    if(taskScheduler.Sensor_Task.eventStatus == false && taskScheduler.time_cnt % taskScheduler.Sensor_Task.eventSpace == 0)
    {
        taskScheduler.Sensor_Task.eventStatus = true;
    }
    if(taskScheduler.Actuator_Task.eventStatus == false && taskScheduler.time_cnt % taskScheduler.Actuator_Task.eventSpace == 0)
    {
        taskScheduler.Actuator_Task.eventStatus = true;
    }
    if(taskScheduler.Comm_Task.eventStatus == false && taskScheduler.time_cnt % taskScheduler.Comm_Task.eventSpace == 0)
    {
        taskScheduler.Comm_Task.eventStatus = true;
    }
    if(taskScheduler.Extern_Task.eventStatus == false && taskScheduler.time_cnt % taskScheduler.Extern_Task.eventSpace == 0)
    {
        taskScheduler.Extern_Task.eventStatus = true;
    }
    if(taskScheduler.Action_Task.eventStatus == false && taskScheduler.time_cnt % taskScheduler.Action_Task.eventSpace == 0)
    {
        taskScheduler.Action_Task.eventStatus = true;
    }
    taskScheduler.time_cnt = taskScheduler.time_cnt % 100000; /* 系统最大任务间隔为100s */
}
/**
 * @brief   NVIC嵌套向量中断控制器初始化，设置优先级分组
 * @param   none
 * @retval  none
 */
void NVICConfig_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 设置优先级分组为2，抢占优先级为0~3，响应优先级为0~3 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);/* 设置中断向量表地址 */
}
/**
 * @brief   系统外设初始化
 * @param   none
 * @retval  none
 */
void Peripheral_Init(void)
{
    /* 1.禁用JTAG，使能SWD调试接口，释放PA15、PB3、PB4这三个引脚作为普通IO使用 开启所有端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            /* 必须开启AFIO时钟，才能修改引脚复用功能（包括禁用JTAG/SWJ） */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);        /* 禁用JTAG，使能SWD调试接口 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
}
