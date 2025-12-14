#include "key.h"
struct
{
    uint8_t key_state;  /* 保存按键状态 */
    uint8_t key_judge;  /* 进行状态跟踪 */
    uint8_t key_flag;   /* 按键按下标志 */
    uint8_t key_reserve;/* 保留位，未使用 */
}key[4];

/*
*********************************************************************************************************
*   文件说明:Key_Tick函数调用周期 1ms
*********************************************************************************************************
*/

/**
 * @brief   KEY初始化 
 * @param   none
 * @retval  none
 */
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启按键对应GPIO时钟
    RCC_APB2PeriphClockCmd(KEY1_CLK | KEY2_CLK | KEY3_CLK | KEY4_CLK, ENABLE);

    // 初始化所有按键为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 初始化 KEY1
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);

    // 初始化 KEY2
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY2_PORT, &GPIO_InitStructure);

    // 初始化 KEY3
    GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY3_PORT, &GPIO_InitStructure);

    // 初始化 KEY4
    GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
    GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
}

/**
 * @brief   KEY扫描函数
 * @param   none
 * @retval  key_value   ：   按下KEY的键值 KEY1_PRESS | KEY2_PRESS | KEY3_PRESS | KEY4_PRESS
 */
uint8_t Key_Scan(void)
{
    uint8_t key_value = 0;
    for(uint8_t i = 0; i < 4; i++)
    {
        if(key[i].key_flag)
        {
            key[i].key_flag = 0;
            switch(i)
            {
                case 0:     // KEY1按下
                    key_value = KEY1_PRESS;
                    break;
                case 1:     // KEY2按下
                    key_value = KEY2_PRESS;
                    break;
                case 2:     // KEY3按下
                    key_value = KEY3_PRESS;
                    break;
                case 3:     // KEY4按下
                    key_value = KEY4_PRESS;
                    break;
            }
            break;  /* 下次再来处理 - 防止按键标志被挤掉 */
        }
    }
    return key_value;
}


/**
 * @brief   按键检测函数，需要实现调用周期为1ms，建议放在1ms的中断处理函数中
 * @param   none
 * @retval  none
 */
void Key_Tick(void)
{
    static uint8_t Tick_Count = 0;
    Tick_Count++;
    if(Tick_Count >= 10)
    {
        Tick_Count = 0;
        /* 1.读取引脚电平 */
        key[0].key_state = GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN);
        key[1].key_state = GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN);
        key[2].key_state = GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN);
        key[3].key_state = GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN);
        
        for(uint8_t i = 0; i < 4; i++)
        {
            switch(key[i].key_judge)
            {
                case 0:     // 检测按键状态
                    if(key[i].key_state == 0)
                    {
                        key[i].key_judge++;
                    }
                    break;
                case 1:     // 判断按键是否按下
                    if(key[i].key_state == 0)
                    {
                        key[i].key_judge++;
                        key[i].key_flag = 1;
                    }
                    else
                    {
                        key[i].key_judge = 0;
                    }
                    break;
                case 2:     // 等待按键松开
                    if(key[i].key_state != 0)
                    {
                        key[i].key_judge = 0;
                    }
                    break;
            }
        }
    }
}
