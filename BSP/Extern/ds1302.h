#ifndef __DS1302_H 
#define __DS1302_H  
 
#include "sys.h" 

#define DS1302_CE_CLK                       RCC_APB2Periph_GPIOA
#define DS1302_CE_PORT                      GPIOA
#define DS1302_CE_PIN                       GPIO_Pin_5

#define DS1302_DATA_CLK                     RCC_APB2Periph_GPIOB
#define DS1302_DATA_PORT                    GPIOB
#define DS1302_DATA_PIN                     GPIO_Pin_6

#define DS1302_SCLK_CLK                     RCC_APB2Periph_GPIOC
#define DS1302_SCLK_PORT                    GPIOC
#define DS1302_SCLK_PIN                     GPIO_Pin_7

#define CE_L                GPIO_ResetBits(DS1302_CE_PORT,DS1302_CE_PIN)//CE
#define CE_H                GPIO_SetBits(DS1302_CE_PORT,DS1302_CE_PIN)

#define DATA_L              GPIO_ResetBits(DS1302_SCLK_PORT,DS1302_DATA_PIN)//DATA
#define DATA_H              GPIO_SetBits(DS1302_SCLK_PORT,DS1302_DATA_PIN)

#define SCLK_L              GPIO_ResetBits(DS1302_DATA_PORT,DS1302_SCLK_PIN)//SCLK
#define SCLK_H              GPIO_SetBits(DS1302_DATA_PORT,DS1302_SCLK_PIN)
 
struct TIMEData
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint8_t  week;
};//创建TIMEData结构体方便存储时间日期数据

struct TIMERAM
{
    uint8_t  hour_kai;
    uint8_t  minute_kai;
    uint8_t  hour_guan;
    uint8_t  minute_guan;
    uint8_t  kai;
    uint8_t  guan;
};//创建TIMEData结构体方便存储时间日期数据

extern struct TIMERAM TimeRAM;
extern struct TIMEData TimeData;//全局变量

void DS1302_GPIO_Init(void);//ds1302端口初始化
void DS1302_write_onebyte(uint8_t data);//向ds1302发送一字节数据
void DS1302_wirte_rig(uint8_t address,uint8_t data);//向指定寄存器写一字节数据
uint8_t      DS1302_read_rig(uint8_t address);//从指定寄存器读一字节数据
void DS1302_Init(void);//ds1302初始化函数
void DS1302_DATAOUT_init(void);//IO端口配置为输出
void DS1302_DATAINPUT_init(void);//IO端口配置为输入
void DS1302_read_time(void);//从ds1302读取实时时间（BCD码）
void DS1302_read_realTime(void);//将BCD码转化为十进制数据
void DS1302_wirteRAM(void);
void DS1302_readRAM(void);
 
#endif






