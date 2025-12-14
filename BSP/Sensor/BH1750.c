#include "BH1750.h"

typedef   unsigned char BYTE;

/**
**  设置SDA为输出
**/
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= BH1750_SDA_GPIO_PIN;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(BH1750_SDA_GPIO_PORT, &GPIO_InitStructer);
}


/**
**  设置SDA为输入
**/
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= BH1750_SDA_GPIO_PIN;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(BH1750_SDA_GPIO_PORT, &GPIO_InitStructer);
}


void Single_Write_BH1750(uint8_t REG_Address)
{
   BH1750_IIC_Start();                      //起始信号
   BH1750_IIC_Send_Byte(BHAddWrite);        //发送设备地址+写信号
   BH1750_IIC_Send_Byte(REG_Address);       //内部寄存器地址，
  //  BH1750_SendByte(REG_data);            //内部寄存器数据，
   BH1750_IIC_Stop();                       //发送停止信号
}

void BH1750_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(BH1750_SCL_GPIO_CLK | BH1750_SDA_GPIO_CLK, ENABLE);     //使能时钟
    
    GPIO_InitStructure.GPIO_Pin = BH1750_SCL_GPIO_PIN;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
    GPIO_Init(BH1750_SCL_GPIO_PORT, &GPIO_InitStructure);       //初始化GPIO
    
    GPIO_InitStructure.GPIO_Pin = BH1750_SDA_GPIO_PIN;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
    GPIO_Init(BH1750_SDA_GPIO_PORT, &GPIO_InitStructure);       //初始化GPIO
    
    GPIO_SetBits(BH1750_SCL_GPIO_PORT,BH1750_SCL_GPIO_PIN);    
    GPIO_SetBits(BH1750_SDA_GPIO_PORT,BH1750_SDA_GPIO_PIN); 
    
    //Single_Write_BH1750(0x01);
    bh_data_send(BHPowOn);  //BH1750上电
    bh_data_send(BHReset);    //BH1750复位
    bh_data_send(BHModeH2); //BH1750写入测量模式：选择的是分辨率是4lx，测量时间是16ms的
}

//产生IIC起始信号
void BH1750_IIC_Start(void)
{
    SDA_OUT();//sda线输出
    BH1750_IIC_SDA=1;
    BH1750_IIC_SCL=1;
    delay_us(4);
    BH1750_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    BH1750_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void BH1750_IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    BH1750_IIC_SCL=0;
    BH1750_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    BH1750_IIC_SCL=1; 
    BH1750_IIC_SDA=1;//发送I2C总线结束信号
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t BH1750_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      //SDA设置为输入  
    BH1750_IIC_SDA=1;delay_us(1);       
    BH1750_IIC_SCL=1;delay_us(1);     
    while(BH1750_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            BH1750_IIC_Stop();
            return 1;
        }
    }
    BH1750_IIC_SCL=0;//时钟输出0        
    return 0;  
} 
//产生ACK应答
void BH1750_IIC_Ack(void)
{
    BH1750_IIC_SCL=0;
    SDA_OUT();
    BH1750_IIC_SDA=0;
    delay_us(2);
    BH1750_IIC_SCL=1;
    delay_us(2);
    BH1750_IIC_SCL=0;
}
//不产生ACK应答            
void BH1750_IIC_NAck(void)
{
    BH1750_IIC_SCL=0;
    SDA_OUT();
    BH1750_IIC_SDA=1;
    delay_us(2);
    BH1750_IIC_SCL=1;
    delay_us(2);
    BH1750_IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答 
void BH1750_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    SDA_OUT();         
    BH1750_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
        BH1750_IIC_SDA=1;
        else
            BH1750_IIC_SDA=0;
        txd<<=1;       
        delay_us(2);   //对TEA5767这三个延时都是必须的
        BH1750_IIC_SCL=1;
        delay_us(2); 
        BH1750_IIC_SCL=0;    
        delay_us(2);
    }     
}         
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t BH1750_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        BH1750_IIC_SCL=0; 
        delay_us(2);
        BH1750_IIC_SCL=1;
        receive<<=1;
        if(BH1750_READ_SDA)receive++;   
        delay_us(1); 
    }                     
    if (!ack)
        BH1750_IIC_NAck();//发送nACK
    else
        BH1750_IIC_Ack(); //发送ACK   
    return receive;
}
void bh_data_send(uint8_t command)
{
    uint8_t retry = 100;
    do{
        BH1750_IIC_Start();                     //iic起始信号
        BH1750_IIC_Send_Byte(BHAddWrite);       //发送器件地址
    }while(BH1750_IIC_Wait_Ack() && retry--);   //等待从机应答
    BH1750_IIC_Send_Byte(command);              //发送指令
    BH1750_IIC_Wait_Ack();                      //等待从机应答
    BH1750_IIC_Stop();                          //iic停止信号
}

/*************************************************************************************/

uint16_t BH1750_Get_Light(void)
{
    uint16_t buf;
    BH1750_IIC_Start();                         //iic起始信号
    BH1750_IIC_Send_Byte(BHAddRead);            //发送器件地址+读标志位
    BH1750_IIC_Wait_Ack();                      //等待从机应答
    buf=BH1750_IIC_Read_Byte(1);                //读取数据
    buf=buf<<8;                                 //读取并保存高八位数据
    buf+=0x00ff&BH1750_IIC_Read_Byte(0);        //读取并保存第八位数据
    BH1750_IIC_Stop();                          //发送停止信号 
    return buf; 
}


