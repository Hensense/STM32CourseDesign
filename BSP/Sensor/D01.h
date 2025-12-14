#ifndef __D01_H
#define __D01_H

#define D01_START_STATE   0   /* 1：启用本文件 0：禁用本文件 */
#include "sys.h"

#if D01_START_STATE
#define D01_USARTx   3      // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if D01_USARTx == 1
    #define D01_USART_UX           USART1
    #define D01_USART_IRQn         USART1_IRQn
    #define D01_USART_IRQHandler   USART1_IRQHandler
    #define D01_GPIO_TX_PORT       GPIOA
    #define D01_GPIO_TX_PIN        GPIO_Pin_9
    #define D01_GPIO_RX_PORT       GPIOA
    #define D01_GPIO_RX_PIN        GPIO_Pin_10
    #define D01_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define D01_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif D01_USARTx == 2
    #define D01_USART_UX           USART2
    #define D01_USART_IRQn         USART2_IRQn
    #define D01_USART_IRQHandler   USART2_IRQHandler
    #define D01_GPIO_TX_PORT       GPIOA
    #define D01_GPIO_TX_PIN        GPIO_Pin_2
    #define D01_GPIO_RX_PORT       GPIOA
    #define D01_GPIO_RX_PIN        GPIO_Pin_3
    #define D01_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define D01_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif D01_USARTx == 3
    #define D01_USART_UX           USART3
    #define D01_USART_IRQn         USART3_IRQn
    #define D01_USART_IRQHandler   USART3_IRQHandler
    #define D01_GPIO_TX_PORT       GPIOB
    #define D01_GPIO_TX_PIN        GPIO_Pin_10
    #define D01_GPIO_RX_PORT       GPIOB
    #define D01_GPIO_RX_PIN        GPIO_Pin_11
    #define D01_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define D01_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif


void D01_Init(void);
uint16_t D01_GetValue(void);

#endif

#endif
