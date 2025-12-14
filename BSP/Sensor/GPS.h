#ifndef __GPS_H
#define __GPS_H
#define GPS_START_STATE     0   /* 0:禁用文件 1:启用文件 */
#if GPS_START_STATE
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
// 用户配置区 - 只需修改此处即可选择串口
#define GPS_USARTx  3  // 选择USART端口：1,2,3

// 根据宏定义自动选择对应的GPIO和中断
#if GPS_USARTx == 1
    #define GPS_USART_UX           USART1
    #define GPS_USART_IRQn         USART1_IRQn
    #define GPS_USART_IRQHandler   USART1_IRQHandler
    #define GPS_GPIO_TX_PORT       GPIOA
    #define GPS_GPIO_TX_PIN        GPIO_Pin_9
    #define GPS_GPIO_RX_PORT       GPIOA
    #define GPS_GPIO_RX_PIN        GPIO_Pin_10
    #define GPS_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define GPS_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif GPS_USARTx == 2
    #define GPS_USART_UX           USART2
    #define GPS_USART_IRQn         USART2_IRQn
    #define GPS_USART_IRQHandler   USART2_IRQHandler
    #define GPS_GPIO_TX_PORT       GPIOA
    #define GPS_GPIO_TX_PIN        GPIO_Pin_2
    #define GPS_GPIO_RX_PORT       GPIOA
    #define GPS_GPIO_RX_PIN        GPIO_Pin_3
    #define GPS_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define GPS_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif GPS_USARTx == 3
    #define GPS_USART_UX           USART3
    #define GPS_USART_IRQn         USART3_IRQn
    #define GPS_USART_IRQHandler   USART3_IRQHandler
    #define GPS_GPIO_TX_PORT       GPIOB
    #define GPS_GPIO_TX_PIN        GPIO_Pin_10
    #define GPS_GPIO_RX_PORT       GPIOB
    #define GPS_GPIO_RX_PIN        GPIO_Pin_11
    #define GPS_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define GPS_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif

// GPS模块配置
#define GPS_USART_REC_LEN  100      // 定义最大接收字节数
#define EN_GPS_RX           1       // 使能（1）/禁止（0）GPS接收

#define false 0
#define true 1
    
// 定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct GPS_SaveData 
{
    char GPS_Buffer[GPS_Buffer_Length];
    char isGetData;                     // 是否获取到GPS数据
    char isParseData;                   // 是否解析完成
    char UTCTime[UTCTime_Length];       // UTC时间
    char latitude[latitude_Length];     // 纬度
    char N_S[N_S_Length];               // N/S
    char longitude[longitude_Length];   // 经度
    char E_W[E_W_Length];               // E/W
    char isUsefull;                     // 定位信息是否有效
} GPS_SaveData;

// 函数声明
void GPS_Init(void);
void GPS_GetPosition(float * latitude, float *longitude);

#endif

#endif
