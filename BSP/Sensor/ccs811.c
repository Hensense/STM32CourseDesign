#include "ccs811.h"

uint8_t MeasureMode,Status,Error_ID;
uint8_t Information[10];
uint8_t BUF[12];

typedef struct {
    uint16_t eco2;
    uint16_t tvoc;
    uint8_t status;
    uint8_t error_id;
    uint16_t raw_data;
}ccs811_measurement_t;
ccs811_measurement_t CCS;


uint16_t car_num;


void CCS811_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(CCS811_IIC_SCL_CLK | CCS811_IIC_SDA_CLK,ENABLE);//先使能外设IO PORTB时钟 

    GPIO_InitStructure.GPIO_Pin = CCS811_IIC_SCL_PIN;     // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //IO口速度为50MHz
    GPIO_Init(CCS811_IIC_SCL_PORT, &GPIO_InitStructure);                     //根据设定参数初始化GPIO 

    GPIO_InitStructure.GPIO_Pin = CCS811_IIC_SDA_PIN;     // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //IO口速度为50MHz
    GPIO_Init(CCS811_IIC_SDA_PORT, &GPIO_InitStructure);                     //根据设定参数初始化GPIO 
    
    CCS811_IIC_SCL = 1;
    CCS811_IIC_SDA = 1;

    CCS811_EN();
    CCS811_Single_ReadI2C(CCS811_Add,0x00,&Status,1);
    CCS811_Single_ReadI2C(CCS811_Add,0xE0,&Error_ID,1);
    CCS811_Single_ReadI2C(CCS811_Add,0x02,BUF,8);
    CCS811_Single_ReadI2C(CCS811_Add,0x20,Information,1);  //Read CCS's information  ,ID
}

//CCS811引脚输出模式控制
void CCS811_IIC_SDA_OUT(void)//SDA输出方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=CCS811_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA推挽输出
    GPIO_Init(CCS811_IIC_SDA_PORT,&GPIO_InitStructure);                         

}

void CCS811_IIC_SDA_IN(void)//SDA输入方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=CCS811_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL上拉输入
    GPIO_Init(CCS811_IIC_SDA_PORT,&GPIO_InitStructure);
    
}

uint16_t CCS811_GetValue(void)
{
    int car;
    //CCS811  CO2数据采集
    ON_CCS811();                            //nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low     
    CCS811_Single_ReadI2C(CCS811_Add,0x00,&Status,1);
    CCS811_Single_ReadI2C(CCS811_Add,0xE0,&Error_ID,1);
    CCS811_Single_ReadI2C(CCS811_Add,0x02,BUF,8);
    CCS811_Single_ReadI2C(CCS811_Add,0x20,Information,1);  //Read CCS's information  ,ID
    OFF_CCS811(); 
    CCS.eco2= (uint16_t)BUF[0]*256+BUF[1];
    CCS.tvoc= (uint16_t)BUF[2]*256+BUF[3];
    Information[0]=0;
    car=(float)CCS.eco2;  //二氧化碳

    return car;
}



////////IIC起始函数//////////
/*
IIC起始:当SCL处于高电平期间，SDA由高电平变成低电平出现一个下降沿，然后SCL拉低
*/
uint8_t CCS811_IIC_Start(void)
{
    CCS811_IIC_SDA_OUT();
    CCS811_IIC_SDA = 1; 
    delay_us(5);    //延时保证时钟频率低于40K，以便从机识别
    CCS811_IIC_SCL = 1;
    delay_us(5);//延时保证时钟频率低于40K，以便从机识别
    //if(!CCS811_READ_SDA) return 0;//SDA线为低电平则总线忙,退出
    CCS811_IIC_SDA = 0;   //SCL处于高电平的时候，SDA拉低
    delay_us(5);
    //if(CCS811_READ_SDA) return 0;//SDA线为高电平则总线出错,退出
    CCS811_IIC_SCL = 0;
    delay_us(5);
    return 1;
}
//**************************************
//IIC停止信号
/*
IIC停止:当SCL处于高电平期间，SDA由低电平变成高电平出现一个上升沿
*/
//**************************************
void CCS811_IIC_Stop(void)
{
    CCS811_IIC_SDA_OUT();
    CCS811_IIC_SDA = 0;
    CCS811_IIC_SCL = 0;
    delay_us(5);
    CCS811_IIC_SCL = 1;
    delay_us(5);
    CCS811_IIC_SDA = 1;//当SCL处于高电平期间，SDA由低电平变成高电平             //延时
}
//**************************************
//IIC发送应答信号
//入口参数:ack (0:ACK 1:NAK)
/*
应答：当从机接收到数据后，向主机发送一个低电平信号
先准备好SDA电平状态，在SCL高电平时，主机采样SDA
*/
//**************************************
void CCS811_IIC_SendACK(uint8_t i)
{
    CCS811_IIC_SDA_OUT();
    if(1==i)
        CCS811_IIC_SDA = 1;                 //准备好SDA电平状态，不应答
    else 
        CCS811_IIC_SDA = 0;                          //准备好SDA电平状态，应答     
    CCS811_IIC_SCL = 1;                    //拉高时钟线
    delay_us(5);                 //延时
    CCS811_IIC_SCL = 0 ;                  //拉低时钟线
    delay_us(5);    
} 
///////等待从机应答////////
/*
当本机(主机)发送了一个数据后，等待从机应答
先释放SDA，让从机使用，然后采集SDA状态
*/
/////////////////
uint8_t CCS811_IIC_WaitAck(void)      //返回为:=1有ACK,=0无ACK
{    
    uint16_t i=0;
    CCS811_IIC_SDA_IN();
    CCS811_IIC_SDA = 1;delay_us(1);            //释放SDA
    CCS811_IIC_SCL = 1;delay_us(1);         //SCL拉高进行采样
    while(CCS811_READ_SDA)//等待SDA拉低
    {
        i++;      //等待计数
        if(i==500)//超时跳出循环
        break;
        
    }
    if(CCS811_READ_SDA)//再次判断SDA是否拉低
    {
        CCS811_IIC_SCL = 0; 
        return RESET;//从机应答失败，返回0
    }
    delay_us(5);//延时保证时钟频率低于40K，
    CCS811_IIC_SCL = 0;
    delay_us(5); //延时保证时钟频率低于40K，
    CCS811_IIC_SDA_OUT();
    return SET;//从机应答成功，返回1
}
//**************************************
//向IIC总线发送一个字节数据
/*
一个字节8bit,当SCL低电平时，准备好SDA，SCL高电平时，从机采样SDA
*/
//**************************************
void CCS811_IIC_SendByte(uint8_t dat)
{   
    uint8_t i;
    CCS811_IIC_SDA_OUT();
    CCS811_IIC_SCL = 0;//SCL拉低，给SDA准备
    for (i=0; i<8; i++)         //8位计数器
    {
        if(dat&0x80)//SDA准备
            CCS811_IIC_SDA = 1;  
        else 
            CCS811_IIC_SDA = 0;
        CCS811_IIC_SCL = 1;                 //拉高时钟，给从机采样
        delay_us(5);                        //延时保持IIC时钟频率，也是给从机采样有充足时间
        CCS811_IIC_SCL = 0;                 //拉低时钟，给SDA准备
        delay_us(5);                        //延时保持IIC时钟频率
        dat <<= 1;                          //移出数据的最高位  
    }
    delay_us(10);
}
//**************************************
//从IIC总线接收一个字节数据
//**************************************
uint8_t CCS811_IIC_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    CCS811_IIC_SDA_IN();
    CCS811_IIC_SDA = 1;//释放SDA，给从机使用
    delay_us(5);         //延时给从机准备SDA时间            
    for (i=0; i<8; i++)         //8位计数器
    { 
        dat <<= 1;
        CCS811_IIC_SCL = 1;                //拉高时钟线，采样从机SDA
        if(CCS811_READ_SDA) //读数据    
           dat |=0x01;      
        delay_us(5);     //延时保持IIC时钟频率        
        CCS811_IIC_SCL = 0;           //拉低时钟线，处理接收到的数据
        delay_us(5);   //延时给从机准备SDA时间
    } 
    return dat;
}
//**************************************
//向IIC设备写入一个字节数据
//**************************************
uint8_t CCS811_Single_WriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t data)
{
    if(CCS811_IIC_Start()==0)  //起始信号
    {
        CCS811_IIC_Stop(); 
        return RESET;
    }

    CCS811_IIC_SendByte(Slave_Address);   //发送设备地址+写信号
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}

    CCS811_IIC_SendByte(REG_Address);    //内部寄存器地址，
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}

    CCS811_IIC_SendByte(data);       //内部寄存器数据，
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}

    CCS811_IIC_Stop();   //发送停止信号

    return SET;
}

uint8_t CCS811_Single_MWriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t const *data,uint8_t length)
{
    if(CCS811_IIC_Start()==0)  //起始信号
    {CCS811_IIC_Stop();return RESET;}           

    CCS811_IIC_SendByte(Slave_Address);   //发送设备地址+写信号
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop();return RESET;}

    CCS811_IIC_SendByte(REG_Address);    //内部寄存器地址，
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop();return RESET;}

    while(length)
    {
        CCS811_IIC_SendByte(*data++);       //内部寄存器数据，
        if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}           //应答
        length--;
    }
    //    CCS811_IIC_SendByte(*data);       //内部寄存器数据，
    //    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}
    CCS811_IIC_Stop();   //发送停止信号        
    return SET;
}

//**************************************
//从IIC设备读取一个字节数据
//**************************************
uint8_t CCS811_Single_ReadI2C(uint8_t Slave_Address,uint8_t REG_Address,uint8_t *REG_data,uint8_t length)
{
    if(CCS811_IIC_Start()==0)  //起始信号
    {CCS811_IIC_Stop();return RESET;}          
     
    CCS811_IIC_SendByte(Slave_Address);    //发送设备地址+写信号
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop()    ;return RESET;} 

    CCS811_IIC_SendByte(REG_Address);     //发送存储单元地址
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop();return RESET;} 

    if(CCS811_IIC_Start()==0)  //起始信号
            {CCS811_IIC_Stop(); return RESET;}            

    CCS811_IIC_SendByte(Slave_Address+1);  //发送设备地址+读信号
    if(!CCS811_IIC_WaitAck()){CCS811_IIC_Stop(); return RESET;}

    while(length-1)
    {
        *REG_data++=CCS811_IIC_RecvByte();       //读出寄存器数据
        CCS811_IIC_SendACK(0);               //应答
        length--;
    }
    *REG_data=CCS811_IIC_RecvByte();  
    CCS811_IIC_SendACK(1);     //发送停止传输信号
    CCS811_IIC_Stop();                    //停止信号
    return SET;
}




void CCS811_EN()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(CCS811_WAK_CLK,ENABLE);//先使能外设IO PORTB时钟 
        
    GPIO_InitStructure.GPIO_Pin = CCS811_WAK_PIN;     // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //IO口速度为50MHz
    GPIO_Init(CCS811_WAK_PORT, &GPIO_InitStructure);                     //根据设定参数初始化GPIO 

    GPIO_ResetBits(CCS811_WAK_PORT,CCS811_WAK_PIN);
}

void ON_CCS811()
{
    GPIO_ResetBits(CCS811_WAK_PORT,CCS811_WAK_PIN);
}
void OFF_CCS811()
{
    GPIO_SetBits(CCS811_WAK_PORT,CCS811_WAK_PIN);
}


