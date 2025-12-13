#include "INA240.h"

void CURRENT_Init(CURRENT_T *sensor) {
	sensor->CSA_GAIN = 50.0f;
	sensor->shunt_resistor = 0.01f;  // 10毫欧
	sensor->volte_to_amps_ratio =
	  1.0f / sensor->shunt_resistor / sensor->CSA_GAIN;
	sensor->gain_u = sensor->volte_to_amps_ratio;
	sensor->gain_v = sensor->volte_to_amps_ratio;
	sensor->gain_w = sensor->volte_to_amps_ratio;
	sensor->offset_iu = 0.0f;
	sensor->offset_iv = 0.0f;
	sensor->offset_iw = 0.0f;
}

/**
 * @description: 计算零位飘移
 * @param {CURRENT_T*} sensor
 * @return {*}
 */
void CURRENT_CalibrateOffsets(CURRENT_T *sensor) {
//	const int calibration_rounds = 1000;
//	for (int i = 0; i < calibration_rounds; i++) {
//		sensor->offset_iu += hadc->Instance->JDR1;  // 读取Rank1（Channe4）数据
//		sensor->offset_iv += hadc->Instance->JDR2;  // 读取Rank2（Channe5）数据
//		sensor->offset_iw += hadc->Instance->JDR3;  // 读取Rank3（Channe6）数据
//	}
//	sensor->offset_iu /= calibration_rounds;
//	sensor->offset_iv /= calibration_rounds;
//	sensor->offset_iw /= calibration_rounds;
//	printf("%f,%f,%f\n", sensor->offset_iu, sensor->offset_iv, sensor->offset_iw);
}

void CURRENT_Update(CURRENT_T *sensor, double angle) {
//	current_clarke[0] = sensor->current.u - sensor->current.v/2 - sensor->current.w/2;
//	current_clarke[1] = _SQRT3*sensor->current.v/2 - _SQRT3*sensor->current.w/2;
//	cos(val)*
}


