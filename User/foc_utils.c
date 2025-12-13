#include "foc_utils.h"
#include <string.h>

// 归一化角度到[0, 2PI]
float _normalizeAngle(float angle)
{
    float a = fmod(angle, _2PI);
    return a >= 0 ? a : (a + _2PI);
}

// 电角度求解
float _electricalAngle(float shaft_angle, int pole_pairs)
{
    return (shaft_angle * pole_pairs);
}

// uint转float
float _Uint16ToFloat(uint16_t raw, float scale, float max_raw) {
    return (float)raw * scale / max_raw;
}

// 中值滤波函数
float median_filter(float new_value) {
    static float window[5] = {0}; // 窗口大小5
    static int index = 0;
    static int count = 0;
    
    // 更新窗口
    window[index] = new_value;
    index = (index + 1) % 5;
    if (count < 5) count++;
    
    // 复制窗口数据进行排序
    float temp[5];
    memcpy(temp, window, sizeof(temp));
    
    // 简单冒泡排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                float swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }
    
    // 返回中值
    return temp[count / 2];
}

float process_sensor_data(float raw_data) {
    // 第一级：去毛刺
    float denoised = median_filter(raw_data);
    
    // 第二级：平滑
    static float smoothed = 0;
    smoothed = 0.2f * denoised + 0.8f * smoothed;
    
    return smoothed;
}

__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag()
{
    return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

uint64_t micros(void)
{
    /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
    LL_SYSTICK_IsActiveCounterFlag();
    uint32_t m = HAL_GetTick();
    static uint32_t tms;
	tms = SysTick->LOAD + 1;
    __IO uint32_t u = tms - SysTick->VAL;
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
        m = HAL_GetTick();
        u = tms - SysTick->VAL;
    }
    return (m * 1000 + (u * 1000) / tms);
}

