#ifndef _AS5600_H
#define _AS5600_H

#include "i2c.h"
#include "usart.h"

#define I2C_TIMEOUT 100  // 超时时间（ms）
#define AS5600_Add 0x6c
#define Raw_Add 0x0c

void I2C_ScanDevices(I2C_HandleTypeDef *hi2c);
uint16_t getAngle(void);


#endif
