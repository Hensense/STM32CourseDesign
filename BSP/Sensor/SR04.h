#ifndef __SR04_H
#define __SR04_H
#include "sys.h"
#define SR04_START_STATE    0   /* 0:禁用文件 1:启动文件 */
#if SR04_START_STATE
/*************** 用户配置区 - 根据实际硬件连接修改 ***************/
// 触发引脚配置
#define TRIG_PORT    GPIOA
#define TRIG_PIN     GPIO_Pin_0
#define TRIG_RCC     RCC_APB2Periph_GPIOA

// 回波引脚配置
#define ECHO_PORT    GPIOA
#define ECHO_PIN     GPIO_Pin_1
#define ECHO_RCC     RCC_APB2Periph_GPIOA

// 定时器配置
#define SR04_TIM               TIM2
#define SR04_TIM_RCC           RCC_APB1Periph_TIM2
#define SR04_TIM_IRQn          TIM2_IRQn
#define SR04_TIM_IRQHandler    TIM2_IRQHandler
#define SR04_TIM_CHANNEL       TIM_Channel_2
/*************** END ***************/

// 定义触发引脚操作
#define TRIG_OUT(x)   GPIO_WriteBit(TRIG_PORT, TRIG_PIN, (BitAction)(x))

// 状态变量声明
extern uint8_t capture_status;
extern uint16_t capture_value;

void SR04_Init(uint16_t arr, uint16_t psc);  /* SR04初始化函数 */
uint32_t SR04_GetDistance(void);            /* SR04获取前方障碍物的距离 */

#endif
#endif
