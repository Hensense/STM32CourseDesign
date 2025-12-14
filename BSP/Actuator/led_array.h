#ifndef __LED_ARRAY_H
#define __LED_ARRAY_H

#include "sys.h"

// LED编号枚举
typedef enum{
    LED_ONE = 1,
    LED_TWO = 2,
    LED_THREE = 3,
    LED_FOUR  = 4,
}LED_Number;


/*************** 用户配置区 ***************/
#define LED_NUM  2  // 设置LED数量（1~4）

#define LED_ON_STATE    1   /* LED打开时引脚状态 根据实际原理图进行配置 */
#define LED_OFF_STATE   0

/* LED1配置 */
#define LED1_CLK         RCC_APB2Periph_GPIOA   //照明
#define LED1_GPIO_PORT   GPIOA
#define LED1_GPIO_PIN    GPIO_Pin_5
#define LED1_ON()        GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF()       GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)

#if LED_NUM >= 2
/* LED2配置 */
#define LED2_CLK         RCC_APB2Periph_GPIOA   //紫外线
#define LED2_GPIO_PORT   GPIOA
#define LED2_GPIO_PIN    GPIO_Pin_6
#define LED2_ON()        GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF()       GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN)
#endif

#if LED_NUM >= 3
/* LED3配置 */
#define LED3_CLK         RCC_APB2Periph_GPIOA
#define LED3_GPIO_PORT   GPIOA
#define LED3_GPIO_PIN    GPIO_Pin_2
#define LED3_ON()        GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF()       GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN)
#endif

#if LED_NUM >= 4
/* LED4配置 */
#define LED4_CLK         RCC_APB2Periph_GPIOA
#define LED4_GPIO_PORT   GPIOA
#define LED4_GPIO_PIN    GPIO_Pin_3
#define LED4_ON()        GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_OFF()       GPIO_ResetBits(LED4_GPIO_PORT, LED4_GPIO_PIN)
#endif


/*************** END ***************/

// LED状态结构体
typedef struct {
    uint8_t LED1 : 1; // 位域存储每个LED的状态
    uint8_t LED2 : 1;
    uint8_t LED3 : 1;
    uint8_t LED4 : 1;
} LEDState_TypeDef;

void LED_Array_Init(void);                     /* 初始化LED */
void LED_Array_On(LED_Number led_num);         /* 开启指定LED */
void LED_Array_Off(LED_Number led_num);        /* 关闭指定LED */
void LED_Array_Toggle(LED_Number led_num);     /* 切换指定LED状态 */
uint8_t LED_Array_GetState(LED_Number led_num);/* 获取指定LED状态 */
#endif /* __LED_MATRIX_H */
