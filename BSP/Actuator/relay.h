#ifndef __RELAY_H
#define __RELAY_H
#include "sys.h"
/* 测试情况：本文件已完成测试，功能一切正常 */
//OLED_Printf(0,0,OLED_8X16,"Relay1:%01d",Relay_Ctrler.RunState(RELAY_ONE));
//OLED_Printf(0,16,OLED_8X16,"Relay2:%01d",Relay_Ctrler.RunState(RELAY_TWO));
typedef enum{
    RELAY_ONE = 1,
    RELAY_TWO = 2,
    RELAY_THREE = 3,
    RELAY_FOUR  = 4,
}RELAY_Number;
#define RELAY_ON_STATE      1
#define RELAY_OFF_STATE     0
/*************** 用户配置区 ***************/
#define RELAY_NUM  2  // 设置继电器数量（1~4）

/* 继电器1配置 */
#define RELAY1_CLK        RCC_APB2Periph_GPIOA  // 加热
#define RELAY1_GPIO_PORT  GPIOA
#define RELAY1_GPIO_PIN   GPIO_Pin_2
#define RELAY1_ON()       GPIO_SetBits(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN)
#define RELAY1_OFF()      GPIO_ResetBits(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN)
#if RELAY_NUM >= 2
/* 继电器2配置 */
#define RELAY2_CLK        RCC_APB2Periph_GPIOA  // 风扇
#define RELAY2_GPIO_PORT  GPIOA
#define RELAY2_GPIO_PIN   GPIO_Pin_3
#define RELAY2_ON()       GPIO_SetBits(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN)
#define RELAY2_OFF()      GPIO_ResetBits(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN)
#endif

/* 继电器3配置（当RELAY_NUM >= 3时启用） */
#if RELAY_NUM >= 3
#define RELAY3_CLK        RCC_APB2Periph_GPIOB
#define RELAY3_GPIO_PORT  GPIOB
#define RELAY3_GPIO_PIN   GPIO_Pin_14
#define RELAY3_ON()       GPIO_SetBits(RELAY3_GPIO_PORT, RELAY3_GPIO_PIN)
#define RELAY3_OFF()      GPIO_ResetBits(RELAY3_GPIO_PORT, RELAY3_GPIO_PIN)
#endif

/* 继电器4配置（当RELAY_NUM >= 4时启用） */
#if RELAY_NUM >= 4
#define RELAY4_CLK        RCC_APB2Periph_GPIOB
#define RELAY4_GPIO_PORT  GPIOB
#define RELAY4_GPIO_PIN   GPIO_Pin_15
#define RELAY4_ON()       GPIO_SetBits(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN)
#define RELAY4_OFF()      GPIO_ResetBits(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN)
#endif
/*************** END ***************/

// 继电器状态结构体
typedef struct {
    uint8_t Relay1 : 1; //位域
    uint8_t Relay2 : 1;
    uint8_t Relay3 : 1;
    uint8_t Relay4 : 1;
} RelayState_TypeDef;

// 函数声明
void RELAY_Init(void);
void RELAY_On(RELAY_Number relay_num);
void RELAY_Off(RELAY_Number relay_num);
uint8_t RELAY_GetState(RELAY_Number relay_num);

#endif /* __RELAY_H */


