#ifndef __ZIGBEE_H
#define __ZIGBEE_H
#define ZIGBEE_START_STATE     0  /* 0：禁用文件 1：使能文件 */
#if ZIGBEE_START_STATE
#include "sys.h"

#define ZIGBEE_USARTx   2  // 可选：USART1, USART2, USART3

// 根据宏定义自动选择对应的GPIO和中断
#if ZIGBEE_USARTx == 1
    #define ZIGBEE_USART_UX           USART1
    #define ZIGBEE_USART_IRQn         USART1_IRQn
    #define ZIGBEE_USART_IRQHandler   USART1_IRQHandler
    #define ZIGBEE_GPIO_TX_PORT       GPIOA
    #define ZIGBEE_GPIO_TX_PIN        GPIO_Pin_9
    #define ZIGBEE_GPIO_RX_PORT       GPIOA
    #define ZIGBEE_GPIO_RX_PIN        GPIO_Pin_10
    #define ZIGBEE_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define ZIGBEE_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif ZIGBEE_USARTx == 2
    #define ZIGBEE_USART_UX           USART2
    #define ZIGBEE_USART_IRQn         USART2_IRQn
    #define ZIGBEE_USART_IRQHandler   USART2_IRQHandler
    #define ZIGBEE_GPIO_TX_PORT       GPIOA
    #define ZIGBEE_GPIO_TX_PIN        GPIO_Pin_2
    #define ZIGBEE_GPIO_RX_PORT       GPIOA
    #define ZIGBEE_GPIO_RX_PIN        GPIO_Pin_3
    #define ZIGBEE_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define ZIGBEE_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif ZIGBEE_USARTx == 3
    #define ZIGBEE_USART_UX           USART3
    #define ZIGBEE_USART_IRQn         USART3_IRQn
    #define ZIGBEE_USART_IRQHandler   USART3_IRQHandler
    #define ZIGBEE_GPIO_TX_PORT       GPIOB
    #define ZIGBEE_GPIO_TX_PIN        GPIO_Pin_10
    #define ZIGBEE_GPIO_RX_PORT       GPIOB
    #define ZIGBEE_GPIO_RX_PIN        GPIO_Pin_11
    #define ZIGBEE_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define ZIGBEE_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

#define ZIGBEE_RXMAX  150

typedef struct{
    uint8_t rxbuff[ZIGBEE_RXMAX];       //接收数据缓存
    uint16_t rxcount;                   //接收的数量
}ZIGBEE_Rec;

void ZIGBEE_Init(void);
void ZIGBEE_Printf(char *format, ...);
#endif

#endif
