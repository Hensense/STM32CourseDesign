#ifndef __ASRPRO_H
#define __ASRPRO_H
#define ASRPRO_START_STATE     0  /* 0：禁用文件 1：使能文件 */
#if ASRPRO_START_STATE
#include "sys.h"

#define ASRPRO_USARTx   2  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if ASRPRO_USARTx == 1
    #define ASRPRO_USART_UX           USART1
    #define ASRPRO_USART_IRQn         USART1_IRQn
    #define ASRPRO_USART_IRQHandler   USART1_IRQHandler
    #define ASRPRO_GPIO_TX_PORT       GPIOA
    #define ASRPRO_GPIO_TX_PIN        GPIO_Pin_9
    #define ASRPRO_GPIO_RX_PORT       GPIOA
    #define ASRPRO_GPIO_RX_PIN        GPIO_Pin_10
    #define ASRPRO_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define ASRPRO_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif ASRPRO_USARTx == 2
    #define ASRPRO_USART_UX           USART2
    #define ASRPRO_USART_IRQn         USART2_IRQn
    #define ASRPRO_USART_IRQHandler   USART2_IRQHandler
    #define ASRPRO_GPIO_TX_PORT       GPIOA
    #define ASRPRO_GPIO_TX_PIN        GPIO_Pin_2
    #define ASRPRO_GPIO_RX_PORT       GPIOA
    #define ASRPRO_GPIO_RX_PIN        GPIO_Pin_3
    #define ASRPRO_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define ASRPRO_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif ASRPRO_USARTx == 3
    #define ASRPRO_USART_UX           USART3
    #define ASRPRO_USART_IRQn         USART3_IRQn
    #define ASRPRO_USART_IRQHandler   USART3_IRQHandler
    #define ASRPRO_GPIO_TX_PORT       GPIOB
    #define ASRPRO_GPIO_TX_PIN        GPIO_Pin_10
    #define ASRPRO_GPIO_RX_PORT       GPIOB
    #define ASRPRO_GPIO_RX_PIN        GPIO_Pin_11
    #define ASRPRO_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define ASRPRO_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

#define ASRPRO_RXMAX  150

typedef struct{
    uint8_t rxbuff[ASRPRO_RXMAX];       //接收数据缓存
    uint16_t rxcount;                   //接收的数量
}ASRPRO_Rec;

void ASRPRO_Init(void);
void ASRPRO_Printf(char *format, ...);
#endif

#endif
