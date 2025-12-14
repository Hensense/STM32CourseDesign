#ifndef __MLX90614_H
#define __MLX90614_H

#include "sys.h"

typedef enum
{
    HUMAN_MODE = 0,     /* 人体温度检测模式 */
    OBJECT_MODE         /* 物体温度检测模式 */
}TempDetect_Mode;       /* 检测模式 */

/************************************根据需求修改********************************************/
#define         MLX90614_IIC_SCL_CLK            RCC_APB2Periph_GPIOB
#define         MLX90614_IIC_SCL_PORT           GPIOB
#define         MLX90614_IIC_SCL_PIN            GPIO_Pin_10

#define         MLX90614_IIC_SDA_CLK            RCC_APB2Periph_GPIOB
#define         MLX90614_IIC_SDA_PORT           GPIOB
#define         MLX90614_IIC_SDA_PIN            GPIO_Pin_11

#define         MLX90614_SDA_IN                 PBin(11)
#define         MLX90614_SDA_OUT                PBout(11)

#define         MLX90614_SCL                    PBout(10)
/************************************* end *************************************************/
void  MLX90614_Init(void);
float MLX90614_GetTemperature(TempDetect_Mode detection_mode);
#endif
