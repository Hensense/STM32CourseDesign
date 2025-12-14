#ifndef _JQ8900_H
#define _JQ8900_H
#include "sys.h"

#define JQ8900_GPIO_CLK                RCC_APB2Periph_GPIOA
#define JQ8900_GPIO_PIN                GPIO_Pin_0
#define JQ8900_GPIO_PORT               GPIOA

#define sda(EN)         EN ?  GPIO_WriteBit(JQ8900_GPIO_PORT,JQ8900_GPIO_PIN,Bit_SET) \
                                    :GPIO_WriteBit(JQ8900_GPIO_PORT,JQ8900_GPIO_PIN,Bit_RESET)

void JQ8900_Init(void);
void JQ8900_Next(void);
void JQ8900_Previous(void);
void JQ8900_StopMusic(void);
void JQ8900_SetVolume(uint8_t vol);
void JQ8900_PlayMusic(uint16_t num);
#endif
