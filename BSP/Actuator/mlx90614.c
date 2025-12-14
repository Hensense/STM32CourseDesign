#include "mlx90614.h"
/* 红外测温传感器 */
uint8_t value_H,value_L,PEC;
    
void MLX90614_Init()
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(MLX90614_IIC_SCL_CLK, ENABLE);   //开启SCL的时钟
    RCC_APB2PeriphClockCmd(MLX90614_IIC_SDA_CLK, ENABLE);   //开启SDA的时钟
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = MLX90614_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MLX90614_IIC_SCL_PORT, &GPIO_InitStructure);   //将SCL引脚初始化为开漏输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = MLX90614_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MLX90614_IIC_SDA_PORT, &GPIO_InitStructure);   //将SDA引脚初始化为开漏输出
    /*设置默认电平*/
    GPIO_SetBits(MLX90614_IIC_SCL_PORT, MLX90614_IIC_SCL_PIN);//设置SCL和SDA引脚初始化后默认为高电平（释放总线状态）
    GPIO_SetBits(MLX90614_IIC_SDA_PORT, MLX90614_IIC_SDA_PIN);
}


/*******发起始位*********/
void MLX90614_SDA_Start()
{    
    MLX90614_SDA_OUT = 1;
    delay_us(1);
    MLX90614_SCL = 1;
    delay_us(5);
    MLX90614_SDA_OUT = 0;
    delay_us(10);
    MLX90614_SCL = 0;    
    delay_us(4);
}

/*********发结束位********/
void MLX90614_SDA_Stop()
{
    MLX90614_SCL = 0;
    delay_us(5);
    MLX90614_SDA_OUT = 0;
    delay_us(5);
    MLX90614_SCL = 1;
    delay_us(10);
    MLX90614_SDA_OUT = 1;
    
}


/*发送一个位*/
void MLX90614_SDA_send_bit(uint8_t bit_out)
{    
    if(bit_out == 1)
        MLX90614_SDA_OUT = 1;
    else
        MLX90614_SDA_OUT = 0;
    delay_us(5);
    MLX90614_SCL = 1;
    delay_us(10);
    MLX90614_SCL = 0;    
    delay_us(4);
}

/**接收一个位**/ 
uint8_t MLX90614_SDA_receive_bit(void)
{    
    uint8_t bit_in = 0;

    MLX90614_SDA_OUT = 1;
    MLX90614_SCL = 1;
    delay_us(10);    
    
    if(MLX90614_SDA_IN)
        bit_in = 1;
    else
        bit_in = 0;
    
    MLX90614_SCL = 0;
    delay_us(4);
    
    return bit_in;
}


/***  功 能 ：发送一个字节   ***
 ***  参 数 ：发送的一个字节 ***
 ***  返回值：应答位         ***/
uint8_t MLX90614_SDA_send(uint8_t dat)
{
    uint8_t i = 0,bit_out = 0;
    
    for(i=0;i<8;i++)
    {      
        if(dat & 0x80)   //是从最高位开始传
            bit_out=1;    
        else
            bit_out=0;
        MLX90614_SDA_send_bit(bit_out); 
        dat <<= 1;        
    }
    if(MLX90614_SDA_receive_bit())
        return 1;
    else
        return 0;
}

/**接收一个字节**/
uint8_t MLX90614_SDA_receive()
{
    uint8_t i=0,MLX90614_dat=0;
    
    for(i=0;i<8;i++)
    {
        MLX90614_dat <<= 1;
        if(MLX90614_SDA_receive_bit())
            MLX90614_dat |= 0x01;
    }
    MLX90614_SDA_send_bit(0);
    return MLX90614_dat;
}


    
/*读取温度值*/
/*
1、起始
2、发送要Slave address Tobj1 007h （物体温度）
3、读 读应答
4、发送要执行的命令（选择读RAM）
5、发送起始位
6、读寄存器
7、读PEC数据
*/

float MLX90614_ReadValue()
{
    u16 MLX90614_value = 0;
  float MLX90614_t = 0;
    
    MLX90614_SDA_Start();    
  //Slave address 单个MLX90614时地址为0x00  地址+wr    
    if(MLX90614_SDA_send(0x00) == 1)    //有应答  
        return 0;
    //发送命令
    if(MLX90614_SDA_send(0x07) == 1)
        return 0;
    //-----------
        
    MLX90614_SDA_Start();    //Sr
        
    if(MLX90614_SDA_send(0x01) == 1)
        return 0;
        
    //读Tobj1低八位并发送应答
    value_L = MLX90614_SDA_receive();
            
    //读Tobj1高八位并发送应答  
    value_H = MLX90614_SDA_receive();

        
    //读PEC出错数据包并发送应答 
    PEC = MLX90614_SDA_receive();
        
    MLX90614_SDA_Stop();    
    
    /*数据的处理*/    
    MLX90614_value = value_H;
    MLX90614_value <<= 8;
    MLX90614_value |= value_L;
    MLX90614_t=MLX90614_value*0.02-272.3;
    
    return MLX90614_t;    
}

/**
  * @brief  获取MLX90614红外温度传感器读数
  * @param  detection_mode : 测温模式选择(物体温度, 人体温度+3.5℃补偿) HUMAN_MODE | OBJECT_MODE
  * @retval 当前测量温度值(单位:℃)
  */
float MLX90614_GetTemperature(TempDetect_Mode detection_mode)
{
    float temp = 0;
    
    // 连续采样100次取平均值
    for(int i=0; i<100; i++) {
        temp += MLX90614_ReadValue();
    }
    temp /= 100.0f;
    
    // 人体温度补偿
    if(detection_mode == HUMAN_MODE) {
        temp += 3.5f;
    }
    return temp;
}

