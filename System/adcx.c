#include "adcx.h"
#include <string.h>

/* 全局变量 */
volatile uint8_t adc_data_ready = 0;
volatile uint16_t ADC_ConvertedValue[ADC_BUFFER_SIZE] __attribute__((aligned(4))); // DMA要求4字节对齐

/**
  * @brief  ADC基础初始化
  */
void ADCx_Init(void)
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(ADC_CLK | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    /* 配置ADC时钟（6分频） */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /* 配置ADC参数 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADCx, &ADC_InitStructure);
    
    /* 启用ADC */
    ADC_Cmd(ADCx, ENABLE);
    
    /* 校准ADC */
    ADC_ResetCalibration(ADCx);
    while(ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);
    while(ADC_GetCalibrationStatus(ADCx));
}

/**
  * @brief  DMA初始化
  */
void ADC_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    /* 开启DMA时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* 配置DMA参数 */
    DMA_DeInit(ADC_DMA_CHANNEL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADCx->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
    
    /* 启用DMA中断 */
    DMA_ITConfig(ADC_DMA_CHANNEL, DMA_IT_TC, ENABLE);
    NVIC_EnableIRQ(ADC_DMA_IRQn);
    
    /* 启动DMA */
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
    ADC_DMACmd(ADCx, ENABLE);
}

/**
  * @brief  启动自动多通道采集
  */
void ADC_StartAutoConvert(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    
    /* 配置多通道扫描 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUM;
    ADC_Init(ADCx, &ADC_InitStructure);
    
    /* 配置所有通道 */
    for(uint8_t i=0; i<ADC_CHANNEL_NUM; i++) {
        ADC_RegularChannelConfig(ADCx, i, i+1, ADC_SampleTime_55Cycles5);
    }
    
    /* 启动转换 */
    ADC_Cmd(ADCx, ENABLE);
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/**
  * @brief  获取ADC值（兼容DMA/非DMA模式）
  */
uint16_t ADC_GetValue(uint8_t channel, uint8_t sample_time)
{
    /* DMA模式：返回最近10次采样平均值 */
    uint32_t sum = 0;
    for(int i=0; i<SAMPLE_TIMES; i++) {
        sum += ADC_ConvertedValue[channel + SAMPLE_TIMES * i];
    }
    return sum / SAMPLE_TIMES;
}
/**
  * @brief  获取ADC转换后的数据
  * @param  ADC_Channel 	选择需要采集的ADC通道
  * @param  ADC_SampleTime  选择需要采样时间
  * @retval 返回转换后的模拟信号数值
  */
uint16_t ADC_GetValue1(uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
    //配置ADC通道
    ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime);
    
    ADC_SoftwareStartConvCmd(ADCx, ENABLE); //软件触发ADC转换
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET); //读取ADC转换完成标志位
    return ADC_GetConversionValue(ADCx);
}
/**
  * @brief  高级初始化（自动识别模式）
  */
void ADCx_AdvancedInit(void)
{
    ADCx_Init();
    ADC_DMA_Init();
    ADC_StartAutoConvert();
}

/**
  * @brief  DMA中断服务函数
  */
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1)) {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        adc_data_ready = 1; // 设置数据更新标志
    }
}

/* 暂停DMA 用于处理转运完成的数据 */
void ADC_DMA_Pause(void) {
    while(DMA_GetCurrDataCounter(ADC_DMA_CHANNEL) == 0); // 等待当前传输完成
    ADC_Cmd(ADCx, DISABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, DISABLE);
    ADC_DeInit(ADCx);
}

/* 恢复DMA */
void ADC_DMA_Resume(void) {
    ADCx_AdvancedInit();
}

