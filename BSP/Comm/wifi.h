#ifndef __WIFI_H
#define __WIFI_H
#define WIFI_NETWORK_START_STATE     0   /* 0：禁用文件 1：使能文件 */
#if WIFI_NETWORK_START_STATE
/* ***************************头文件引入 开始******************************* */
#include "sys.h"
/* ***************************头文件引入 结束******************************* */

/* ***************************宏定义 开始*********************************** */
/* 网络连接参数 */
#define WIFI_SSID               "boy"               /* ESP01S需要连接的WIFI名称 2.4G */
#define WIFI_PASSWORD           "123456789"         /* ESP01S需要连接的WIFI密码 */
#define WIFI_ONENET_PRODUCT_ID  "nTH6ND93fp"        /* ONENET平台产品ID */
#define WIFI_ONENET_DEVICE_NAME "kcdz_Device"       /* ONENET平台设备名 */
/* TOKEN工具生成的TOKEN */
#define WIFI_ONENET_TOKEN       "version=2018-10-31&res=products%2FnTH6ND93fp%2Fdevices%2Fkcdz_Device&et=1819783517&method=sha1&sign=uzb%2FBwoB%2B7a%2B7xJzqr7GxroIWQ8%3D"
/* Onenet mqtt服务器地址 */
#define MQTT_HOST_URL           "mqtts.heclouds.com"/* MQTT服务器主机地址 */
#define MQTT_HOST_PORT          1883                /* MQTT服务器端口号 */
/* AT指令 定义 */
#define WIFI_ONENET_REVEIVE_SET_TOPIC           "\"$sys/" WIFI_ONENET_PRODUCT_ID "/" WIFI_ONENET_DEVICE_NAME "/thing/property/set\""        /* 接收到ONENET云平台下发设置命令 */
#define WIFI_ONENET_SEND_SET_TOPIC_RESPONSE     "\"$sys/" WIFI_ONENET_PRODUCT_ID "/" WIFI_ONENET_DEVICE_NAME "/thing/property/set_reply\""  /* 设置命令响应指令 */
/* **用户配置区 开始** */
#define WIFI_USARTx   1      /* 可选：USART1, USART2, USART3 */

#if WIFI_USARTx == 1
    #define WIFI_USART_UX           USART1
    #define WIFI_USART_IRQn         USART1_IRQn
    #define WIFI_USART_IRQHandler   USART1_IRQHandler
    #define WIFI_USART_CLK          RCC_APB2Periph_USART1
    #define WIFI_GPIO_TX_PORT       GPIOA
    #define WIFI_GPIO_TX_PIN        GPIO_Pin_9
    #define WIFI_GPIO_RX_PORT       GPIOA
    #define WIFI_GPIO_RX_PIN        GPIO_Pin_10
    #define WIFI_GPIO_CLK           RCC_APB2Periph_GPIOA
#elif WIFI_USARTx == 2
    #define WIFI_USART_UX           USART2
    #define WIFI_USART_IRQn         USART2_IRQn
    #define WIFI_USART_IRQHandler   USART2_IRQHandler
    #define WIFI_GPIO_TX_PORT       GPIOA
    #define WIFI_GPIO_TX_PIN        GPIO_Pin_2
    #define WIFI_GPIO_RX_PORT       GPIOA
    #define WIFI_GPIO_RX_PIN        GPIO_Pin_3
    #define WIFI_USART_CLK          RCC_APB1Periph_USART2
    #define WIFI_GPIO_CLK           RCC_APB2Periph_GPIOA
#elif WIFI_USARTx == 3
    #define WIFI_USART_UX           USART3
    #define WIFI_USART_IRQn         USART3_IRQn
    #define WIFI_USART_IRQHandler   USART3_IRQHandler
    #define WIFI_GPIO_TX_PORT       GPIOB
    #define WIFI_GPIO_TX_PIN        GPIO_Pin_10
    #define WIFI_GPIO_RX_PORT       GPIOB
    #define WIFI_GPIO_RX_PIN        GPIO_Pin_11
    #define WIFI_USART_CLK          RCC_APB1Periph_USART3
    #define WIFI_GPIO_CLK           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif
#define WIFI_USART_RXBUFF_MAX   512     /* 接收数据最大缓冲区 */
#define WIFI_USART_TXBUFF_MAX   512     /* 发送数据最大缓冲区 */

/* **用户配置区 结束** */
#define WIFI_FALSE              0
#define WIFI_OK                 1

#define WIFI_REV_WAIT           0
#define WIFI_REV_OK             1

#define WIFI_OLED_LOG_PRINTF    1   /* WIFI初始化过程中是否使能OLED打印连接进度 0：禁用 1：使能*/
/* ***************************宏定义 结束*********************************** */

/* ***************************枚举类型定义 开始***************************** */

/* ***************************枚举类型定义 结束***************************** */

/* ***************************共用体类型定义 开始*************************** */
typedef union{
    struct{
        uint8_t Wifi_Connect    :1; /* false:WIFI未连接    true:WIFI已连接 */
        uint8_t Onenet_Connect  :1; /* false:Onenet未连接  true:Onenet已连接 */
        uint8_t Publish         :1; /* false:未发布数据/发布失败  true:发布数据成功 */
        uint8_t Subscribe_GetReply  :1;/* 订阅设备属性上报响应主题 false:未订阅 true :已订阅 */
        uint8_t Subscribe_SetTopic  :1;/* 订阅设备属性设置请求 false:未订阅 true :已订阅*/
        uint8_t Reserved        :3; /* 保留位，未使用 */
    }Bits;
    uint8_t Status;
}WIFI_Status;           /* WIFI状态 */

typedef union{
    struct{
        uint32_t AT_RST_ERROR           :1;/* WIFI模块复位错误 */
        uint32_t AT_ATE0_ERROR          :1;/* WIFI模块关闭回显错误 */
        uint32_t AT_CWMODE_ERROR        :1;/* WIFI模块设置为客户端错误 */
        uint32_t AT_CWJAP_ERROR         :1;/* WIFI模块连接WIFI热点错误 */
        uint32_t AT_MQTTUSERCFG_ERROR   :1;/* WIFI模块配置MQTT连接参数错误 */
        uint32_t AT_MQTTCONN_ERROR      :1;/* WIFI模块连接MQTT服务器错误 */
        uint32_t AT_MQTTSUB_ERROR       :1;/* WIFI模块订阅主题错误 */
        uint32_t AT_Reserved            :25;/* 保留位 */
    }Bits;
    uint32_t Bytes;
}ERROR_Code;            /* AT响应错误代码 */
/* ***************************共用体类型定义 结束*************************** */

/* ***************************结构体类型定义 开始*************************** */
typedef void (*CallbackFunc)(void);

typedef struct{
    uint32_t    Rx_Length;
    uint8_t     Rx_Buff[WIFI_USART_RXBUFF_MAX];
    uint32_t    Tx_Length;
    uint8_t     Tx_Buff[WIFI_USART_TXBUFF_MAX];
    
    uint8_t     Rx_DataFlag;    /* 接收数据完成标志位 */
    uint8_t     Update_Property;/* 更新物模型标志 */
    uint32_t    Response_Id;    /* 接收Onenet控制命令后的响应数据ID */
    uint8_t     Response_Flag;  /* 需要发送响应数据 */
    
    WIFI_Status Connect;        /* WIFI连接状态 */
    ERROR_Code  Error_Code;     /* AT响应错误代码 */  
}WIFI_Handler;  /* WIFI模块控制器 */
/* ***************************结构体类型定义 结束*************************** */

/* ***************************外部变量声明 开始***************************** */
extern WIFI_Handler Wifi_Handler;   /* 提供给外部使用 */
/* ***************************外部变量声明 结束***************************** */

/* ***************************函数接口声明 开始***************************** */
/* 初始化与配置 */
void WIFI_Init(void);                         // 模块初始化
void WIFI_UARTInit(void);                     // 串口初始化

/* 数据接收与解析 */
void WIFI_ClearRxBuffer(void);                // 清空接收缓冲区
uint8_t WIFI_GetRxCompleteFlag(void);         // 获取接收完成标志
void WIFI_ParseRxData(void);                  // 解析接收数据

/* 数据发送 */
void WIFI_SendString(char *str);              // 发送字符串
uint8_t WIFI_SendATCommand(char* command, char *response, uint8_t retry, uint16_t delay_xms); // 发送AT指令

/* 连接状态 */
uint8_t WIFI_GetWifiConnectionStatus(void);     // WIFI连接状态
uint8_t WIFI_GetMQTTConnectionStatus(void);     // MQTT服务器连接状态

/* Onenet 云平台交互 */
uint8_t WIFI_GetOnenetCommand(void);            // 检测Onenet命令
uint8_t WIFI_GetOnenetResponseCommand(void);    // 检测发布的命令是否是Onenet响应命令
void Onenet_SetPacket(void);                    // 声明Onenet_SetPacket
void WIFI_ProcessOnenetCommand(void);           // 处理Onenet命令
void WIFI_Publish_Property(CallbackFunc Onenet_SetPacket);   // 上报长数据到Onenet
void WIFI_SendOnenetCommandResponse(uint32_t response_id, uint16_t response_code, char* response_msg); // 发送命令响应
/* ***************************函数接口声明 结束***************************** */

#endif

#endif
