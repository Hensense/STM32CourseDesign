#include "tds.h"
/* 水质传感器 */
void TDS_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (TDS_GPIO_CLK, ENABLE );         // 打开 ADC IO端口时钟
    GPIO_InitStructure.GPIO_Pin = TDS_GPIO_PIN;             // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // 设置为模拟输入
    
    GPIO_Init(TDS_GPIO_PORT, &GPIO_InitStructure);          // 初始化 ADC IO
}

float TDS_GetData_PPM(void)
{
    float TDS_DAT;

    uint16_t adc_sum = 0;
    
    for(uint8_t i = 0; i < 10; i++)
    {
        adc_sum += ADC_ConvertedValue[TDS_ADC_CHANNEL + i * 10] & 0x0FFF;
    }
    adc_sum /= 10;
    
    TDS_DAT = (adc_sum/4095.0)*3.3;
    TDS_DAT = 66.71*TDS_DAT*TDS_DAT*TDS_DAT-127.93*TDS_DAT*TDS_DAT+428.7*TDS_DAT;
    if(TDS_DAT<20) TDS_DAT = 0;
    
    return TDS_DAT;

}



