#include "jsq.h"

void JSQ_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(JSQ_CLK, ENABLE ); //≈‰÷√ ±÷”
    
    GPIO_InitStructure.GPIO_Pin = JSQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(JSQ_GPIO_PORT,&GPIO_InitStructure);

    JSQ_OFF;
}
__IO uint8_t JSQ_State = 0;
void JSQ_On(void)
{
    JSQ_ON;
    JSQ_State = 1;
}
void JSQ_Off(void)
{
    JSQ_OFF;
    JSQ_State = 0;
}
uint8_t JSQ_GetState(void)
{
    return JSQ_State;
}
