#ifndef __SHT30_H
#define    __SHT30_H

#include "sys.h"

/***************根据自己需求更改****************/
// CSHT30 GPIO宏定义

#define     SHT30_IIC_CLK              RCC_APB2Periph_GPIOB
#define     SHT30_IIC_PORT             GPIOB
#define     SHT30_IIC_SDA_PIN          GPIO_Pin_8    
#define     SHT30_IIC_SCL_PIN          GPIO_Pin_9    

//IO操作函数     
#define  SHT30_IIC_SDA_H               GPIO_SetBits(SHT30_IIC_PORT,SHT30_IIC_SDA_PIN)
#define  SHT30_IIC_SDA_L               GPIO_ResetBits(SHT30_IIC_PORT,SHT30_IIC_SDA_PIN)
                 
#define  SHT30_IIC_SCL_H               GPIO_SetBits(SHT30_IIC_PORT,SHT30_IIC_SCL_PIN)
#define  SHT30_IIC_SCL_L               GPIO_ResetBits(SHT30_IIC_PORT,SHT30_IIC_SCL_PIN) 

#define SHT30_READ_SDA                 GPIO_ReadInputDataBit(SHT30_IIC_PORT, SHT30_IIC_SDA_PIN)         //输入SDA 

/*********************END**********************/

#define SHT30_ADDR (uint8_t)(0x44<<1) //sht30 i2c地址，ADDR管脚接低电平时为0x44,接高电平为0x45

//SHT30命令
#define SHT30_READ_HUMITURE (uint16_t)0x2c06  //读温湿度

//CRC多项式
#define POLYNOMIAL 0x31 // X^8 + X^5 + X^4 + 1

//SHT30

void SHT30_Init(void);
void SHT30_IIC_SDA_IN(void);
void SHT30_IIC_SDA_OUT(void);
    

void SHT30_IIC_Init(void);
void SHT30_IIC_start(void); 
void SHT30_IIC_stop(void);
uint8_t SHT30_IIC_Get_ack(void);
void SHT30_IIC_ACK(void);
void SHT30_IIC_NACK(void);
void SHT30_IIC_write_byte(uint8_t Data);
uint8_t SHT30_IIC_read_byte(uint8_t ack);



/**********************************
函数声明
**********************************/
unsigned char SHT3X_CRC(uint8_t *data, uint8_t len);
void SHT30_CMD(uint16_t cmd);
u8 SHT30_Read_Humiture(int *temp,uint16_t *humi);


#endif



