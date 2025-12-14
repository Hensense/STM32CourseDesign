#include "gate_guard.h"

void GATE_GUARD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (GATE_GUARD_GPIO_CLK, ENABLE );            // 打开连接 单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = GATE_GUARD_GPIO_PIN;                // 配置连接 单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                     // 设置为上拉输入
    
    GPIO_Init(GATE_GUARD_GPIO_PORT, &GPIO_InitStructure);             // 初始化 

}

uint8_t GATE_GUARD_GetState()
{
    static uint8_t gate_judge = 0;
    if(GATE_GUARD == 1)
    {
        if(gate_judge == 0)
        {
            gate_judge = 1;
        }
        else if(gate_judge == 1)
        {
            if(GATE_GUARD == 1)
            {
                gate_judge = 2;
                return 1;   /* 门磁开关处于开状态 */
            }
            else
            {
                gate_judge = 0;
            }
        }
        else if(gate_judge == 2)
        {
            return 2;       /* 当前处于持续开状态 */
        }
    }
    else
    {
        gate_judge = 0;
    }
    return 0;              /* 门磁开关处于关状态 */
}
