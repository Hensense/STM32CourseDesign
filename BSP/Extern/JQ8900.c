#include "JQ8900.h" 

void JQ8900_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(JQ8900_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=JQ8900_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(JQ8900_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(JQ8900_GPIO_PORT,JQ8900_GPIO_PIN);
}
//用于控制jq8900指令
void SendData(uint8_t addr)
{
    uint8_t i;
    sda(1);//拉高
    delay_us( 700);
    sda(0);//拉低
    
    delay_us (3200);
     for ( i = 0; i < 8; i++ ) 
    {
        sda(1);//拉高
        if ( addr & 0x01 )//高电平
        {
            delay_us( 600 );
            sda(0);//拉低
            delay_us( 210 );
        }
        else //低电平
        {
            delay_us( 210 );
            sda(0);//拉低
            delay_us( 600 );
        }
      addr >>= 1;//右移一位
    }
    sda(1);//拉高
}
//播放下一首
void JQ8900_Next(void)
{
    SendData(0x15);    
}
//播放上一首
void JQ8900_Previous(void)
{
    SendData(0x14);    
}
//停止播放
void JQ8900_StopMusic(void)
{
    SendData(0x13);    
}
//设置音量 1~9
void JQ8900_SetVolume(uint8_t vol)
{
    SendData(0x0a);//清零数字
    SendData(vol);
    SendData(0x00);
    SendData(0x0c);//设置音量
}
void JQ8900_PlayMusic(uint16_t num)
{
    if(num < 10)
    {
        SendData(0x11);
        SendData(0x0a);
        SendData(0x01*num); 
        SendData(0x0b);
    }
    else if(num >= 10)
    {
        SendData(0x11);
        SendData(0x0a);
        SendData(0x10 *(num / 10));
        SendData(0x01 *(num % 10)); 
        SendData(0x0b);
    }
}
