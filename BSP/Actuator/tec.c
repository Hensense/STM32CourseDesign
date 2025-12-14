#include "tec.h"

void TEC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(TEC_CLK, ENABLE ); //≈‰÷√ ±÷”
    
    GPIO_InitStructure.GPIO_Pin = TEC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(TEC_GPIO_PORT,&GPIO_InitStructure);

    TEC_OFF;
}
__IO uint8_t TEC_State = 0;

void TEC_On(void) {
    TEC_ON;
    TEC_State = 1;
}

void TEC_Off(void) {
    TEC_OFF;
    TEC_State = 0;
}

uint8_t TEC_GetState(void) {
    return TEC_State;
}
