#include "ptc.h"

void PTC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(PTC_CLK, ENABLE ); //≈‰÷√ ±÷”
    
    GPIO_InitStructure.GPIO_Pin = PTC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PTC_GPIO_PORT,&GPIO_InitStructure);

    PTC_OFF;
}

__IO uint8_t PTC_State = 0; 

void PTC_On(void) {
    PTC_ON;           
    PTC_State = 1;     
}

void PTC_Off(void) {
    PTC_OFF;
    PTC_State = 0;
}

uint8_t PTC_GetState(void) {
    return PTC_State;
}
