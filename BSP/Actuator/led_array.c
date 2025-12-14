#include "led_array.h"

// 全局LED状态变量
static LEDState_TypeDef LEDState = {0};

/**
 * @brief 初始化LED矩阵
 */
void LED_Array_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置GPIO为推挽输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 初始化LED1
    RCC_APB2PeriphClockCmd(LED1_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    LED1_OFF(); // 初始状态关闭
    LEDState.LED1 = 0;

// 根据配置初始化其他LED
#if LED_NUM >= 2
    RCC_APB2PeriphClockCmd(LED2_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
    LED2_OFF();
    LEDState.LED2 = 0;
#endif

#if LED_NUM >= 3
    RCC_APB2PeriphClockCmd(LED3_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
    LED3_OFF();
    LEDState.LED3 = 0;
#endif

#if LED_NUM >= 4
    RCC_APB2PeriphClockCmd(LED4_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
    GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);
    LED4_OFF();
    LEDState.LED4 = 0;
#endif
}

/**
 * @brief 打开指定LED
 * @param led_num LED编号 (LED_ONE, LED_TWO, LED_THREE | LED_FOUR)
 */
void LED_Array_On(LED_Number led_num)
{
    switch(led_num)
    {
        case LED_ONE:
            LED1_ON();
            LEDState.LED1 = LED_ON_STATE;
            break;
            
#if LED_NUM >= 2
        case LED_TWO:
            LED2_ON();
            LEDState.LED2 = LED_ON_STATE;
            break;
#endif
            
#if LED_NUM >= 3
        case LED_THREE:
            LED3_ON();
            LEDState.LED3 = LED_ON_STATE;
            break;
#endif
            
#if LED_NUM >= 4
        case LED_FOUR:
            LED4_ON();
            LEDState.LED4 = LED_ON_STATE;
            break;
#endif
            
        default:
            // 无效LED编号，不做任何操作
            break;
    }
}

/**
 * @brief 关闭指定LED
 * @param led_num LED编号 (LED_ONE, LED_TWO, LED_THREE | LED_FOUR)
 */
void LED_Array_Off(LED_Number led_num)
{
    switch(led_num)
    {
        case LED_ONE:
            LED1_OFF();
            LEDState.LED1 = LED_OFF_STATE;
            break;
            
#if LED_NUM >= 2
        case LED_TWO:
            LED2_OFF();
            LEDState.LED2 = LED_OFF_STATE;
            break;
#endif
            
#if LED_NUM >= 3
        case LED_THREE:
            LED3_OFF();
            LEDState.LED3 = LED_OFF_STATE;
            break;
#endif
            
#if LED_NUM >= 4
        case LED_FOUR:
            LED4_OFF();
            LEDState.LED4 = LED_OFF_STATE;
            break;
#endif
            
        default:
            // 无效LED编号，不做任何操作
            break;
    }
}

/**
 * @brief 切换指定LED状态
 * @param led_num LED编号 (LED_ONE, LED_TWO, LED_THREE | LED_FOUR)
 */
void LED_Array_Toggle(LED_Number led_num)
{
    switch(led_num)
    {
        case LED_ONE:
            if(LEDState.LED1) {
                LED1_OFF();
                LEDState.LED1 = 0;
            } else {
                LED1_ON();
                LEDState.LED1 = 1;
            }
            break;
            
#if LED_NUM >= 2
        case LED_TWO:
            if(LEDState.LED2) {
                LED2_OFF();
                LEDState.LED2 = 0;
            } else {
                LED2_ON();
                LEDState.LED2 = 1;
            }
            break;
#endif
            
#if LED_NUM >= 3
        case LED_THREE:
            if(LEDState.LED3) {
                LED3_OFF();
                LEDState.LED3 = 0;
            } else {
                LED3_ON();
                LEDState.LED3 = 1;
            }
            break;
#endif
            
#if LED_NUM >= 4
        case LED_FOUR:
            if(LEDState.LED4) {
                LED4_OFF();
                LEDState.LED4 = 0;
            } else {
                LED4_ON();
                LEDState.LED4 = 1;
            }
            break;
#endif
            
        default:
            // 无效LED编号，不做任何操作
            break;
    }
}

/**
 * @brief 获取指定LED状态
 * @param led_num LED编号 (LED_ONE, LED_TWO, LED_THREE | LED_FOUR)
 * @return 1:LED亮, 0:LED灭
 */
uint8_t LED_Array_GetState(LED_Number led_num)
{
    switch(led_num)
    {
        case LED_ONE:
            return LEDState.LED1;
            
#if LED_NUM >= 2
        case LED_TWO:
            return LEDState.LED2;
#endif
            
#if LED_NUM >= 3
        case LED_THREE:
            return LEDState.LED3;
#endif
            
#if LED_NUM >= 4
        case LED_FOUR:
            return LEDState.LED4;
#endif
            
        default:
            return 0; // 无效LED编号返回0
    }
}
