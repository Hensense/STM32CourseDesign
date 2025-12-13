/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_InjectionConfTypeDef sConfigInjected = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_4;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
  sConfigInjected.InjectedNbrOfConversion = 3;
  sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;
  sConfigInjected.AutoInjectedConv = DISABLE;
  sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
  sConfigInjected.InjectedOffset = 0;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_5;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_2;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_6;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_3;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA4     ------> ADC1_IN4
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA4     ------> ADC1_IN4
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#include "bldc_motor.h"
#include "lowpass_filter_interface.h"
#include "SGSmooth_Interface.h"
#include "gpio.h"
__attribute__((section("ccmram")))
int ind = 0;
float val[3];
uint16_t value[3];
float current_clarke[2];
double value_data[100];
double value_data_filter[100];
extern MOTOR_T motor;;
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc1)
	{
//		printf("%u,%u\n", __HAL_TIM_GET_COUNTER(&htim1), __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_4));
		value[0] = hadc->Instance->JDR1;  // 读取Rank1（Channel4）数据
		value[1] = hadc->Instance->JDR2;  // 读取Rank2（Channel5）数据
		value[2] = hadc->Instance->JDR3;  // 读取Rank3（Channel6）数据
		if(ind < calibration_rounds){
			motor.current_sensor.offset_iu += (value[0] / calibration_rounds);
			motor.current_sensor.offset_iv += (value[1] / calibration_rounds);
			motor.current_sensor.offset_iw += (value[2] / calibration_rounds);
			ind++;
		}
		else{
//			motor.current_sensor.current.u = (value[0]) * 3.3f / 4096.0f;
//			motor.current_sensor.current.v = (value[1]) * 3.3f / 4096.0f;
//			motor.current_sensor.current.w = (value[2]) * 3.3f / 4096.0f;
			motor.current_sensor.current.u = (value[0] - motor.current_sensor.offset_iu) * 3.3f / 4096.0f * motor.current_sensor.CSA_GAIN;
//			motor.current_sensor.current.v = (value[1] - motor.current_sensor.offset_iv) * 3.3f / 4096.0f;
			motor.current_sensor.current.w = (value[2] - motor.current_sensor.offset_iw) * 3.3f / 4096.0f * motor.current_sensor.CSA_GAIN;
			motor.current_sensor.current.v = -(motor.current_sensor.current.u + motor.current_sensor.current.w);
			current_clarke[0] = motor.current_sensor.current.u 
							- motor.current_sensor.current.v/2 
							- motor.current_sensor.current.w/2;
			current_clarke[1] = _SQRT3*motor.current_sensor.current.v/2
							- _SQRT3*motor.current_sensor.current.w/2;
//			current_clarke[0] = motor.current_sensor.current.u;
//			current_clarke[1] = (motor.current_sensor.current.u + motor.current_sensor.current.v*2.0f)*0.57735f;
			motor.angle = getAngle()*0.001534;
			float angle_tmp = _normalizeAngle(_electricalAngle(motor.angle + motor.sensor_offset, Pole_Pairs));
			motor.DQcurrent.Id = cos(angle_tmp)*current_clarke[0]+sin(angle_tmp)*current_clarke[1];
			motor.DQcurrent.Iq = -sin(angle_tmp)*current_clarke[0]+cos(angle_tmp)*current_clarke[1];
			motor.DQcurrent.Id = LowPassFilter_Update(motor.LPF_current_d, motor.DQcurrent.Id);
			motor.DQcurrent.Iq = LowPassFilter_Update(motor.LPF_current_q, motor.DQcurrent.Iq);
//			val[0] = process_sensor_data(value[0]);
//			val[2] = process_sensor_data(value[2]);
//			printf("%f,%f\n", motor.DQcurrent.Id, motor.DQcurrent.Iq);
//			printf("%u,%u,%f,%f\n", value[0], value[2], val[0], val[2]);
//			printf("%u,%u,%u\n", value[0], value[1], value[2]);
//			printf("%f,%f,%f\n", motor.current_sensor.current.u, motor.current_sensor.current.v, motor.current_sensor.current.w);
//			printf("%f,%f\n", current_clarke[0], current_clarke[1]);
			move(&motor);
		}
	}
}
/* USER CODE END 1 */
