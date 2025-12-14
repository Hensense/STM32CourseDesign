#include "MPU6050.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS        0xD0        //MPU6050的I2C从机地址
/*引脚配置层*/

/**
  * 函    数：I2C写SCL引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCL的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCL为低电平，当BitValue为1时，需要置SCL为高电平
  */
void MPU6050_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(MPU6050_IIC_SCL_PORT, MPU6050_IIC_SCL_PIN, (BitAction)BitValue);        //根据BitValue，设置SCL引脚的电平
    delay_us(10);                                                //延时10us，防止时序频率超过要求
}

/**
  * 函    数：I2C写SDA引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SDA的电平，范围0~0xFF
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SDA为低电平，当BitValue非0时，需要置SDA为高电平
  */
void MPU6050_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(MPU6050_IIC_SDA_PORT, MPU6050_IIC_SDA_PIN, (BitAction)BitValue);        //根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
    delay_us(10);                                                //延时10us，防止时序频率超过要求
}

/**
  * 函    数：I2C读SDA引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前SDA的电平，范围0~1
  * 注意事项：此函数需要用户实现内容，当前SDA为低电平时，返回0，当前SDA为高电平时，返回1
  */
uint8_t MPU6050_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(MPU6050_IIC_SDA_PORT, MPU6050_IIC_SDA_PIN);        //读取SDA电平
    delay_us(10);                                                //延时10us，防止时序频率超过要求
    return BitValue;                                            //返回SDA电平
}

/**
  * 函    数：I2C初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SCL和SDA引脚的初始化
  */
void MPU6050_I2C_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(MPU6050_IIC_SCL_CLK, ENABLE);   //开启SCL的时钟
    RCC_APB2PeriphClockCmd(MPU6050_IIC_SDA_CLK, ENABLE);   //开启SDA的时钟
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = MPU6050_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_IIC_SCL_PORT, &GPIO_InitStructure);   //将SCL引脚初始化为开漏输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = MPU6050_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_IIC_SDA_PORT, &GPIO_InitStructure);   //将SDA引脚初始化为开漏输出
    /*设置默认电平*/
    GPIO_SetBits(MPU6050_IIC_SCL_PORT, MPU6050_IIC_SCL_PIN);//设置SCL和SDA引脚初始化后默认为高电平（释放总线状态）
    GPIO_SetBits(MPU6050_IIC_SDA_PORT, MPU6050_IIC_SDA_PIN);
}

/*协议层*/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void MPU6050_Start(void)
{
    MPU6050_W_SDA(1);                            //释放SDA，确保SDA为高电平
    MPU6050_W_SCL(1);                            //释放SCL，确保SCL为高电平
    MPU6050_W_SDA(0);                            //在SCL高电平期间，拉低SDA，产生起始信号
    MPU6050_W_SCL(0);                            //起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void MPU6050_Stop(void)
{
    MPU6050_W_SDA(0);                            //拉低SDA，确保SDA为低电平
    MPU6050_W_SCL(1);                            //释放SCL，使SCL呈现高电平
    MPU6050_W_SDA(1);                            //在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MPU6050_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)                //循环8次，主机依次发送数据的每一位
    {
        MPU6050_W_SDA(Byte & (0x80 >> i));    //使用掩码的方式取出Byte的指定一位数据并写入到SDA线
        MPU6050_W_SCL(1);                        //释放SCL，从机在SCL高电平期间读取SDA
        MPU6050_W_SCL(0);                        //拉低SCL，主机开始发送下一位数据
    }
}

/**
  * 函    数：I2C接收一个字节
  * 参    数：无
  * 返 回 值：接收到的一个字节数据，范围：0x00~0xFF
  */
uint8_t MPU6050_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;                    //定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
    MPU6050_W_SDA(1);                            //接收前，主机先确保释放SDA，避免干扰从机的数据发送
    for (i = 0; i < 8; i ++)                //循环8次，主机依次接收数据的每一位
    {
        MPU6050_W_SCL(1);                        //释放SCL，主机机在SCL高电平期间读取SDA
        if (MPU6050_R_SDA() == 1){Byte |= (0x80 >> i);}    //读取SDA数据，并存储到Byte变量
                                                        //当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
        MPU6050_W_SCL(0);                        //拉低SCL，从机在SCL低电平期间写入SDA
    }
    return Byte;                            //返回接收到的一个字节数据
}

/**
  * 函    数：I2C发送应答位
  * 参    数：Byte 要发送的应答位，范围：0~1，0表示应答，1表示非应答
  * 返 回 值：无
  */
void MPU6050_SendAck(uint8_t AckBit)
{
    MPU6050_W_SDA(AckBit);                    //主机把应答位数据放到SDA线
    MPU6050_W_SCL(1);                            //释放SCL，从机在SCL高电平期间，读取应答位
    MPU6050_W_SCL(0);                            //拉低SCL，开始下一个时序模块
}

/**
  * 函    数：I2C接收应答位
  * 参    数：无
  * 返 回 值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
  */
uint8_t MPU6050_ReceiveAck(void)
{
    uint8_t AckBit;                            //定义应答位变量
    MPU6050_W_SDA(1);                            //接收前，主机先确保释放SDA，避免干扰从机的数据发送
    MPU6050_W_SCL(1);                            //释放SCL，主机机在SCL高电平期间读取SDA
    AckBit = MPU6050_R_SDA();                    //将应答位存储到变量里
    MPU6050_W_SCL(0);                            //拉低SCL，开始下一个时序模块
    return AckBit;                            //返回定义应答位变量
}
/******************************************************************************************/
/**
  * 函    数：MPU6050写寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MPU6050_Start();                        //I2C起始
    MPU6050_SendByte(MPU6050_ADDRESS);    //发送从机地址，读写位为0，表示即将写入
    MPU6050_ReceiveAck();                    //接收应答
    MPU6050_SendByte(RegAddress);            //发送寄存器地址
    MPU6050_ReceiveAck();                    //接收应答
    MPU6050_SendByte(Data);                //发送要写入寄存器的数据
    MPU6050_ReceiveAck();                    //接收应答
    MPU6050_Stop();                        //I2C终止
}

/**
  * 函    数：MPU6050读寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;
    
    MPU6050_Start();                        //I2C起始
    MPU6050_SendByte(MPU6050_ADDRESS);    //发送从机地址，读写位为0，表示即将写入
    MPU6050_ReceiveAck();                    //接收应答
    MPU6050_SendByte(RegAddress);            //发送寄存器地址
    MPU6050_ReceiveAck();                    //接收应答
    
    MPU6050_Start();                        //I2C重复起始
    MPU6050_SendByte(MPU6050_ADDRESS | 0x01);    //发送从机地址，读写位为1，表示即将读取
    MPU6050_ReceiveAck();                    //接收应答
    Data = MPU6050_ReceiveByte();            //接收指定寄存器的数据
    MPU6050_SendAck(1);                    //发送应答，给从机非应答，终止从机的数据输出
    MPU6050_Stop();                        //I2C终止
    
    return Data;
}

/**
  * 函    数：MPU6050初始化
  * 参    数：无
  * 返 回 值：无
  */
void MPU6050_Init(void)
{
    MPU6050_I2C_Init();                                    //先初始化底层的I2C
    
    /*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);        //电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);        //电源管理寄存器2，保持默认值0，所有轴均不待机
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);        //采样率分频寄存器，配置采样率
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);            //配置寄存器，配置DLPF
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);    //陀螺仪配置寄存器，选择满量程为±2000°/s
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);    //加速度计配置寄存器，选择满量程为±16g
}

/**
  * 函    数：MPU6050获取ID号
  * 参    数：无
  * 返 回 值：MPU6050的ID号
  */
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);        //返回WHO_AM_I寄存器的值
}

/**
  * 函    数：MPU6050获取数据
  * 参    数：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 参    数：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_GetValue(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;                                //定义数据高8位和低8位的变量
    
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);       //读取加速度计X轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);       //读取加速度计X轴的低8位数据
    *AccX = (DataH << 8) | DataL;                        //数据拼接，通过输出参数返回
    
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);       //读取加速度计Y轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);       //读取加速度计Y轴的低8位数据
    *AccY = (DataH << 8) | DataL;                        //数据拼接，通过输出参数返回
    
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);       //读取加速度计Z轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);       //读取加速度计Z轴的低8位数据
    *AccZ = (DataH << 8) | DataL;                        //数据拼接，通过输出参数返回
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);        //读取陀螺仪X轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);        //读取陀螺仪X轴的低8位数据
    *GyroX = (DataH << 8) | DataL;                       //数据拼接，通过输出参数返回
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);        //读取陀螺仪Y轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);        //读取陀螺仪Y轴的低8位数据
    *GyroY = (DataH << 8) | DataL;                       //数据拼接，通过输出参数返回
    
    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);        //读取陀螺仪Z轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);        //读取陀螺仪Z轴的低8位数据
    *GyroZ = (DataH << 8) | DataL;                       //数据拼接，通过输出参数返回
}
