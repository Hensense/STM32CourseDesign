#include "sht30.h"

#define POLYNOMIAL_CXDZ 0x31 // X^8 + X^5 + X^4 + 1

void SHT30_Init(void)    
{

    GPIO_InitTypeDef  GPIO_InitStructure;  

    RCC_APB2PeriphClockCmd(SHT30_IIC_CLK,ENABLE);
 

    GPIO_InitStructure.GPIO_Pin =  SHT30_IIC_SDA_PIN | SHT30_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
    GPIO_Init(SHT30_IIC_PORT, &GPIO_InitStructure);

    SHT30_IIC_SDA_H;//释放IIC总线的数据线。
    SHT30_IIC_SCL_H;//释放IIC总线的时钟线。
    delay_us(6);
    
}


//SHT30引脚输出模式控制
void SHT30_IIC_SDA_OUT(void)//SDA输出方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=SHT30_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA推挽输出
    GPIO_Init(SHT30_IIC_PORT,&GPIO_InitStructure);                         
}

void SHT30_IIC_SDA_IN(void)//SDA输入方向配置
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    GPIO_InitStructure.GPIO_Pin=SHT30_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL上拉输入
    GPIO_Init(SHT30_IIC_PORT,&GPIO_InitStructure);
    
}



/****************************************************************
通讯起始信号：当 SCL 线是高电平时 SDA 线从高电平向低电平切换，表示通讯的开始；
*****************************************************************/

void SHT30_IIC_start(void)  
{

        SHT30_IIC_SDA_H;//拉高
        SHT30_IIC_SCL_H;
        delay_us(6);
        SHT30_IIC_SDA_L;
        SHT30_IIC_SCL_L;        //拉低时钟线，准备开始时钟
        delay_us(6);
}
/****************************************************************
通讯停止信号：当 SCL 是高电平时 SDA线由低电平向高电平切换，表示通讯的停止。
*****************************************************************/

void SHT30_IIC_stop(void) 
{

        SHT30_IIC_SCL_H;
        SHT30_IIC_SDA_L;
        delay_us(6);
        SHT30_IIC_SDA_H;    //通讯停止
        SHT30_IIC_SCL_L;
        delay_us(6);
}
/****************************************************************
//接收应答信号函数
*****************************************************************/

uint8_t SHT30_IIC_Get_ack(void)
{
        uint16_t CNT;
        SHT30_IIC_SDA_IN();
        SHT30_IIC_SCL_L;        //拉低时钟线。
        SHT30_IIC_SCL_H;        //拉高时钟线。
        delay_us(6);
        while((SHT30_READ_SDA)  && (CNT < 100) )
        {
                CNT++;
                if(CNT == 100)
                {
                        return 0;
                }
        }

        SHT30_IIC_SCL_L;        //拉低时钟线。
        delay_us(6);
        SHT30_IIC_SDA_OUT();
        return 1;

}
/****************************************************************
//发送应答信号函数
*****************************************************************/

void SHT30_IIC_ACK(void)    
{ 

        SHT30_IIC_SDA_L;    //拉低数据线，应答
        SHT30_IIC_SCL_H;    //产生第九个时钟信号。
        delay_us(6);
        SHT30_IIC_SCL_L;    
        delay_us(6);
}
//非应答

void SHT30_IIC_NACK(void)
{ 

        SHT30_IIC_SDA_H;    //拉高数据线，非应答
        SHT30_IIC_SCL_H;    //产生第九个时钟信号。
        delay_us(6);
        SHT30_IIC_SCL_L;    

}

/****************************************************************
//向IIC总线写入一个字节的数据
*****************************************************************/

void SHT30_IIC_write_byte(uint8_t Data)
{

        uint8_t i;
        for(i=0;i<8;i++)//八位数据
        {
                if((Data & 0x80) == 0x80)
                {
                        SHT30_IIC_SDA_H;
                        
                }
                else
                {
                        SHT30_IIC_SDA_L;
                        
                }
                
                SHT30_IIC_SCL_H; //一个时钟信号送入数据
                delay_us(6);
                SHT30_IIC_SCL_L;
                delay_us(6);
                Data = Data << 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
                
        }
        SHT30_IIC_SDA_L;        //应答处理前拉低，跳过应答

}
/****************************************************************
//从IIC总线读取一个字节的数据函数
*****************************************************************/

uint8_t SHT30_IIC_read_byte(uint8_t ack)
{

        uint8_t i;
        uint8_t Data = 0;       //定义一个缓冲寄存器。
        SHT30_IIC_SDA_IN();
        SHT30_IIC_SCL_L;    //先拉低时钟线
        SHT30_IIC_SDA_H;    //再拉高数据线
        delay_us(6);
        for(i = 0;i < 8;i++)
        {
                Data = Data<<1;        //将缓冲字节的数据左移一位，准备读取数据。
                
                SHT30_IIC_SCL_H;                //拉高时钟线，开始读取下一位数据
                delay_us(6);
                if(SHT30_READ_SDA)        //如果数据线为高平电平。
                {
                        Data = Data|0x01;    //则给缓冲字节的最低位写1。
                }
                SHT30_IIC_SCL_L;                //拉低时钟线，一位读取完成
                delay_us(6);
        }
        SHT30_IIC_SDA_OUT();
        if (!ack)
                SHT30_IIC_NACK();//发送nACK
    else
        SHT30_IIC_ACK(); //发送ACK   
        return Data;                    //返回读取的一个字节数据。



}



//SHT3X CRC校验
unsigned char SHT3X_CRC(uint8_t *data, uint8_t len)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial @GZCXDZ
    for(byteCtr = 0; byteCtr < len; byteCtr++) {
            crc ^= (data[byteCtr]);
            for(bit = 8; bit > 0; --bit) {
                    if(crc & 0x80) {
                            crc = (crc << 1) ^ POLYNOMIAL_CXDZ;
                    }  else {
                            crc = (crc << 1);
                    }
            }
    }
  return crc;
}

//SHT30命令函数
//addr:表示产品的序号，因为SHT30使用IIC总线的话一条线上可以挂两个

void SHT30_CMD(uint16_t cmd)
{
    SHT30_IIC_start();
  SHT30_IIC_write_byte(SHT30_ADDR+0);  //发送设备地址,写寄存器
    SHT30_IIC_Get_ack();
    SHT30_IIC_write_byte((cmd>>8)&0xff); //MSB
    SHT30_IIC_Get_ack();
    SHT30_IIC_write_byte(cmd&0xff); //LSB
    SHT30_IIC_Get_ack();
    SHT30_IIC_stop();
    delay_ms(50);//命令发完后需要等待20ms以上才能读写
}



//SHT30读取温湿度
//temp:温度，-400~1250，实际温度=temp/10,分辨率0.1℃,精度±0.3℃
//humi:湿度，0~1000，实际湿度=humi/10,分辨率0.1%rh,精度±3
//返回0成功，1失败

u8 SHT30_Read_Humiture(int *temp,uint16_t *humi)
{
    uint8_t buff[6];
    
    SHT30_CMD(SHT30_READ_HUMITURE);//读温湿度命令
    
    SHT30_IIC_start();
    SHT30_IIC_write_byte(SHT30_ADDR+1); //发送设备地址,读寄存器
    SHT30_IIC_Get_ack();
    buff[0]=SHT30_IIC_read_byte(1);//继续读，给应答
    buff[1]=SHT30_IIC_read_byte(1);//继续读，给应答
    buff[2]=SHT30_IIC_read_byte(1);//继续读，给应答
    buff[3]=SHT30_IIC_read_byte(1);//继续读，给应答
    buff[4]=SHT30_IIC_read_byte(1);//继续读，给应答
    buff[5]=SHT30_IIC_read_byte(0);//不继续给停止应答
    SHT30_IIC_stop();

    
    //printf("buff=%d,%d,%d,%d,%d,%d\r\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
    //CRC校验
    if(SHT3X_CRC(&buff[0],2)==buff[2] && SHT3X_CRC(&buff[3],2)==buff[5])
    {

        *temp=(-45+(175.0*((buff[0]<<8)+buff[1])/65535.0));
        *humi=100*(((buff[3]<<8)+buff[4])/65535.0);
        if(*temp>1250) *temp=1250;
        else if(*temp<-400) *temp=-400;
        return 0;
    }
    else return 1;    
    
}






