// lowpass_filter_c_interface.cpp
#include "lowpass_filter.h"
#include "lowpass_filter_interface.h"

// 定义不透明指针（实际指向 C++ 类）
struct LowPassFilter_C {
    LowPassFilter instance;
};

// 创建滤波器（C 接口）
LowPassFilter_C* LowPassFilter_Create(float Tf) {
    LowPassFilter_C* obj = new LowPassFilter_C;
    obj->instance = LowPassFilter(Tf);  // 调用带参构造函数
    return obj;
}

// 销毁滤波器（C 接口）
void LowPassFilter_Destroy(LowPassFilter_C* filter) {
    delete filter;
}

// 更新滤波值（C 接口）
float LowPassFilter_Update(LowPassFilter_C* filter, float x) {
	return filter->instance.operator()(x);
}
