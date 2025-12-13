// lowpass_filter_c_interface.h (C/C++ 共用头文件)
#ifndef _LOWPASS_FILTER_C_INTERFACE_H
#define _LOWPASS_FILTER_C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 声明 C 兼容的不透明指针（隐藏 C++ 类的细节）
typedef struct LowPassFilter_C LowPassFilter_C;

// C 接口函数
LowPassFilter_C* LowPassFilter_Create(float Tf);  // 创建滤波器
void LowPassFilter_Destroy(LowPassFilter_C* filter);  // 销毁滤波器
float LowPassFilter_Update(LowPassFilter_C* filter, float x);  // 更新滤波值

#ifdef __cplusplus
}
#endif

#endif
