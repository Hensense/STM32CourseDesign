#ifndef __AS608_H
#define __AS608_H

#define AS608_START_STATE   0   /* 0:禁用文件 1:启用文件 */
#if AS608_START_STATE
#include "sys.h"
#include <stdio.h>
#include <string.h>
/************************************引脚宏定义*******************************************************/
#define AS608_USARTx   3  // 可选：USART1, USART2, USART3

/* 串口选择 */
#if AS608_USARTx == 1
    #define AS608_USART_UX           USART1
    #define AS608_USART_IRQn         USART1_IRQn
    #define AS608_USART_IRQHandler   USART1_IRQHandler
    #define AS608_GPIO_TX_PORT       GPIOA
    #define AS608_GPIO_TX_PIN        GPIO_Pin_9
    #define AS608_GPIO_RX_PORT       GPIOA
    #define AS608_GPIO_RX_PIN        GPIO_Pin_10
    #define AS608_RCC_APBPeriph      RCC_APB2Periph_USART1
    #define AS608_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif AS608_USARTx == 2
    #define AS608_USART_UX           USART2
    #define AS608_USART_IRQn         USART2_IRQn
    #define AS608_USART_IRQHandler   USART2_IRQHandler
    #define AS608_GPIO_TX_PORT       GPIOA
    #define AS608_GPIO_TX_PIN        GPIO_Pin_2
    #define AS608_GPIO_RX_PORT       GPIOA
    #define AS608_GPIO_RX_PIN        GPIO_Pin_3
    #define AS608_RCC_APBPeriph      RCC_APB1Periph_USART2
    #define AS608_GPIO_RCC           RCC_APB2Periph_GPIOA
#elif AS608_USARTx == 3
    #define AS608_USART_UX           USART3
    #define AS608_USART_IRQn         USART3_IRQn
    #define AS608_USART_IRQHandler   USART3_IRQHandler
    #define AS608_GPIO_TX_PORT       GPIOB
    #define AS608_GPIO_TX_PIN        GPIO_Pin_10
    #define AS608_GPIO_RX_PORT       GPIOB
    #define AS608_GPIO_RX_PIN        GPIO_Pin_11
    #define AS608_RCC_APBPeriph      RCC_APB1Periph_USART3
    #define AS608_GPIO_RCC           RCC_APB2Periph_GPIOB
#else
    #error "Unsupported USART port! Please choose USART1/2/3."
#endif
/* 感应引脚选择 */
#define AS608_PS_GPIO_CLK           RCC_APB2Periph_GPIOB
#define AS608_PS_GPIO_PORT          GPIOB
#define AS608_PS_GPIO_PIN           GPIO_Pin_1
#define PS_Sta                      PBin(1)//读指纹模块状态引脚
/************************************引脚宏定义*******************************************************/
/* 定时器选择 */
#define AS608_TIMx    4  // 可选：TIM1, TIM2, TIM3, TIM4

// 根据宏定义自动选择对应的TIM和GPIO
#if AS608_TIMx == 1
    #define AS608_TIM               TIM1
    #define AS608_TIM_IRQn          TIM1_UP_IRQn
    #define AS608_TIM_IRQHandler    TIM1_UP_IRQHandler
    #define AS608_TIM_RCC           RCC_APB2Periph_TIM1
#elif AS608_TIMx == 2
    #define AS608_TIM               TIM2
    #define AS608_TIM_IRQn          TIM2_IRQn
    #define AS608_TIM_IRQHandler    TIM2_IRQHandler
    #define AS608_TIM_RCC           RCC_APB1Periph_TIM2
#elif AS608_TIMx == 3
    #define AS608_TIM               TIM3
    #define AS608_TIM_IRQn          TIM3_IRQn
    #define AS608_TIM_IRQHandler    TIM3_IRQHandler
    #define AS608_TIM_RCC           RCC_APB1Periph_TIM3
#elif AS608_TIMx == 4
    #define AS608_TIM               TIM4
    #define AS608_TIM_IRQn          TIM4_IRQn
    #define AS608_TIM_IRQHandler    TIM4_IRQHandler
    #define AS608_TIM_RCC           RCC_APB1Periph_TIM4
#else
    #error "Unsupported TIM port! Please choose TIM1/2/3/4."
#endif


#define CharBuffer1 0x01
#define CharBuffer2 0x02

extern uint32_t AS608Addr;//模块地址

typedef struct  
{
    uint16_t pageID;//指纹ID
    uint16_t mathscore;//匹配得分
}SearchResult;

typedef struct
{
    uint16_t PS_max;//指纹最大容量
    uint8_t  PS_level;//安全等级
    uint32_t PS_addr;
    uint8_t  PS_size;//通讯数据包大小
    uint8_t  PS_N;//波特率基数N
}SysPara;

void AS608_Check(void);
void AS608_Init(void);//初始化AS608
uint8_t AS608_AddFR(void);  /* 录入指纹 */
uint8_t AS608_PressFR(void);/* 刷指纹 */
uint8_t AS608_DeleteFR(uint16_t delete_id); /* 删除指纹 */
uint8_t AS608_GetFRNumber(void);            /* 获取指纹数 */
    
uint8_t PS_GetImage(void); //录入图像 
 
uint8_t PS_GenChar(uint8_t BufferID);//生成特征 

uint8_t PS_Match(void);//精确比对两枚指纹特征 

uint8_t PS_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);//搜索指纹 
 
uint8_t PS_RegModel(void);//合并特征（生成模板） 
 
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID);//储存模板 

uint8_t PS_DeletChar(uint16_t PageID,uint16_t N);//删除模板 

uint8_t PS_Empty(void);//清空指纹库 

uint8_t PS_WriteReg(uint8_t RegNum,uint8_t DATA);//写系统寄存器 
 
uint8_t PS_ReadSysPara(SysPara *p); //读系统基本参数 

uint8_t PS_SetAddr(uint32_t addr);  //设置模块地址 

uint8_t PS_WriteNotepad(uint8_t NotePageNum,uint8_t *content);//写记事本 

uint8_t PS_ReadNotepad(uint8_t NotePageNum,uint8_t *note);//读记事 

uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);//高速搜索 
  
uint8_t PS_ValidTempleteNum(uint16_t *ValidN);//读有效模板个数 

uint8_t PS_HandShake(uint32_t *PS_Addr); //与AS608模块握手

#endif

#endif







