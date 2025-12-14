#ifndef __SGP30_H__
#define __SGP30_H__

#include "sys.h"

// 硬件配置宏定义 - 根据需要修改
#define SGP30_IIC_SDA_RCC       RCC_APB2Periph_GPIOB
#define SGP30_IIC_SCL_PORT      GPIOB
#define SGP30_IIC_SCL_PIN       GPIO_Pin_10

#define SGP30_IIC_SDA_RCC       RCC_APB2Periph_GPIOB
#define SGP30_IIC_SDA_PORT      GPIOB
#define SGP30_IIC_SDA_PIN       GPIO_Pin_11


// IIC时序延迟(us)
#define IIC_DELAY_US            20

// SGP30地址定义
#define SGP30_READ_ADDR         0xB1
#define SGP30_WRITE_ADDR        0xB0

// 函数声明
void SGP30_Init(void);
void SGP30_GetValue(uint16_t* co2, uint16_t* tvoc);

#endif
