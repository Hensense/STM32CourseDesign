#ifndef INA226_H
#define INA226_H
#include "sys.h"
/*******************************根据需求定义***************************************/
#define         INA226_IIC_SCL_CLK             RCC_APB2Periph_GPIOB
#define         INA226_IIC_SCL_PORT            GPIOB
#define         INA226_IIC_SCL_PIN             GPIO_Pin_10

#define         INA226_IIC_SDA_CLK             RCC_APB2Periph_GPIOB
#define         INA226_IIC_SDA_PORT            GPIOB
#define         INA226_IIC_SDA_PIN             GPIO_Pin_11
/**********************************************************************/
#define READ_ADDR                  0x81	 //A0=GND，A1=GND // R=1, W=0
#define WRITE_ADDR                 0x80

#define Config_Reg                 0x00
#define Shunt_V_Reg                0x01
#define Bus_V_Reg                  0x02
#define Power_Reg                  0x03
#define Current_Reg                0x04
#define Calib_Reg                  0x05
#define Mask_En_Reg                0x06
#define Alert_Reg                  0x07
#define Man_ID_Reg                 0xFE  //0x5449
#define ID_Reg                     0xFF  //0x2260

void INA226_Init(void);
float INA226_GetVoltage(void);
float INA226_GetCurrent(void);
float INA226_GetPower(void);
float INA226_GetShuntVoltage(void);
#endif
