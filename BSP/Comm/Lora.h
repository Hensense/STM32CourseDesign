#ifndef __LORA_H
#define __LORA_H
#define LORA_START_STATE     0  /* 0：禁用文件 1：使能文件 */
#if LORA_START_STATE
#include "sys.h"

#define LORA_USARTx   2  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if LORA_USARTx == 1
    #define LORA_USART_UX           USART1
    #define LORA_USART_IRQn         USART1_IRQn
    #define LORA_USART_IRQHandler   USART1_IRQHandler
    #define LORA_GPIO_TX_PORT       GPIOA
    #define LORA_GPIO_TX_PIN        GPIO_Pin_9
    #define LORA_GPIO_RX_PORT       GPIOA
    #define LORA_GPIO_RX_PIN        GPIO_Pin_10
    #define LORA_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define LORA_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif LORA_USARTx == 2
    #define LORA_USART_UX           USART2
    #define LORA_USART_IRQn         USART2_IRQn
    #define LORA_USART_IRQHandler   USART2_IRQHandler
    #define LORA_GPIO_TX_PORT       GPIOA
    #define LORA_GPIO_TX_PIN        GPIO_Pin_2
    #define LORA_GPIO_RX_PORT       GPIOA
    #define LORA_GPIO_RX_PIN        GPIO_Pin_3
    #define LORA_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define LORA_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif LORA_USARTx == 3
    #define LORA_USART_UX           USART3
    #define LORA_USART_IRQn         USART3_IRQn
    #define LORA_USART_IRQHandler   USART3_IRQHandler
    #define LORA_GPIO_TX_PORT       GPIOB
    #define LORA_GPIO_TX_PIN        GPIO_Pin_10
    #define LORA_GPIO_RX_PORT       GPIOB
    #define LORA_GPIO_RX_PIN        GPIO_Pin_11
    #define LORA_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define LORA_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

#define LORA_RXMAX  150

typedef struct{
    uint8_t rxbuff[LORA_RXMAX];       //接收数据缓存
    uint16_t rxcount;                   //接收的数量
}LORA_Rec;

void LORA_Init(void);
void LORA_Printf(char *format, ...);
#endif

#endif
