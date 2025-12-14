#include "IR_Remote.h"
/**
  * 低电平：560us低电平+560us高电平
  * 高电平：560us低电平+1680us高电平
  * 引导码：9ms低电平+4.5ms高电平
  * 重复码：9ms低电平+2.5ms高电平
  * 每个IR周期的长度是110ms
*/
#if IR_REMOTE_START_STATE
// 全局变量
uint8_t IR_Remote_State = 0;
uint8_t IR_Remote_DataFlag = 0;
uint8_t IR_Remote_RepeatFlag = 0;
uint8_t IR_Remote_Data[4] = {0};
uint8_t IR_Remote_Address = 0;
uint8_t IR_Remote_Command = 0;
uint8_t IR_Remote_pData = 0;
uint16_t IR_Remote_Time = 0;

// 获取地址
uint8_t IR_Remote_GetAddress(void) {
    return IR_Remote_Address;
}

// 获取命令
uint8_t IR_Remote_GetCommand(void) {
    return IR_Remote_Command;
}

// 获取数据标志
uint8_t IR_Remote_GetDataFlag(void) {
    if(IR_Remote_DataFlag) {
        IR_Remote_DataFlag = 0;
        return 1;
    }
    return 0;
}

// 获取重复标志
uint8_t IR_Remote_GetRepeatFlag(void) {
    if(IR_Remote_RepeatFlag) {
        IR_Remote_RepeatFlag = 0;
        return 1;
    }
    return 0;
}

/**
  * 函    数：红外遥控初始化
  * 参    数：无
  * 返 回 值：无
  */
void IR_Remote_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // 开启AFIO时钟
    RCC_APB2PeriphClockCmd(IR_REMOTE_CLK, ENABLE); 
    RCC_APB1PeriphClockCmd(IR_TIM_RCC, ENABLE);             // 开启定时器时钟
    
    /* GPIO初始化 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 上拉输入模式
    GPIO_InitStructure.GPIO_Pin = IR_REMOTE_PIN;            // 选择引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 设置引脚速度
    GPIO_Init(IR_REMOTE_PORT, &GPIO_InitStructure);         // 初始化引脚
    
    /* AFIO选择中断引脚 */
    GPIO_EXTILineConfig(IR_REMOTE_PORT_SOURCE, IR_REMOTE_PIN_SOURCE); // 映射外部中断
    
    /* EXTI初始化 */
    EXTI_InitStructure.EXTI_Line = IR_REMOTE_EXTI_LINE;        // 选择外部中断线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // 使能外部中断
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_Init(&EXTI_InitStructure);                         // 初始化EXTI

    /* NVIC配置 */
    NVIC_InitStructure.NVIC_IRQChannel = IR_REMOTE_EXTI_IRQn;   // 选择外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 使能通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 响应优先级
    NVIC_Init(&NVIC_InitStructure);                             // 初始化NVIC
    
    /* 定时器初始化 */
    TIM_TimeBaseStructure.TIM_Period = 65536-1;                 // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;                 // 预分频器 (72MHz/72 = 1MHz)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(IR_TIM, &TIM_TimeBaseStructure);           // 初始化定时器
    
    /* 启动定时器 */
    TIM_Cmd(IR_TIM, ENABLE);
}

/**
  * 函数：外部中断处理函数
  * 参    数：无
  * 返 回 值：无
  */
void ID_REMOTE_IRQHandler(void)
{
    // 检查中断标志
    if (EXTI_GetITStatus(IR_REMOTE_EXTI_LINE) == SET)
    {
        // 清除中断标志
        EXTI_ClearITPendingBit(IR_REMOTE_EXTI_LINE);
        
        // 状态机处理
        if(IR_Remote_State == 0) {      // 空闲状态
            TIM_SetCounter(IR_TIM, 0);
            IR_Remote_State = 1;
        }
        else if(IR_Remote_State == 1) { // 接收数据状态，等待start和repeat信号
            IR_Remote_Time = TIM_GetCounter(IR_TIM);
            TIM_SetCounter(IR_TIM, 0);
            if(IR_Remote_Time > 13500-500 && IR_Remote_Time <13500+500) {
                /* 接收到引导码 */
                IR_Remote_State = 2;
            }
            else if(IR_Remote_Time > 11250-500 && IR_Remote_Time < 11250+500) {
                /* 接收到连发码 */
                IR_Remote_RepeatFlag = 1;
                IR_Remote_State = 0;
            }
            else {
                /* 接收出错 */
                IR_Remote_State = 1;
            }
        }
        else if(IR_Remote_State == 2) {  // 接收32位的数据帧
            IR_Remote_Time = TIM_GetCounter(IR_TIM);
            TIM_SetCounter(IR_TIM, 0);
            
            if(IR_Remote_Time > 1120-500 && IR_Remote_Time < 1120+500) {
                IR_Remote_Data[IR_Remote_pData / 8] &= ~(0x01 << IR_Remote_pData % 8);
                IR_Remote_pData++;        
            }
            else if(IR_Remote_Time > 2240-500 && IR_Remote_Time < 2240+500) {
                IR_Remote_Data[IR_Remote_pData / 8] |= (0x01 << IR_Remote_pData % 8);
                IR_Remote_pData++;
            }
            else {
                /* 接收出错 */
                IR_Remote_pData = 0;
                IR_Remote_State = 1;
            }
            
            if(IR_Remote_pData >= 32) {
                /* 数据接收完毕 */
                if(IR_Remote_Data[0] == (uint8_t)~IR_Remote_Data[1] && 
                   IR_Remote_Data[2] == (uint8_t)~IR_Remote_Data[3]) {
                    /* 数据接收正确 */
                    IR_Remote_Address = IR_Remote_Data[0];
                    IR_Remote_Command = IR_Remote_Data[2];
                    IR_Remote_DataFlag = 1;
                }
                IR_Remote_pData = 0;
                IR_Remote_State = 0;
            }
        }
    }
}
#endif
