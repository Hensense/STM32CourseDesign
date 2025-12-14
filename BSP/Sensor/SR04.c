#include "SR04.h"
#if SR04_START_STATE
// 全局变量
int time;   // 超声波模块的回波时间
uint8_t capture_status = 0;    // 输入捕获状态                            
uint16_t capture_value = 0;    // 输入捕获值

// 定时器初始化结构体
TIM_ICInitTypeDef TIM_ICInitStructure;

void SR04_Init(uint16_t arr, uint16_t psc)
{     
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能定时器时钟
    RCC_APB1PeriphClockCmd(SR04_TIM_RCC, ENABLE);
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(TRIG_RCC | ECHO_RCC, ENABLE);

    // 配置回波引脚(ECHO)为输入
    GPIO_InitStructure.GPIO_Pin  = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // 下拉输入
    GPIO_Init(ECHO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(ECHO_PORT, ECHO_PIN);  // 下拉

    // 配置触发引脚(TRIG)为输出
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_PORT, &GPIO_InitStructure);
    GPIO_SetBits(TRIG_PORT, TRIG_PIN);  // 初始高电平
    
    // 初始化定时器时基
    TIM_TimeBaseStructure.TIM_Period = arr;  // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInit(SR04_TIM, &TIM_TimeBaseStructure);
    
    // 初始化输入捕获参数
    TIM_ICInitStructure.TIM_Channel = SR04_TIM_CHANNEL;  // 选择通道
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  // 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  // 直接映射
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  // 不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x00;  // 不滤波
    TIM_ICInit(SR04_TIM, &TIM_ICInitStructure);
    
    // 配置中断
    NVIC_InitStructure.NVIC_IRQChannel = SR04_TIM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 使能更新中断和捕获中断
    TIM_ITConfig(SR04_TIM, TIM_IT_Update | TIM_IT_CC2, ENABLE);
    
    // 使能定时器
    TIM_Cmd(SR04_TIM, ENABLE);
}

// 定时器中断服务程序
void SR04_TIM_IRQHandler(void)
{ 
    if((capture_status & 0X80) == 0)  // 还未成功捕获
    {      
        if (TIM_GetITStatus(SR04_TIM, TIM_IT_Update) != RESET)
        {        
            if(capture_status & 0X40)  // 已经捕获到高电平
            {
                if((capture_status & 0X3F) == 0X3F)  // 高电平太长
                {
                    capture_status |= 0X80;  // 标记成功捕获
                    capture_value = 0XFFFF;
                } else {
                    capture_status++;
                }
            }     
        }
        
        // 捕获中断处理
        if (TIM_GetITStatus(SR04_TIM, TIM_IT_CC2) != RESET)
        {    
            if(capture_status & 0X40)  // 捕获到下降沿
            {                  
                capture_status |= 0X80;  // 标记成功捕获
                capture_value = TIM_GetCapture2(SR04_TIM);
                TIM_OC2PolarityConfig(SR04_TIM, TIM_ICPolarity_Rising);  // 设置为上升沿捕获
            } 
            else  // 第一次捕获上升沿
            {
                capture_status = 0;  // 清空状态
                capture_value = 0;
                TIM_SetCounter(SR04_TIM, 0);  // 计数器清零
                capture_status |= 0X40;  // 标记捕获到上升沿
                TIM_OC2PolarityConfig(SR04_TIM, TIM_ICPolarity_Falling);  // 设置为下降沿捕获
            }            
        }                                                
    }
    
    // 清除中断标志
    TIM_ClearITPendingBit(SR04_TIM, TIM_IT_CC2 | TIM_IT_Update);
}

// 获取距离函数
uint32_t SR04_GetDistance(void)
{
    uint32_t distance = 0;
    uint32_t timeout = 0xFFFFF;
    
    // 发送触发信号
    TRIG_OUT(1);  // 高电平
    delay_us(13);  // 持续13us
    TRIG_OUT(0);  // 低电平
    
    // 等待捕获完成
    while(timeout--)
    {
        if(capture_status & 0X80)  // 成功捕获
        {
            time = capture_status & 0X3F;  // 溢出次数
            time *= 65536;  // 溢出时间总和
            time += capture_value;  // 总的高电平时间
            distance = time * 0.033 / 2;  // 计算距离 (cm)
            
            // 重置状态
            capture_status = 0;
            break;
        }
    }
    
    return distance;
}
#endif
