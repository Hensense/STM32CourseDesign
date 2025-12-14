#ifndef __JQ6500_H
#define __JQ6500_H

#include "sys.h"
#include <stdio.h>

#define JQ6500_USARTx   3  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if JQ6500_USARTx == 1
    #define JQ6500_USART_UX           USART1
    #define JQ6500_USART_IRQn         USART1_IRQn
    #define JQ6500_USART_IRQHandler   USART1_IRQHandler
    #define JQ6500_GPIO_TX_PORT       GPIOA
    #define JQ6500_GPIO_TX_PIN        GPIO_Pin_9
    #define JQ6500_GPIO_RX_PORT       GPIOA
    #define JQ6500_GPIO_RX_PIN        GPIO_Pin_10
    #define JQ6500_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define JQ6500_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JQ6500_USARTx == 2
    #define JQ6500_USART_UX           USART2
    #define JQ6500_USART_IRQn         USART2_IRQn
    #define JQ6500_USART_IRQHandler   USART2_IRQHandler
    #define JQ6500_GPIO_TX_PORT       GPIOA
    #define JQ6500_GPIO_TX_PIN        GPIO_Pin_2
    #define JQ6500_GPIO_RX_PORT       GPIOA
    #define JQ6500_GPIO_RX_PIN        GPIO_Pin_3
    #define JQ6500_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define JQ6500_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif JQ6500_USARTx == 3
    #define JQ6500_USART_UX           USART3
    #define JQ6500_USART_IRQn         USART3_IRQn
    #define JQ6500_USART_IRQHansdler   USART3_IRQHandler
    #define JQ6500_GPIO_TX_PORT       GPIOB
    #define JQ6500_GPIO_TX_PIN        GPIO_Pin_10
    #define JQ6500_GPIO_RX_PORT       GPIOB
    #define JQ6500_GPIO_RX_PIN        GPIO_Pin_11
    #define JQ6500_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define JQ6500_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

void JQ6500_Init(void);
void JQ6500_Play(uint8_t dat);

#endif
