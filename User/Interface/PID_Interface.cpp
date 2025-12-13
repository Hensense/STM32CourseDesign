#include "pid.h"
#include "pid_interface.h"

extern "C" {

// 创建 PID 控制器对象
PIDControllerHandle PIDController_create(float P, float I, float D, float ramp, float limit) {
    return new PIDController(P, I, D, ramp, limit);  // 返回 C++ 对象指针
}

// 销毁 PID 控制器对象
void PIDController_delete(PIDControllerHandle handle) {
    if (handle) {
        delete static_cast<PIDController*>(handle);
    }
}

// 调用 PID 计算 error = targetValue - currentValue
float PIDController_compute(PIDControllerHandle handle, float error) {
    if (handle) {
        PIDController* controller = static_cast<PIDController*>(handle);
        return (*controller)(error);  // 调用 operator()
    }
    return 0.0f;  // 错误处理
}

} // extern "C"
