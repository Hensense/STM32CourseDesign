#ifndef __JR6001_H
#define __JR6001_H
#include "sys.h"
/* 测试情况：本文件已完成测试，功能一切正常 */

#define JR6001_USARTx   3  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if JR6001_USARTx == 1
    #define JR6001_USART_UX           USART1
    #define JR6001_USART_IRQn         USART1_IRQn
    #define JR6001_USART_IRQHandler   USART1_IRQHandler
    #define JR6001_GPIO_TX_PORT       GPIOA
    #define JR6001_GPIO_TX_PIN        GPIO_Pin_9
    #define JR6001_GPIO_RX_PORT       GPIOA
    #define JR6001_GPIO_RX_PIN        GPIO_Pin_10
    #define JR6001_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define JR6001_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JR6001_USARTx == 2
    #define JR6001_USART_UX           USART2
    #define JR6001_USART_IRQn         USART2_IRQn
    #define JR6001_USART_IRQHandler   USART2_IRQHandler
    #define JR6001_GPIO_TX_PORT       GPIOA
    #define JR6001_GPIO_TX_PIN        GPIO_Pin_2
    #define JR6001_GPIO_RX_PORT       GPIOA
    #define JR6001_GPIO_RX_PIN        GPIO_Pin_3
    #define JR6001_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define JR6001_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JR6001_USARTx == 3
    #define JR6001_USART_UX           USART3
    #define JR6001_USART_IRQn         USART3_IRQn
    #define JR6001_USART_IRQHandler   USART3_IRQHandler
    #define JR6001_GPIO_TX_PORT       GPIOB
    #define JR6001_GPIO_TX_PIN        GPIO_Pin_10
    #define JR6001_GPIO_RX_PORT       GPIOB
    #define JR6001_GPIO_RX_PIN        GPIO_Pin_11
    #define JR6001_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define JR6001_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

void JR6001_Init(void);
void JR6001_StopPlay(void);
void JR6001_PlayMusic(uint32_t num);
void JR6001_SetVolume(uint8_t volume);

#endif
