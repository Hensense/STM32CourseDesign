#ifndef __KEY_H
#define __KEY_H
#include "main.h"

/* 按键端口和引脚宏定义 - 可根据实际硬件修改 */
#define KEY1_CLK        RCC_APB2Periph_GPIOA
#define KEY1_PORT       GPIOA
#define KEY1_PIN        GPIO_Pin_0

#define KEY2_CLK        RCC_APB2Periph_GPIOC
#define KEY2_PORT       GPIOC
#define KEY2_PIN        GPIO_Pin_15

#define KEY3_CLK        RCC_APB2Periph_GPIOC
#define KEY3_PORT       GPIOC
#define KEY3_PIN        GPIO_Pin_14

#define KEY4_CLK        RCC_APB2Periph_GPIOC
#define KEY4_PORT       GPIOC
#define KEY4_PIN        GPIO_Pin_13

/* 键值枚举 */
enum{
    KEY1_PRESS = 1,
    KEY2_PRESS = 2,
    KEY3_PRESS = 3,
    KEY4_PRESS = 4
};

/* 函数声明 */
void Key_Init(void);    /* KEY初始化 */
void Key_Tick(void);    /* KEY检测 需要1ms调用一次 */
uint8_t Key_Scan(void); /* KEY扫描 */

#endif
