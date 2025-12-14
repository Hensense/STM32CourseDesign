#ifndef __BH1750_H
#define __BH1750_H
#include "sys.h"

/***************根据自己需求更改****************/
//BH1750引脚宏定义
#define     BH1750_SCL_GPIO_CLK                RCC_APB2Periph_GPIOB
#define     BH1750_SCL_GPIO_PORT               GPIOB
#define     BH1750_SCL_GPIO_PIN                GPIO_Pin_13

#define     BH1750_SDA_GPIO_CLK                RCC_APB2Periph_GPIOB
#define     BH1750_SDA_GPIO_PORT               GPIOB
#define     BH1750_SDA_GPIO_PIN                GPIO_Pin_12

#define     BH1750_IIC_SCL                     PBout(13)   //SCL
#define     BH1750_IIC_SDA                     PBout(12)   //输出SDA
#define     BH1750_READ_SDA                    PBin(12)    //输入SDA 
/*********************END**********************/

#define BHAddWrite     0x46      //从机地址+最后写方向位
#define BHAddRead      0x47      //从机地址+最后读方向位
#define BHPowDown      0x00      //关闭模块
#define BHPowOn        0x01      //打开模块等待测量指令
#define BHReset        0x07      //重置数据寄存器值在PowerOn模式下有效
#define BHModeH1       0x10      //高分辨率 单位1lx 测量时间120ms
#define BHModeH2       0x11      //高分辨率模式2 单位0.5lx 测量时间120ms
#define BHModeL        0x13      //低分辨率 单位4lx 测量时间16ms
#define BHSigModeH     0x20      //一次高分辨率 测量 测量后模块转到 PowerDown模式
#define BHSigModeH2    0x21      //同上类似
#define BHSigModeL     0x23      // 上类似

void Single_Write_BH1750(uint8_t REG_Address);
void BH1750_Init(void);
void bh_data_send(uint8_t command);
uint16_t BH1750_Get_Light(void);

//IIC所有操作函数
void BH1750_IIC_Init(void);                 //初始化IIC的IO口
void BH1750_IIC_Start(void);                //发送IIC开始信号
void BH1750_IIC_Stop(void);                 //发送IIC停止信号
void BH1750_IIC_Send_Byte(uint8_t txd);     //IIC发送一个字节
uint8_t BH1750_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t BH1750_IIC_Wait_Ack(void);          //IIC等待ACK信号
void BH1750_IIC_Ack(void);                  //IIC发送ACK信号
void BH1750_IIC_NAck(void);                 //IIC不发送ACK信号

void BH1750_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t BH1750_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	

#endif
