#include "hx711.h"

int32_t reset;
float Weights=100.0;            //100g
int32_t Weights_100=8493860;    //100g

/**
  * @brief  PM25初始化程序
  * @param  无
  * @retval 无
  */

void HX711_Init(void)
{
    /*定义一个HX711_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启相关的GPIO外设时钟*/
    RCC_APB2PeriphClockCmd(HX711_SCK_GPIO_CLK | HX711_DT_GPIO_CLK , ENABLE);
    /*选择要控制的GPIO引脚*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Pin = HX711_SCK_GPIO_PIN;
    GPIO_Init(HX711_SCK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HX711_DT_GPIO_PIN;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
    GPIO_Init(HX711_DT_GPIO_PORT, &GPIO_InitStructure);    

    reset = HX711_GetCount();   /* 记录初始重量 */
}

uint32_t HX711_GetCount(void)
{
    uint32_t Count;
    uint8_t i;
    uint8_t retry = 0xFF;
    HX711_SCK_L;
    delay_us(1);
    Count=0;
    while(HX711_DT && retry--);
    if(retry == 0)
    {
        return 0x00;
    }
    for (i=0;i<24;i++)
    {
        HX711_SCK_H;
        delay_us(1);
        Count=Count<<1;
        HX711_SCK_L;
        delay_us(1);
        if(HX711_DT) Count++;
    }
    HX711_SCK_H;
    delay_us(1);
    Count=Count^0x800000;//最高位取反，其他位不变
                      //在HX71芯片中，count是一个32位的有符号整数，用于存储称重传感器的读数。
                      //当count的最高位为1时，表示读数为负数，而HX711芯片不支持负数的读数。
                      //因此，为了将负数转换为正数，需要将count的最高位取反，即将count与0x800000进行异或操作。
                      //具体来说，0x800000的二进制表示为100000000000000000000000，与count进行异或操作后，
                      //可以将count的最高位从1变为0，从而得到对应的正数读数。
    HX711_SCK_L;
    delay_us(1);

    return(Count);
}

float HX711_GetWeight(void)
{
    int value;
    float weight;
    value = HX711_GetCount();
    weight=(float)(value-reset)*Weights/(float)(Weights_100-reset);
    return weight;
}
