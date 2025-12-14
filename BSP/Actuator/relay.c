#include "relay.h"

static RelayState_TypeDef RelayState = {0};

void RELAY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // GPIO配置
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // 初始化继电器1
    RCC_APB2PeriphClockCmd(RELAY1_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = RELAY1_GPIO_PIN;
    GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStruct);
    RELAY1_OFF();

    // 初始化继电器2
    #if RELAY_NUM >= 2
    RCC_APB2PeriphClockCmd(RELAY2_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = RELAY2_GPIO_PIN;
    GPIO_Init(RELAY2_GPIO_PORT, &GPIO_InitStruct);
    RELAY2_OFF();
    #endif

    // 初始化继电器3
    #if RELAY_NUM >= 3
    RCC_APB2PeriphClockCmd(RELAY3_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = RELAY3_GPIO_PIN;
    GPIO_Init(RELAY3_GPIO_PORT, &GPIO_InitStruct);
    RELAY3_OFF();
    #endif

    // 初始化继电器4
    #if RELAY_NUM >= 4
    RCC_APB2PeriphClockCmd(RELAY4_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = RELAY4_GPIO_PIN;
    GPIO_Init(RELAY4_GPIO_PORT, &GPIO_InitStruct);
    RELAY4_OFF();
    #endif
}
/**
 * @brief   开启指定继电器
 * @param   relay_num：继电器编号 RELAY_ONE | RELAY_TWO | RELAY_THREE | RELAY_FOUR
 * @retval  无
 */
void RELAY_On(RELAY_Number relay_num)
{
    switch(relay_num)
    {
        case 1:
            RELAY1_ON();
            RelayState.Relay1 = RELAY_ON_STATE;
            break;
            
        case 2:
            #if RELAY_NUM >= 2
            RELAY2_ON();
            RelayState.Relay2 = RELAY_ON_STATE;
            #endif
            break;
            
        case 3:
            #if RELAY_NUM >= 3
            RELAY3_ON();
            RelayState.Relay3 = RELAY_ON_STATE;
            #endif
            break;
            
        case 4:
            #if RELAY_NUM >= 4
            RELAY4_ON();
            RelayState.Relay4 = RELAY_ON_STATE;
            #endif
            break;
            
        default:
            break;
    }
}
/**
 * @brief   关闭指定继电器
 * @param   relay_num：继电器编号 RELAY_ONE | RELAY_TWO | RELAY_THREE | RELAY_FOUR
 * @retval  无
 */
void RELAY_Off(RELAY_Number relay_num)
{
    switch(relay_num)
    {
        case 1:
            RELAY1_OFF();
            RelayState.Relay1 = RELAY_OFF_STATE;
            break;
            
        case 2:
            #if RELAY_NUM >= 2
            RELAY2_OFF();
            RelayState.Relay2 = RELAY_OFF_STATE;
            #endif
            break;
            
        case 3:
            #if RELAY_NUM >= 3
            RELAY3_OFF();
            RelayState.Relay3 = RELAY_OFF_STATE;
            #endif
            break;
            
        case 4:
            #if RELAY_NUM >= 4
            RELAY4_OFF();
            RelayState.Relay4 = RELAY_OFF_STATE;
            #endif
            break;
            
        default:
            break;
    }
}
/**
 * @brief   获取指定继电器状态
 * @param   relay_num：继电器编号 RELAY_ONE | RELAY_TWO | RELAY_THREE | RELAY_FOUR
 * @retval  继电器状态
 */
uint8_t RELAY_GetState(RELAY_Number relay_num)
{
    switch(relay_num)
    {
        case RELAY_ONE:     return RelayState.Relay1;
        case RELAY_TWO:     return (RELAY_NUM >= 2) ? RelayState.Relay2 : 0;
        case RELAY_THREE:   return (RELAY_NUM >= 3) ? RelayState.Relay3 : 0;
        case RELAY_FOUR:    return (RELAY_NUM >= 4) ? RelayState.Relay4 : 0;
        default: return 0;
    }
}
