#ifndef __JW01_H
#define __JW01_H

#define JW01_START_STATE   0   /* 1：启用本文件 0：禁用本文件 */
#include "sys.h"

#if JW01_START_STATE
#define JW01_USARTx   3     /* 可选：USART1, USART2, USART3 */

// 根据宏定义自动选择对应的GPIO和中断
#if JW01_USARTx == 1
    #define JW01_USART_UX           USART1
    #define JW01_USART_IRQn         USART1_IRQn
    #define JW01_USART_IRQHandler   USART1_IRQHandler
    #define JW01_GPIO_TX_PORT       GPIOA
    #define JW01_GPIO_TX_PIN        GPIO_Pin_9
    #define JW01_GPIO_RX_PORT       GPIOA
    #define JW01_GPIO_RX_PIN        GPIO_Pin_10
    #define JW01_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define JW01_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JW01_USARTx == 2
    #define JW01_USART_UX           USART2
    #define JW01_USART_IRQn         USART2_IRQn
    #define JW01_USART_IRQHandler   USART2_IRQHandler
    #define JW01_GPIO_TX_PORT       GPIOA
    #define JW01_GPIO_TX_PIN        GPIO_Pin_2
    #define JW01_GPIO_RX_PORT       GPIOA
    #define JW01_GPIO_RX_PIN        GPIO_Pin_3
    #define JW01_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define JW01_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JW01_USARTx == 3
    #define JW01_USART_UX           USART3
    #define JW01_USART_IRQn         USART3_IRQn
    #define JW01_USART_IRQHandler   USART3_IRQHandler
    #define JW01_GPIO_TX_PORT       GPIOB
    #define JW01_GPIO_TX_PIN        GPIO_Pin_10
    #define JW01_GPIO_RX_PORT       GPIOB
    #define JW01_GPIO_RX_PIN        GPIO_Pin_11
    #define JW01_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define JW01_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

void JW01_Init(void);
void JW01_GetCo2(uint16_t *data);
#endif

#endif
