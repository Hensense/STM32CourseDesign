// motor.h
#ifndef _BLCD_MOTOR_H
#define _BLCD_MOTOR_H

#include <math.h>
#include <stdio.h>
#include "tim.h"
//#include "usart.h"
#include "as5600.h"
#include "ina240.h"
#include "foc_utils.h"
#include "pid_interface.h"
#include "lowpass_filter_interface.h"
#include "oled.h"


typedef struct _MOTOR_T {
	int pole_pairs;              // 电机极对数
	int8_t modulation_centered;  // flag (1) centered modulation around driver
							   // limit /2  or  (0) pulled to 0
	float Ua, Ub, Uc;        // 用于设定的相电压Ua,Ub,Uc
	float angle;       // 当前电机角度
	float velocity;    // 当前电机速度
	float electrical_angle;  // 当前电角度
	float voltage_limit;     // 电压限制变量 - 全局限制
	float velocity_limit;    // 速度极限变量 - 全局极限
	float theta_sum;         // 电机走过的总角度 -> 求转数
	float timestamp_prev;
	float angle_prev;

	float phase_resistance;  // 电机相电阻

	PIDControllerHandle PID_velocity;
	PIDControllerHandle PID_current_q;
	PIDControllerHandle PID_current_d;
	LowPassFilter_C* LPF_velocity;
	LowPassFilter_C* LPF_current_q;
	LowPassFilter_C* LPF_current_d;
	//  PID_T PID_velocity;
	//  PID_T PID_angle;
	//  PID_T PID_current_q;
	//  PID_T PID_current_d;
	//  LOWPASS_FILTER_T LPF_velocity;
	//  LOWPASS_FILTER_T LPF_angle; 
	//  LOWPASS_FILTER_T LPF_current_q;
	//  LOWPASS_FILTER_T LPF_current_d;

	float voltage_sensor_align;  // 传感器和电机对齐电压参数
	int sensor_direction;        // 传感器方向
	float zero_electric_angle;   // 绝对零电角
	float sensor_offset;         // 自定义传感器零点偏移

	CURRENT_T current_sensor;    // 电流传感器
	DQVoltage_T DQvoltage;
	DQCurrent_T DQcurrent;

	float target_angle;         // 当前目标角度
	float target_velocity;      // 当前目标速度
	float feed_forward_velocity;  // 当前前馈速度
	
  MOTION_CONTROL_TYPE controller;
} MOTOR_T;


void Motor_Init(MOTOR_T* motor, int pole_pairs);
float getVelocity(MOTOR_T* motor);
void VelocityOpenloop(MOTOR_T* motor, float target_velocity);
void PositionOpenloop(MOTOR_T* motor, float target_position, float target_velocity);
void Voltageloop(MOTOR_T* motor, float target_velocity);
void Currentloop(MOTOR_T* motor, float Target_Iq, float Target_Id);
void move(MOTOR_T* motor);

#endif
