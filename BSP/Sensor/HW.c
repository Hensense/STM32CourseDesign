#include "HW.h"
/* 红外传感器 用于小车循迹 障碍物检测 */
#if HW_SENSOR_NUMBER == 1
GPIO_TypeDef* HW_GPIO_PORTS[] = {HW1_GPIO_PORT};
uint16_t HW_GPIO_PINS[] = {HW1_GPIO_PIN};
#elif HW_SENSOR_NUMBER == 2
GPIO_TypeDef* HW_GPIO_PORTS[] = {HW1_GPIO_PORT, HW2_GPIO_PORT};
uint16_t HW_GPIO_PINS[] = {HW1_GPIO_PIN, HW2_GPIO_PIN};
#elif HW_SENSOR_NUMBER == 3
GPIO_TypeDef* HW_GPIO_PORTS[] = {HW1_GPIO_PORT, HW2_GPIO_PORT, HW3_GPIO_PORT};
uint16_t HW_GPIO_PINS[] = {HW1_GPIO_PIN, HW2_GPIO_PIN, HW3_GPIO_PIN};
#elif HW_SENSOR_NUMBER == 4
GPIO_TypeDef* HW_GPIO_PORTS[] = {HW1_GPIO_PORT, HW2_GPIO_PORT, HW3_GPIO_PORT, HW4_GPIO_PORT};
uint16_t HW_GPIO_PINS[] = {HW1_GPIO_PIN, HW2_GPIO_PIN, HW3_GPIO_PIN, HW4_GPIO_PIN};
#endif


void HW_Init(void)
{
#if HW_SENSOR_NUMBER != 0 && HW_SENSOR_NUMBER <= 4
    GPIO_InitTypeDef GPIO_InitStructure;
#endif
    
#if HW_SENSOR_NUMBER == 1
    RCC_APB2PeriphClockCmd (HW1_GPIO_CLK, ENABLE );          // 打开连接 传感器D1 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW1_GPIO_PIN;              // 配置连接 传感器D1 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
#elif HW_SENSOR_NUMBER == 2
    RCC_APB2PeriphClockCmd (HW1_GPIO_CLK, ENABLE );          // 打开连接 传感器D1 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW1_GPIO_PIN;              // 配置连接 传感器D1 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW2_GPIO_CLK, ENABLE );          // 打开连接 传感器D3 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW2_GPIO_PIN;              // 配置连接 传感器D3 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW2_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
#elif HW_SENSOR_NUMBER == 3
    RCC_APB2PeriphClockCmd (HW1_GPIO_CLK, ENABLE );          // 打开连接 传感器D1 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW1_GPIO_PIN;              // 配置连接 传感器D1 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW2_GPIO_CLK, ENABLE );          // 打开连接 传感器D3 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW2_GPIO_PIN;              // 配置连接 传感器D3 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW2_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW3_GPIO_CLK, ENABLE );          // 打开连接 传感器D3 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW3_GPIO_PIN;              // 配置连接 传感器D3 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW3_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
#elif HW_SENSOR_NUMBER == 4
    RCC_APB2PeriphClockCmd (HW1_GPIO_CLK, ENABLE );          // 打开连接 传感器D1 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW1_GPIO_PIN;              // 配置连接 传感器D1 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW3_GPIO_CLK, ENABLE );          // 打开连接 传感器D3 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW3_GPIO_PIN;              // 配置连接 传感器D3 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW3_GPIO_CLK, ENABLE );          // 打开连接 传感器D3 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW3_GPIO_PIN;              // 配置连接 传感器D3 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
    RCC_APB2PeriphClockCmd (HW4_GPIO_CLK, ENABLE );          // 打开连接 传感器D4 的单片机引脚端口时钟
    GPIO_InitStructure.GPIO_Pin = HW4_GPIO_PIN;              // 配置连接 传感器D4 的单片机引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 设置为上拉输入
    GPIO_Init(HW1_GPIO_PORT, &GPIO_InitStructure);           // 初始化 
#endif
}

uint8_t* HW_GetFlag(void)
{
#if HW_SENSOR_NUMBER <= 4 && HW_SENSOR_NUMBER != 0
    static uint8_t HW_Flag[HW_SENSOR_NUMBER];
    for(uint8_t i = 0; i < HW_SENSOR_NUMBER; i++)
    {
        HW_Flag[i] = !GPIO_ReadInputDataBit(HW_GPIO_PORTS[i], HW_GPIO_PINS[i]);
    }
    return &HW_Flag[0];
#elif HW_SENSOR_NUMBER == 0
    return NULL;
#endif
}



