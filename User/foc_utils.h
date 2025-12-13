#ifndef _FOC_UTILS_H
#define _FOC_UTILS_H

#include <math.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define _constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

// utility defines
#define _2PI 6.2831852
#define _PI 3.1415926
#define _PI_2 1.5707963
#define _PI_3 1.04719753
#define voltage_pow_supply 65
#define Pole_Pairs 7
#define Tpwm 1
#define Vdc (voltage_pow_supply)
#define _SQRT3 1.73205080757 // 调制比
#define TIM_Period 1440
#define calibration_rounds 1000.0f
// 速度环 PI控制器参数
#define DEF_PID_VEL_P 0.5f //!< default PID controller P value
#define DEF_PID_VEL_I 10.0f //!<  default PID controller I value
#define DEF_PID_VEL_D 0.0f //!<  default PID controller D value
#define DEF_PID_VEL_RAMP 1000.0f //!< default PID controller voltage ramp value
#define DEF_PID_VEL_LIMIT (voltage_pow_supply) //!< default PID controller voltage limit
#define DEF_VEL_FILTER_Tf 0.005
// 电流环 PI控制器参数
#define DEF_PID_KP 0.22f
#define DEF_PID_KI 0.025f
#define DEF_PID_CURQ_P (DEF_PID_KP) //!< default PID controller P value
#define DEF_PID_CURQ_I (DEF_PID_KI) //!<  default PID controller I value
#define DEF_PID_CURQ_D 0.0f //!<  default PID controller D value
#define DEF_PID_CURQ_RAMP 1000.0f //!< default PID controller voltage ramp value
#define DEF_PID_CURQ_LIMIT (voltage_pow_supply) //!< default PID controller voltage limit
#define DEF_PID_CURD_P (DEF_PID_KP) //!< default PID controller P value 电机电感*电流环带宽*2*pi
#define DEF_PID_CURD_I (DEF_PID_KI) //!<  default PID controller I value 电机电阻*电流环带宽*2*pi/时间步长
#define DEF_PID_CURD_D 0.0f //!<  default PID controller D value
#define DEF_PID_CURD_RAMP 1000.0f //!< default PID controller voltage ramp value
#define DEF_PID_CURD_LIMIT (voltage_pow_supply) //!< default PID controller voltage limit
#define DEF_CUR_Q_FILTER_Tf 0.001
#define DEF_CUR_D_FILTER_Tf 0.0005
#define MotionMode (VoltageControlMode)

// 枚举电机控制模式
typedef enum{
		VoltageControlMode,		// 开环电压控制模式
		CurrentControlMode,		// 电流环控制模式
		VelocityControlMode,	// 速度环控制模式
		PositionControlMode,	// 位置环控制模式
}MOTION_CONTROL_TYPE;

/**
 * 归一化角度到[0, 2PI]
 *
 * @param angle - 角度
 */
float _normalizeAngle(float angle);


/**
 * 电角度求解
 *
 * @param shaft_angle - 电机的机械角度
 * @param pole_pairs - 电机的极对数
 */
float _electricalAngle(float shaft_angle, int pole_pairs);


/**
 * Uint转Float
 *
 * @param raw - 转换的数据
 * @param scale - 转换后的范围
 * @param max_raw - 转换前的范围
 */
float _Uint16ToFloat(uint16_t raw, float scale, float max_raw);

// 中值滤波函数
float median_filter(float new_value);

float process_sensor_data(float raw_data);

/**
 * 获取系统时间戳
 *
 */
uint64_t micros(void);

#ifdef __cplusplus
}
#endif

#endif
