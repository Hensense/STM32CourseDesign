#include "LED.h"
/*
*********************************************************************************************************
*   文件说明:本文件控制的LED为板载LED，控制引脚为PC13
*********************************************************************************************************
*/

/**
 * @brief   LED初始化函数 初始化引脚PA7（板载LED）
 * @param   无
 * @retval  无
 */
void LED_Init(void)
{
    /* 1.使能PC端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    /* 2.配置PC13引脚 GPIO工作模式：推挽 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /* 3.初始化PC13引脚 */
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
    LED_OFF();
}
/**
 * @brief   点亮LED
 * @param   无
 * @retval  无
 */
void LED_ON(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}
/**
 * @brief   关闭LED灯
 * @param   无
 * @retval  无
 */
void LED_OFF(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
/**
 * @brief   切换LED灯的状态
 * @param   无
 * @retval  无
 */
void LED_Turn(void)
{ 
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == 0)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    }
}

