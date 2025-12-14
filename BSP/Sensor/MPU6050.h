#ifndef __MPU6050_H
#define __MPU6050_H
/* 测试情况：本文件已完成测试，功能一切正常 */
/* 数据显示 */
//OLED_Printf(0,0,OLED_6X8,"Ax:%04d Ay:%04d",MPU6050_DataInfo.AccX,MPU6050_DataInfo.AccY);
//OLED_Printf(0,16,OLED_6X8,"Az:%04d",MPU6050_DataInfo.AccZ);
//OLED_Printf(0,32,OLED_6X8,"Gx:%04d Gy:%04d",MPU6050_DataInfo.GyroX,MPU6050_DataInfo.GyroY);
//OLED_Printf(0,48,OLED_6X8,"Gz:%04d",MPU6050_DataInfo.GyroZ);
/* 跌倒检测标志 */
//OLED_Printf(0,0,OLED_8X16,"Fall_Down:%s",MPU6050_DataInfo.Fall_Down_Flag == 1?"TRUE":"FALSE");
#include "sys.h"
/* 引脚宏定义 开始：可自由切换除已使用的任意IO引脚 */
#define         MPU6050_IIC_SCL_CLK             RCC_APB2Periph_GPIOA
#define         MPU6050_IIC_SCL_PORT            GPIOA
#define         MPU6050_IIC_SCL_PIN             GPIO_Pin_1

#define         MPU6050_IIC_SDA_CLK             RCC_APB2Periph_GPIOA
#define         MPU6050_IIC_SDA_PORT            GPIOA
#define         MPU6050_IIC_SDA_PIN             GPIO_Pin_2
/* 引脚宏定义 结束 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetValue(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
