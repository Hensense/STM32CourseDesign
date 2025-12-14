#include "adxl345.h"
#include "math.h"

//ADXL345引脚输出模式控制
void ADXL345_IIC_SDA_OUT(void)//SDA输出方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=ADXL345_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA推挽输出
    GPIO_Init(ADXL345_SDA_PORT,&GPIO_InitStructure);                         

}

void ADXL345_IIC_SDA_IN(void)//SDA输入方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=ADXL345_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL上拉输入
    GPIO_Init(ADXL345_SDA_PORT,&GPIO_InitStructure);
    
}
//以下为模拟IIC总线函数
void ADXL345_IIC_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(ADXL345_SCL_GPIO_CLK, ENABLE);         //使能SCL端口时钟
    GPIO_InitStructure.GPIO_Pin = ADXL345_SCL_PIN;                    //配置为推挽输出,SCL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 //IO口速度为50MHz
    GPIO_Init(ADXL345_SCL_PORT, &GPIO_InitStructure);                
    GPIO_SetBits(ADXL345_SCL_PORT,ADXL345_SCL_PIN); 
    
    RCC_APB2PeriphClockCmd(ADXL345_SDA_GPIO_CLK, ENABLE);         //使能SDA端口时钟
    GPIO_InitStructure.GPIO_Pin = ADXL345_SDA_PIN;                    //配置为推挽输出,SDA
    GPIO_Init(ADXL345_SDA_PORT, &GPIO_InitStructure);
    GPIO_SetBits(ADXL345_SDA_PORT,ADXL345_SDA_PIN); 
}
void ADXL345_IIC_Start()
{
    ADXL345_IIC_SDA_OUT();
    ADXL345_IIC_SDA=1;            
    ADXL345_IIC_SCL=1;
    delay_us(5);
    ADXL345_IIC_SDA=0;
    delay_us(5);
    ADXL345_IIC_SCL=0;
}
void ADXL345_IIC_Stop()
{
    ADXL345_IIC_SDA_OUT();
    ADXL345_IIC_SCL=0;
    ADXL345_IIC_SDA=0;
    delay_us(5);
    ADXL345_IIC_SCL=1; 
    ADXL345_IIC_SDA=1;
    delay_us(5);
}
//主机产生一个应答信号
void ADXL345_IIC_Ack()
{
    ADXL345_IIC_SCL=0;
    ADXL345_IIC_SDA_OUT();
    ADXL345_IIC_SDA=0;
    
    delay_us(2);
    ADXL345_IIC_SCL=1;
    delay_us(5);
    ADXL345_IIC_SCL=0;    
}
//主机不产生应答信号
void ADXL345_IIC_NAck()
{
    ADXL345_IIC_SCL=0;
    ADXL345_IIC_SDA_OUT();
    ADXL345_IIC_SDA=1;
    delay_us(2);
    ADXL345_IIC_SCL=1;
    delay_us(2);
    ADXL345_IIC_SCL=0;
}
//等待从机应答信号
//返回值：1 接收应答失败
//          0 接收应答成功
uint8_t ADXL345_IIC_Wait_Ack()
{
    uint8_t tempTime=0;
    ADXL345_IIC_SDA_IN();
    ADXL345_IIC_SDA=1;
    delay_us(1);
    ADXL345_IIC_SCL=1;
    delay_us(1);

    while(ADXL345_READ_SDA)
    {
        tempTime++;
        if(tempTime>250)
        {
            ADXL345_IIC_Stop();
            return 1;
        }     
    }

    ADXL345_IIC_SCL=0;
    return 0;
}
void ADXL345_IIC_Send_Byte(uint8_t txd)
{
    uint8_t i=0;
    ADXL345_IIC_SDA_OUT();
    ADXL345_IIC_SCL=0;;//拉低时钟开始数据传输
    for(i=0;i<8;i++)
    {
        ADXL345_IIC_SDA=(txd&0x80)>>7;//读取字节
        txd<<=1;
        ADXL345_IIC_SCL=1;
        delay_us(2); //发送数据
        ADXL345_IIC_SCL=0;
        delay_us(2);
    }
}
//读取一个字节
uint8_t ADXL345_IIC_Read_Byte(uint8_t ack)
{
    uint8_t i=0,receive=0;
    ADXL345_IIC_SDA_IN();
   for(i=0;i<8;i++)
   {
            ADXL345_IIC_SCL=0;
            delay_us(2);
            ADXL345_IIC_SCL=1;
            receive<<=1;//左移
            if(ADXL345_READ_SDA)
            receive++;//连续读取八位
            delay_us(1);    
   }

       if(!ack)
           ADXL345_IIC_NAck();
    else
        ADXL345_IIC_Ack();

    return receive;//返回读取到的字节
}



//传感器初始化
void ADXL345_Init()
{
        ADXL345_IIC_Init();
    
        adxl345_write_reg(0X31,0X0B);        //低电平中断输出,13位全分辨率,输出数据右对齐,16g量程 
        adxl345_write_reg(0x2C,0x0B);        //数据输出速度为100Hz
        adxl345_write_reg(0x2D,0x08);        //链接使能,测量模式,省电特性
        adxl345_write_reg(0X2E,0x80);        //不使用中断         
         adxl345_write_reg(0X1E,0x00);
        adxl345_write_reg(0X1F,0x00);
        adxl345_write_reg(0X20,0x05);    
    
    
}
//写寄存器函数
void adxl345_write_reg(uint8_t addr,uint8_t val) 
{
    ADXL345_IIC_Start();                   
    ADXL345_IIC_Send_Byte(slaveaddress);         //发送写器件指令     
    ADXL345_IIC_Wait_Ack();       
  ADXL345_IIC_Send_Byte(addr);               //发送寄存器地址
    ADXL345_IIC_Wait_Ack();                                                           
    ADXL345_IIC_Send_Byte(val);             //发送值                       
    ADXL345_IIC_Wait_Ack();                     
    ADXL345_IIC_Stop();                        //产生一个停止条件        
}
//读寄存器函数
uint8_t adxl345_read_reg(uint8_t addr)
{
    uint8_t temp=0;         
    ADXL345_IIC_Start();                   
    ADXL345_IIC_Send_Byte(slaveaddress);    //发送写器件指令     
    temp=ADXL345_IIC_Wait_Ack();       
    ADXL345_IIC_Send_Byte(addr);           //发送寄存器地址
    temp=ADXL345_IIC_Wait_Ack();                                                           
    ADXL345_IIC_Start();                      //重新启动
    ADXL345_IIC_Send_Byte(regaddress);    //发送读器件指令     
    temp=ADXL345_IIC_Wait_Ack();       
    temp=ADXL345_IIC_Read_Byte(0);        //读取一个字节,不继续再读,发送NAK                
    ADXL345_IIC_Stop();                    //产生一个停止条件         
    return temp;
}
//读取数据函数
void adxl345_read_data(short *x,short *y,short *z)
{
    uint8_t buf[6];
    uint8_t i;
    ADXL345_IIC_Start();                   
    ADXL345_IIC_Send_Byte(slaveaddress);    //发送写器件指令     
    ADXL345_IIC_Wait_Ack();       
    ADXL345_IIC_Send_Byte(0x32);           //发送寄存器地址(数据缓存的起始地址为0X32)
    ADXL345_IIC_Wait_Ack();                                                           
 
     ADXL345_IIC_Start();                      //重新启动
    ADXL345_IIC_Send_Byte(regaddress);    //发送读器件指令
    ADXL345_IIC_Wait_Ack();
    for(i=0;i<6;i++)
    {
        if(i==5)buf[i]=ADXL345_IIC_Read_Byte(0);//读取一个字节,不继续再读,发送NACK  
        else buf[i]=ADXL345_IIC_Read_Byte(1);    //读取一个字节,继续读,发送ACK 
     }                   
    ADXL345_IIC_Stop();                    //产生一个停止条件
    *x=(short)(((u16)buf[1]<<8)+buf[0]);     //合成数据    
    *y=(short)(((u16)buf[3]<<8)+buf[2]);         
    *z=(short)(((u16)buf[5]<<8)+buf[4]); 
}
//连读读取几次取平均值函数
//times 取平均值的次数
void adxl345_read_average(float *x,float *y,float *z,uint8_t times)
{
    uint8_t i;
    short tx,ty,tz;
    *x=0;
    *y=0;
    *z=0;
    if(times)//读取次数不为0
    {
        for(i=0;i<times;i++)//连续读取times次
        {
            adxl345_read_data(&tx,&ty,&tz);
            *x+=tx;
            *y+=ty;
            *z+=tz;
            delay_ms(5);
        }
        *x/=times;
        *y/=times;
        *z/=times;
    }
}
void ADXL345_GetAngle(float *x_angle,float *y_angle,float *z_angle)
{
    float ax,ay,az;
    adxl345_read_average(&ax,&ay,&az,10);
    *x_angle=atan(ax/sqrt((az*az+ay*ay)))*180/3.14;
    *y_angle=atan(ay/sqrt((ax*ax+az*az)))*180/3.14;
    *z_angle=atan(sqrt((ax*ax+ay*ay)/az))*180/3.14;
//return x_angle;
}
