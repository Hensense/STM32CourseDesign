#include "beep.h"

void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BEEP_CLK, ENABLE ); //≈‰÷√ ±÷”
    
    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(BEEP_GPIO_PROT,&GPIO_InitStructure);

    BEEP_OFF;
}
__IO uint8_t BEEP_State;
uint8_t BEEP_GetState(void)
{
    return BEEP_State;
}
void BEEP_Off(void)
{
    BEEP_State = BEEP_OFF_STATE;
    BEEP_OFF;
}
void BEEP_On(void)
{
    BEEP_State = BEEP_ON_STATE;
    BEEP_ON;
}
