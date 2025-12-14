#include "bump.h"

void BUMP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUMP_CLK, ENABLE ); //≈‰÷√ ±÷”
    
    GPIO_InitStructure.GPIO_Pin = BUMP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(BUMP_GPIO_PROT,&GPIO_InitStructure);

    BUMP_OFF;
}
__IO uint8_t BUMP_State;
void BUMP_On(void)
{
    BUMP_ON;
    BUMP_State = 1;
}
void BUMP_Off(void)
{
    BUMP_OFF;
    BUMP_State = 0;
}
uint8_t BUMP_GetState(void)
{
    return BUMP_State;
}
