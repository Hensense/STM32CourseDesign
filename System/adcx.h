#ifndef __ADCX_H
#define __ADCX_H

#include "stm32f10x.h"

/* 硬件相关定义 */
#define ADCx                ADC1
#define ADC_CLK             RCC_APB2Periph_ADC1
#define ADC_DMA_CHANNEL     DMA1_Channel1
#define ADC_DMA_IRQn        DMA1_Channel1_IRQn

/* 参数配置 */
#define ADC_CHANNEL_NUM     10      // 使用IN0-IN9
#define SAMPLE_TIMES        10      // 每通道采样次数
#define ADC_BUFFER_SIZE     (ADC_CHANNEL_NUM * SAMPLE_TIMES)

/* 函数声明 */
void ADCx_Init(void);
void ADCx_AdvancedInit(void);
uint16_t ADC_GetValue(uint8_t channel, uint8_t sample_time);
uint16_t ADC_GetValue1(uint8_t ADC_Channel,uint8_t ADC_SampleTime);

extern volatile uint8_t adc_data_ready;
extern volatile uint16_t ADC_ConvertedValue[];
void ADC_DMA_Pause(void);    // 暂停DMA
void ADC_DMA_Resume(void);   // 恢复DMA

#endif
