#include "SGP30.h"

// 私有函数声明
static void IIC_Init(void);
static void IIC_Start(void);
static void IIC_Stop(void);
static uint8_t IIC_Wait_Ack(void);
static void IIC_Ack(void);
static void IIC_NAck(void);
static void IIC_Send_Byte(uint8_t txd);
static uint8_t IIC_Read_Byte(uint8_t ack);
static void SDA_OUT(void);
static void SDA_IN(void);

// 初始化IIC接口
static void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 初始化SCL
    RCC_APB2PeriphClockCmd(SGP30_IIC_SDA_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SCL_PIN | SGP30_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SGP30_IIC_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
}

// SDA设置为输出
static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SGP30_IIC_SDA_PORT, &GPIO_InitStructure);
}

// SDA设置为输入
static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SGP30_IIC_SDA_PORT, &GPIO_InitStructure);
}

// 产生IIC起始信号
static void IIC_Start(void)
{
    SDA_OUT();
    GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    delay_us(IIC_DELAY_US);
    
    GPIO_ResetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
}

// 产生IIC停止信号
static void IIC_Stop(void)
{
    SDA_OUT();
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    GPIO_ResetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
    
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
}

// 等待应答信号
static uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();
    GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    delay_us(IIC_DELAY_US);
    
    while(GPIO_ReadInputDataBit(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN))
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    return 0;
}

// 产生ACK应答
static void IIC_Ack(void)
{
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    SDA_OUT();
    GPIO_ResetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
}

// 不产生ACK应答
static void IIC_NAck(void)
{
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    SDA_OUT();
    GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    delay_us(IIC_DELAY_US);
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
}

// IIC发送一个字节
static void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
    
    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7)
            GPIO_SetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
        else
            GPIO_ResetBits(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN);
            
        txd <<= 1;
        delay_us(IIC_DELAY_US);
        GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
        delay_us(IIC_DELAY_US);
        GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
        delay_us(IIC_DELAY_US);
    }
}

// IIC读取一个字节
static uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    SDA_IN();
    
    for(i = 0; i < 8; i++)
    {
        GPIO_ResetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
        delay_us(IIC_DELAY_US);
        GPIO_SetBits(SGP30_IIC_SCL_PORT, SGP30_IIC_SCL_PIN);
        receive <<= 1;
        
        if(GPIO_ReadInputDataBit(SGP30_IIC_SDA_PORT, SGP30_IIC_SDA_PIN))
            receive++;
            
        delay_us(IIC_DELAY_US);
    }
    
    if(!ack)
        IIC_NAck();
    else
        IIC_Ack();
        
    return receive;
}

// SGP30写命令
void SGP30_WriteCmd(uint8_t cmd_h, uint8_t cmd_l)
{
    IIC_Start();
    IIC_Send_Byte(SGP30_WRITE_ADDR);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd_h);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd_l);
    IIC_Wait_Ack();
    IIC_Stop();
    delay_ms(100);
}

// SGP30初始化
void SGP30_Init(void)
{
    IIC_Init();
    SGP30_WriteCmd(0x20, 0x03);
    delay_ms(100);
}

// SGP30读取数据
uint32_t SGP30_ReadData(void)
{
    uint32_t dat = 0;
    uint8_t crc;
    
    IIC_Start();
    IIC_Send_Byte(SGP30_READ_ADDR);
    IIC_Wait_Ack();
    
    dat = IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    crc = IIC_Read_Byte(1); // CRC数据，舍去
    (void)crc; // 避免编译警告
    
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(0);
    
    IIC_Stop();
    return dat;
}
/**
 * @brief   获取SGP30传感器的CO2和TVOC测量值
 * @param   co2: 指向存储CO2浓度值的指针，单位ppm
 * @param   tvoc: 指向存储TVOC浓度值的指针，单位ppb
 * @retval  无
 * 
 * @note    1. 该函数会发送测量命令(0x2008)并读取传感器数据
 *          2. CO2测量范围: 400-60000 ppm
 *          3. TVOC测量范围: 0-60000 ppb
 */
void SGP30_GetValue(uint16_t* co2, uint16_t* tvoc)
{
    uint32_t data;
    /* 发送测量命令 0x2008 (Measure Air Quality) */
    SGP30_WriteCmd(0x20, 0x08);
    
    /* 读取传感器返回的32位数据(包含CO2和TVOC值) */
    data = SGP30_ReadData();
    
    /* 提取CO2值(高16位)并存入指针指向的地址 */
    *co2 = (data & 0xffff0000) >> 16;
    
    /* 提取TVOC值(低16位)并存入指针指向的地址 */
    *tvoc = data & 0x0000ffff;
}
