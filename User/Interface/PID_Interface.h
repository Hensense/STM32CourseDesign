#ifndef _PID_INTERFACE_H
#define _PID_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 不透明指针类型（隐藏 C++ 类的具体实现）
typedef void* PIDControllerHandle;

// C 接口函数声明
PIDControllerHandle PIDController_create(float P, float I, float D, float ramp, float limit);
void PIDController_delete(PIDControllerHandle handle);
float PIDController_compute(PIDControllerHandle handle, float error);

#ifdef __cplusplus
}
#endif

#endif
