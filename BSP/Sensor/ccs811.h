#ifndef __CCS811_H
#define    __CCS811_H

#include "sys.h"

/***************根据自己需求更改****************/
// CCS811 GPIO宏定义

#define     CCS811_IIC_SCL_CLK                                RCC_APB2Periph_GPIOB
#define     CCS811_IIC_SCL_PORT                               GPIOB
#define     CCS811_IIC_SDA_PIN                                GPIO_Pin_7    

#define     CCS811_IIC_SDA_CLK                                RCC_APB2Periph_GPIOB
#define     CCS811_IIC_SDA_PORT                               GPIOB
#define     CCS811_IIC_SCL_PIN                                GPIO_Pin_6    

#define     CCS811_WAK_CLK                                    RCC_APB2Periph_GPIOB
#define     CCS811_WAK_PORT                                   GPIOB
#define     CCS811_WAK_PIN                                    GPIO_Pin_5

//IO操作函数     
#define CCS811_IIC_SCL    PBout(6)         //SCL
#define CCS811_IIC_SDA    PBout(7)         //SDA     
#define CCS811_READ_SDA   PBin(7)         //输入SDA 


/*********************END**********************/

//CCS811
#define CCS811_Add  0x5A<<1
#define STATUS_REG 0x00                        //状态寄存器
#define MEAS_MODE_REG 0x01                //测量模式和条件寄存器
#define ALG_RESULT_DATA 0x02            //算法结果。最高有效 2 个字节包含等效 CO2 （eCO2） 水平的 ppm 估计值，最低有效 2 个字节包含总 VOC 水平的 ppb 估计值
#define ENV_DATA 0x05
#define NTC_REG 0x06
#define THRESHOLDS 0x10
#define BASELINE 0x11
#define HW_ID_REG 0x20                        //硬件 ID 值为 0x81
#define ERROR_ID_REG 0xE0                    //错误 ID。当状态寄存器报告错误时，它的源位于此寄存器中
#define APP_START_REG 0xF4
#define SW_RESET 0xFF
#define CCS_811_ADDRESS 0x5A
#define GPIO_WAKE 0x5
#define DRIVE_MODE_IDLE 0x0
#define DRIVE_MODE_1SEC 0x10
#define DRIVE_MODE_10SEC 0x20
#define DRIVE_MODE_60SEC 0x30
#define INTERRUPT_DRIVEN 0x8
#define THRESHOLDS_ENABLED 0x4

void CCS811_Init(void);
void ON_CCS811(void);
void CCS811_EN(void);
void OFF_CCS811(void);

u16 CCS811_GetValue(void);


uint8_t CCS811_Single_WriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t data);
uint8_t CCS811_Single_MWriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t const *data,uint8_t length);
uint8_t CCS811_Single_ReadI2C(uint8_t Slave_Address,uint8_t REG_Address,uint8_t *REG_data,uint8_t length);

#endif



