#ifndef __COMM_TASK_H
#define __COMM_TASK_H

/* ***************************头文件引入 开始******************************* */
#include "main.h"
#include <stdbool.h>
/* ***************************头文件引入 结束******************************* */

/* ***************************宏定义 开始*********************************** */
/* _enable_:使能某个通信模块 _disable_:失能某个通信模块 */
#define IR_REMOTE_ENABLE        _disable_    /* 红外遥控信号接收：需要一个IO引脚和一个TIMx */
#define WIFI_ENABLE             _disable_    /* ESP01S WIFI模块：需要一个串口 USART1/2/3均可 */
#define HC05_ENABLE             _disable_    /* HC05 蓝牙模块：需要一个串口 USART1/2/3均可 */
#define ZIGBEE_ENABLE           _disable_    /* ZIGBEE通信模块：需要一个串口 USART1/2/3均可 */
#define LORA_ENABLE             _disable_    /* LORA通信模块：需要一个串口 USART1/2/3均可 */
#define NRF24L01_ENABLE         _disable_    /* NRF24L01射频通信模块：SPI通信，需要任意四个IO引脚和一个CE控制IO引脚 */
/* ***************************宏定义 结束*********************************** */

/* ***************************预编译 开始*********************************** */
/* 注意：标志类变量使用后需要清除 */
#if WIFI_ENABLE  ==  _enable_
#include "wifi.h"
#define WIFI_RUN_SPACE          1   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    /* 接收部分 */
    __IO uint8_t Update_Flag;           /* ESP01S接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Led_State;             /* 接收云端发送的控制信息 */
    __IO uint8_t Led_State_Flag;        /* LED控制状态更新标志 */
    __IO uint8_t Fan_State;             /* 接收云端发送的控制信息 */
    __IO uint8_t Fan_State_Flag;        /* 风扇控制状态更新标志 */
    __IO uint8_t Humidifier_State;      /* 接收云端发送的控制信息 */
    __IO uint8_t Humidifier_State_Flag; /* 加湿器控制状态更新标志 */
    __IO uint8_t Temp_Limit_Up;         /* 温度上限值 */
    __IO uint8_t Humi_Limit_Down;       /* 湿度下限值 */
    //......后续信息需要根据云平台下发信息定义
}WIFI_Operations;
extern WIFI_Operations WIFI_Ctrler;
#endif
#if HC05_ENABLE  ==  _enable_
#include "HC05.h"
#define HC05_RUN_SPACE          1   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    __IO uint8_t Update_Flag;    /* HC05接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Led_State;      /* 接收云端发送的控制信息 */
    __IO uint8_t Led_State_Flag; /* LED控制状态更新标志 */
    //......后续信息需要根据云平台下发信息定义
    void (*Send)(char*, ...);
}HC05_DataStruct;
extern HC05_DataStruct HC05_DataInfo;
#endif
#if NRF24L01_ENABLE  ==  _enable_
#include "NRF24L01.h"
#define NRF24L01_RUN_SPACE      2   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    uint8_t ReceiveFlag;            /* NRF24L01接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    uint8_t SendFlag;               /* NRF24L01发送完一帧数据 返回发送结果 */
    uint8_t RxPacket[NRF24L01_RX_PACKET_WIDTH]; /* 发送数据包缓冲区 */
    uint8_t TxPacket[NRF24L01_TX_PACKET_WIDTH]; /* 接收数据包缓冲区 */
    uint8_t (*Send)(uint8_t*, uint8_t);         /* 发送指定长度数据包 */
    uint8_t (*Receive)(uint8_t*, uint8_t);      /* 接收指定长度数据包 */
}NRF24L01_Operations;
extern NRF24L01_Operations NRF24L01_Ctrler;
#endif
#if ZIGBEE_ENABLE  ==  _enable_
#include "Zigbee.h"
#define ZIGBEE_RUN_SPACE        2   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    __IO uint8_t Update_Flag;       /* ZIGBEE接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Temp;              /* 接收B端发送过来的温度数据 */
    __IO uint8_t Humi;              /* 接受B端发送过来的湿度数据 */
    //......后续信息需要根据传输信息定义
    void (*Send)(char*, ...);
}ZIGBEE_Operations;
extern ZIGBEE_Operations ZIGBEE_Ctrler;
#endif
#if LORA_ENABLE  ==  _enable_
#include "LORA.h"
#define LORA_RUN_SPACE          2   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    __IO uint8_t Update_Flag;       /* LORA接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Temp;              /* 接收B端发送过来的温度数据 */
    __IO uint8_t Humi;              /* 接受B端发送过来的湿度数据 */
    //......后续信息需要根据传输信息定义
    void (*Send)(char*, ...);
}LORA_Operations;
extern LORA_Operations LORA_Ctrler;
#endif
#if IR_REMOTE_ENABLE  ==  _enable_
#include "IR_Remote.h"
#define IR_REMOTE_RUN_SPACE     1   /* 数据检查间隔，检查是否接受到新的数据 时间单位为Comm_Task执行间隔 */
typedef struct
{
    __IO uint8_t Update_Flag;       /* 红外接收完一帧数据标志 外部通过判断该标志位判断是否接收到新的信息 使用后需要清除 */
    __IO uint8_t Address;           /* 接收地址信息 */
    __IO uint8_t Command;           /* 接收命令信息 */
}IR_REMOTE_DataStruct;
extern IR_REMOTE_DataStruct IR_REMOTE_DataInfo;
#endif
/* ***************************预编译 结束*********************************** */

/* ***************************枚举类型定义 开始***************************** */

/* ***************************枚举类型定义 结束***************************** */

/* ***************************共用体类型定义 开始*************************** */

/* ***************************共用体类型定义 结束*************************** */

/* ***************************结构体类型定义 开始*************************** */

/* ***************************结构体类型定义 结束*************************** */

/* ***************************外部变量声明 开始***************************** */

/* ***************************外部变量声明 结束***************************** */

/* ***************************函数接口声明 开始***************************** */
void Comm_Init(void);       /* 通信接口初始化 */
void Comm_Execute(void);    /* 通信任务 */
/* ***************************函数接口声明 结束***************************** */

#endif
