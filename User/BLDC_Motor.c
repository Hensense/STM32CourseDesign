#include "BLDC_Motor.h"

float shaft_angle = 0.0f;

void Motor_Init(MOTOR_T* motor, int pole_pairs)
{
	CURRENT_Init(&motor->current_sensor);
	motor->pole_pairs = pole_pairs;
	
	motor->angle = 0;
	motor->angle_prev = 0;
	motor->theta_sum = 0;
//	motor->DQvoltage.Uq = 2;
//	motor->DQvoltage.Ud = 0;
	motor->sensor_offset = 1.27168596;
	
	motor->PID_velocity = PIDController_create(DEF_PID_VEL_P, DEF_PID_VEL_I, DEF_PID_VEL_D, DEF_PID_VEL_RAMP, DEF_PID_VEL_LIMIT);
	motor->PID_current_q = PIDController_create(DEF_PID_CURQ_P, DEF_PID_CURQ_I, DEF_PID_CURQ_D, DEF_PID_CURQ_RAMP, DEF_PID_CURQ_LIMIT);
	motor->PID_current_d = PIDController_create(DEF_PID_CURD_P, DEF_PID_CURD_I, DEF_PID_CURD_D, DEF_PID_CURD_RAMP, DEF_PID_CURD_LIMIT);

	motor->LPF_velocity = LowPassFilter_Create(DEF_VEL_FILTER_Tf);
	motor->LPF_current_q = LowPassFilter_Create(DEF_CUR_Q_FILTER_Tf);
	motor->LPF_current_d = LowPassFilter_Create(DEF_CUR_D_FILTER_Tf);
	
	motor->timestamp_prev = 0;
	
	motor->controller = MotionMode;
}

// 判断扇区
int getN(float Ualpha, float Ubeta)
{
	int A = -_SQRT3*Ualpha/2-Ubeta/2>0? 1 : 0;
	int B = _SQRT3*Ualpha/2-Ubeta/2>0? 1 : 0;
	int C = Ubeta>0? 1 : 0;
	int N = 4*A+2*B+C;
	return N;
}

// 电压转换
void setPhaseVoltage(float Uq,float Ud, float angle_el) 
{
	/*
		初始版本 
		会被限制在0-0.5 
		原因未找到
	*/
//	angle_el = _normalizeAngle(angle_el);
//	// 帕克逆变换
//	float Ualpha = cos(angle_el)*Ud - sin(angle_el)*Uq;
//	float Ubeta = sin(angle_el)*Ud + cos(angle_el)*Uq;
//	
//	// 判断扇形分区
//	//  扇区 I   II   III   IV   V   VI  零向量
//	//   N   3   1	   5    4    6   2   0 / 7
//	int N = getN(Ualpha, Ubeta);
//	double X = _SQRT3*Tpwm*Ubeta/Vdc;
//	double Y = _SQRT3*Tpwm/Vdc*(_SQRT3*Ualpha/2+Ubeta/2);
//	double Z = _SQRT3*Tpwm/Vdc*(-_SQRT3*Ualpha/2+Ubeta/2);
//	float T1,T2,T0;
//	float Tcm1,Tcm2,Tcm3;
//	
//	switch(N)
//	{
//		case 1:
//			T1 = Z;
//			T2 = Y;
//			break;
//		case 2:
//			T1 = Y;
//			T2 = -X;
//			break;
//		case 3:
//			T1 = -Z;
//			T2 = X;
//			break;
//		case 4:
//			T1 = -X;
//			T2 = Z;
//			break;
//		case 5:
//			T1 = X;
//			T2 = -Y;
//			break;
//		default:
//			T1 = -Y;
//			T2 = -Z;
//			break;
//	}
//	
//	// 过调制 Tpwm = T0 + T1 + T2 + T7 = 2T0 + T1 + T2 
//	//        即 T0 = （Tpwm - T1 - T2)/2
//	if (Tpwm - T1 -T2 < 0)
//	{
//		float T1_temp = T1 * Tpwm / (T1 + T2);
//		float T2_temp = T2 * Tpwm / (T1 + T2);
//		T1 = T1_temp;
//		T2 = T2_temp;
//	}
//	T0 = (Tpwm - T1 - T2);
//	
//	switch(N)
//	{
//		case 1:
//			Tcm1 = (T0 + T1)/2;
//			Tcm2 = T0/2;
//			Tcm3 = (T0 + T1 + T2)/2;
//			break;
//		case 2:
//			Tcm1 = T0/2;
//			Tcm2 = (T0 + T1 + T2)/2;
//			Tcm3 = (T0 + T1)/2;
//			break;
//		case 3:
//			Tcm1 = T0/2;
//			Tcm2 = (T0 + T1)/2;
//			Tcm3 = (T0 + T1 + T2)/2;
//			break;
//		case 4:
//			Tcm1 = (T0 + T1 + T2)/2;
//			Tcm2 = (T0 + T1)/2;
//			Tcm3 = T0/2;
//			break;
//		case 5:
//			Tcm1 = (T0 + T1 + T2)/2;
//			Tcm2 = T0/2;
//			Tcm3 = (T0 + T1)/2;
//			break;
//		case 6:
//			Tcm1 = (T0 + T1)/2;
//			Tcm2 = (T0 + T1 + T2)/2;
//			Tcm3 = T0/2;
//			break;
//		default:
//			Tcm1 = 0;
//			Tcm2 = 0;
//			Tcm3 = 0;
//			break;
//	}
//	/*
//	PWM频率：Freq = CK_PSC / (PSC+1) / (ARR+1) 其中CK_PSC为时钟频率 一般为72MHz
//	PWM占空比：Duty = CCR / (ARR+1)
//	PWM分辨率：Reso = 1 / (ARR+1)
//	*/
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (Tcm1/Tpwm)*TIM_Period);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (Tcm2/Tpwm)*TIM_Period);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (Tcm3/Tpwm)*TIM_Period);
//	printf("%.12f,%.12f,%.12f\n", (Tcm1/Tpwm), (Tcm2/Tpwm), (Tcm3/Tpwm));
////	printf("%.8f,%.8f\n", motor->angle, motor->velocity);
////	printf("%.12f,%.12f,%.12f\n", Tcm1, Tcm2, Tcm3);
	/*
		改进版本
	*/
	float Uout;
	uint32_t sector;
	float T0,T1,T2;
	float Tcm1,Tcm2,Tcm3;
	
	if(Ud) // only if Ud and Uq set 
	{// _sqrt is an approx of sqrt (3-4% error)
		Uout = _sqrt(Ud*Ud + Uq*Uq) / voltage_pow_supply;
		// angle normalisation in between 0 and 2pi
		// only necessary if using _sin and _cos - approximation functions
		angle_el = _normalizeAngle(angle_el + atan2(Uq, Ud));
	}
	else
	{// only Uq available - no need for atan2 and sqrt
		Uout = Uq / voltage_pow_supply;
		// angle normalisation in between 0 and 2pi
		// only necessary if using _sin and _cos - approximation functions
		angle_el = _normalizeAngle(angle_el + _PI_2);
	}
	if(Uout> 0.577)Uout= 0.577;
	if(Uout<-0.577)Uout=-0.577;
	
	sector = (angle_el / _PI_3) + 1;
	T1 = _SQRT3*sin(sector*_PI_3 - angle_el) * Uout;
	T2 = _SQRT3*sin(angle_el - (sector-1.0)*_PI_3) * Uout;
	T0 = 1 - T1 - T2;
	
	// calculate the duty cycles(times)
	switch(sector)
	{
		case 1:
			Tcm1 = T1 + T2 + T0/2;
			Tcm2 = T2 + T0/2;
			Tcm3 = T0/2;
			break;
		case 2:
			Tcm1 = T1 +  T0/2;
			Tcm2 = T1 + T2 + T0/2;
			Tcm3 = T0/2;
			break;
		case 3:
			Tcm1 = T0/2;
			Tcm2 = T1 + T2 + T0/2;
			Tcm3 = T2 + T0/2;
			break;
		case 4:
			Tcm1 = T0/2;
			Tcm2 = T1+ T0/2;
			Tcm3 = T1 + T2 + T0/2;
			break;
		case 5:
			Tcm1 = T2 + T0/2;
			Tcm2 = T0/2;
			Tcm3 = T1 + T2 + T0/2;
			break;
		case 6:
			Tcm1 = T1 + T2 + T0/2;
			Tcm2 = T0/2;
			Tcm3 = T1 + T0/2;
			break;
		default:  // possible error state
			Tcm1 = 0;
			Tcm2 = 0;
			Tcm3 = 0;
	}
	
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Tcm1*TIM_Period);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, Tcm2*TIM_Period);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, Tcm3*TIM_Period);
	
//	printf("%.12f,%.12f,%.12f\n",Tcm1, Tcm2, Tcm3);
}

/**
 * @brief 位置开环控制（HAL库版本）
 * @param target_position 目标角度（rad）
 * @return 输出的Q轴电压 Uq
 */
void PositionOpenloop(MOTOR_T* motor, float target_position, float target_velocity)
{
	if ((target_position - shaft_angle) < 1e-5) target_velocity = 0;
	printf("%.12f,%.12f\n", shaft_angle, target_position);
    float Uq;
	unsigned long timestamp_now = micros();
	float Ts = (timestamp_now - motor->timestamp_prev) * 1e-6f;
    // quick fix for strange cases (micros overflow)
    if (Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
	
    // 更新轴角度（归一化到 0~2π）
	motor->target_velocity = target_velocity;
	motor->velocity = getVelocity(motor);
//	float error = motor->target_velocity - motor->velocity;
//	float output = PIDController_compute(motor->PID_velocity, error);
//	printf("%.8f\n", output);
	motor->angle = getAngle()*0.001534;
    shaft_angle = _normalizeAngle(motor->angle + target_velocity * Ts);
    
    // 计算Q轴电压（简单开环控制）
    Uq = voltage_pow_supply;
    
    // 设置三相电压（需实现）
    setPhaseVoltage(Uq, 0.0f, _electricalAngle(shaft_angle, 7));
	
	motor->timestamp_prev = timestamp_now;
//	printf("%f\n", val);
}

/**
 * @brief 速度开环控制（HAL库版本）
 * @param target_velocity 目标速度（rad/s）
 * @return 输出的Q轴电压 Uq
 */
void VelocityOpenloop(MOTOR_T* motor, float target_velocity)
{
	unsigned long timestamp_now = micros();
	float Ts = (timestamp_now - motor->timestamp_prev) * 1e-6f;
    // quick fix for strange cases (micros overflow)
    if (Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
	
    // 更新轴角度（归一化到 0~2π）
	motor->target_velocity = target_velocity;
	motor->velocity = getVelocity(motor);
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);
	
	setPhaseVoltage(motor->DQvoltage.Uq, motor->DQvoltage.Ud, _electricalAngle(shaft_angle, Pole_Pairs));
	motor->timestamp_prev = timestamp_now;
}

/**
 * @brief 电流环控制（HAL库版本）
 * @param Target_Iq 目标电流（A）
 * @param Target_Id 目标电流（A）
 * @return 
 */
void Currentloop(MOTOR_T* motor, float Target_Iq, float Target_Id)
{
	// Iq、Id处理 + 限幅
	float errIq = Target_Iq - motor->DQcurrent.Iq;
	float errId = Target_Id - motor->DQcurrent.Id;
	motor->DQvoltage.Uq = PIDController_compute(motor->PID_current_q, errIq);
	motor->DQvoltage.Ud = PIDController_compute(motor->PID_current_d, errId);
	motor->DQvoltage.Uq = _constrain(motor->DQvoltage.Uq, -8, 8);
	motor->DQvoltage.Ud = _constrain(motor->DQvoltage.Ud, -8, 8);
	Voltageloop(motor, motor->target_velocity);
}

/**
 * @brief 开环电压控制（HAL库版本）
 * @param target_velocity 目标速度（rad/s）
 * @return 
 */
void Voltageloop(MOTOR_T* motor, float target_velocity)
{
	unsigned long timestamp_now = micros();
	float Ts = (timestamp_now - motor->timestamp_prev) * 1e-6f;
    // quick fix for strange cases (micros overflow)
    if (Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
    // 更新轴角度（归一化到 0~2π）
	motor->target_velocity = target_velocity;
	motor->velocity = getVelocity(motor);
	motor->velocity = LowPassFilter_Update(motor->LPF_velocity, motor->velocity);
//	printf("%f,%f\n", target_velocity, motor->velocity);
//	float error = motor->target_velocity - motor->velocity;
//	float output = PIDController_compute(motor->PID_velocity, error);
//	printf("%.8f\n", output);
    shaft_angle = _normalizeAngle(motor->angle + target_velocity * Ts);
//    printf("%.8f\n", motor->angle + target_velocity * Ts);
    // 设置三相电压（需实现）
    setPhaseVoltage(motor->DQvoltage.Uq, motor->DQvoltage.Ud, _electricalAngle(shaft_angle, Pole_Pairs));
	motor->timestamp_prev = timestamp_now;
}

/**
 * @brief 获取电机速度
 * @param motor
 * @return 输出当前速度 vel
 */
float getVelocity(MOTOR_T* motor)
{
	unsigned long timestamp_now = micros();
	float Ts = (timestamp_now - motor->timestamp_prev) * 1e-6f;
    // quick fix for strange cases (micros overflow)
    if (Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
	float vel;
	if(fabs(motor->angle - motor->angle_prev) > 0.5*_2PI){
		vel = fabs(_2PI + motor->angle - motor->angle_prev) / Ts;
	}else{
		vel = fabs(motor->angle - motor->angle_prev) / Ts;
	}
//	printf("%f\n", vel);
	motor->angle_prev = motor->angle;
	motor->theta_sum += (vel*Ts);
	return vel;
}

/**
* @brief 电机模式控制
 * @param motor
 * @return 
 */
void move(MOTOR_T* motor)
{
		switch(motor->controller)
		{
			case VoltageControlMode:
				motor->DQvoltage.Uq = 4;
				motor->DQvoltage.Ud = 0;
				motor->target_velocity = 0.1;
				Voltageloop(motor, motor->target_velocity);
				char buffer1[20], buffer2[20];
				sprintf(buffer1, "%.2f", motor->angle);
				sprintf(buffer2, "%.2f", motor->velocity);
				OLED_ShowString(1, 7, buffer1);
				OLED_ShowString(2, 7, buffer2);
//				printf("%f,%f\n", motor->DQcurrent.Id, motor->DQcurrent.Iq);
//	  		printf("%f,%f,%f\n", motor.current_sensor.current.u, motor.current_sensor.current.v, motor.current_sensor.current.w);
				break;
			case CurrentControlMode:
				motor->DQcurrent.target_Id = 1.5;
				motor->DQcurrent.target_Iq = 0;
				motor->target_velocity = 0.1;
				Currentloop(motor, motor->DQcurrent.target_Iq, motor->DQcurrent.target_Id);
//				printf("%f,%f\n", motor->DQvoltage.Uq, motor->DQvoltage.Ud);
				printf("%f,%f\n", motor->DQcurrent.Iq, motor->DQcurrent.Id);
				break;
			case VelocityControlMode:
				break;
			case PositionControlMode:
				break;
			default:
				break;
		}
}






