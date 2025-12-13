#ifndef _INA240_H
#define _INA240_H

#include "adc.h"
//#include "usart.h"

typedef struct _PhaseCurrent_T {
	float u;
	float v;
	float w;
} PhaseCurrent_T;

typedef struct _DQCurrent_T {
	float Id;
	float Iq;
	float target_Id;
	float target_Iq;
} DQCurrent_T;

typedef struct _DQVoltage_T {
	float Ud;
	float Uq;
} DQVoltage_T;

typedef struct _CURRENT_T {
		float CSA_GAIN;  //
		float gain_u, gain_v, gain_w;
		PhaseCurrent_T current;
		float offset_iu, offset_iv, offset_iw;
		int err_code;
		float shunt_resistor;
		float volte_to_amps_ratio;
	} CURRENT_T;

void CURRENT_Init(CURRENT_T *sensor);
void CURRENT_Update(CURRENT_T *sensor, double angle);
void CURRENT_CalibrateOffsets(CURRENT_T *sensor);

#endif
