#include "sw420.h"
/* 震动传感器 */
void SW420_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (SW420_GPIO_CLK, ENABLE );    // 打开连接 传感器DO 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = SW420_GPIO_PIN;        // 配置连接 传感器DO 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;        // 设置为下拉输入
    
    GPIO_Init(SW420_GPIO_PORT, &GPIO_InitStructure);     // 初始化 
    
}
/**
 * @brief   震动传感器 有震动时为高电平 无震动时为低电平 
 * @param   无
 * @retval  0x00：状态未改变 0x01：发生震动 0x02：未震动（注意：震动或者未震动标志位每次触发只返回一次标志位）
 */
uint8_t sw420_run_state = 0;  /* 运行状态机 用于消抖 */
uint8_t SW420_GetFlag(void)
{
    static uint8_t nowData = 0, preData = 0;
    uint8_t delay_count = 0;   /* 用于非阻塞延时消抖 */
    nowData = GPIO_ReadInputDataBit(SW420_GPIO_PORT, SW420_GPIO_PIN);
    if(nowData != preData)
    {
        if(sw420_run_state == 0)
        {
            sw420_run_state = 1;
            return 0;
        }
        else if(sw420_run_state == 1)
        {
            delay_count++;
            if(delay_count >= 10)
            {
                delay_count = 0;
                sw420_run_state = 0;
                preData = nowData;
                if(nowData == 1)
                {
                    return 0x01;    /* 震动 */
                }
                else
                {
                    return 0x02;    /* 无震动 */
                }
            }
            else
            {
                return 0;
            }
        }
        
    }
    delay_count = 0;
    sw420_run_state = 0;
    preData = nowData;
    return 0x00;            /* 默认返回值 - 不做处理 */
}



