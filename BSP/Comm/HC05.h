#ifndef __HC05_H
#define __HC05_H
#define HC05_START_STATE     0  /* 0：禁用文件 1：使能文件 */
#if HC05_START_STATE
#include "sys.h"

#define HC05_USARTx   2  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if HC05_USARTx == 1
    #define HC05_USART_UX           USART1
    #define HC05_USART_IRQn         USART1_IRQn
    #define HC05_USART_IRQHandler   USART1_IRQHandler
    #define HC05_GPIO_TX_PORT       GPIOA
    #define HC05_GPIO_TX_PIN        GPIO_Pin_9
    #define HC05_GPIO_RX_PORT       GPIOA
    #define HC05_GPIO_RX_PIN        GPIO_Pin_10
    #define HC05_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define HC05_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif HC05_USARTx == 2
    #define HC05_USART_UX           USART2
    #define HC05_USART_IRQn         USART2_IRQn
    #define HC05_USART_IRQHandler   USART2_IRQHandler
    #define HC05_GPIO_TX_PORT       GPIOA
    #define HC05_GPIO_TX_PIN        GPIO_Pin_2
    #define HC05_GPIO_RX_PORT       GPIOA
    #define HC05_GPIO_RX_PIN        GPIO_Pin_3
    #define HC05_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define HC05_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif HC05_USARTx == 3
    #define HC05_USART_UX           USART3
    #define HC05_USART_IRQn         USART3_IRQn
    #define HC05_USART_IRQHandler   USART3_IRQHandler
    #define HC05_GPIO_TX_PORT       GPIOB
    #define HC05_GPIO_TX_PIN        GPIO_Pin_10
    #define HC05_GPIO_RX_PORT       GPIOB
    #define HC05_GPIO_RX_PIN        GPIO_Pin_11
    #define HC05_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define HC05_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

#define HC05_RXMAX  50

typedef struct{
    uint8_t rxbuff[HC05_RXMAX];     //接收数据缓存
    uint16_t rxcount;               //接收的数量
}HC05_Rec;

void HC05_Init(void);
void HC05_Printf(char *format, ...);
#endif

#endif
