#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "sys.h"

#define NRF24L01_TX_PACKET_WIDTH        4                            //发送数据包宽度，范围：1~32字节
#define NRF24L01_RX_PACKET_WIDTH        4                            //接收通道0数据包宽度，范围：1~32字节

/********************* NRF24L01硬件配置 ********************/
/* CE引脚配置 */
#define NRF24L01_CE_GPIO_CLK      RCC_APB2Periph_GPIOA
#define NRF24L01_CE_GPIO_PORT     GPIOA
#define NRF24L01_CE_GPIO_PIN      GPIO_Pin_0

/* CSN引脚配置 */
#define NRF24L01_CSN_GPIO_CLK     RCC_APB2Periph_GPIOA
#define NRF24L01_CSN_GPIO_PORT    GPIOA
#define NRF24L01_CSN_GPIO_PIN     GPIO_Pin_1

/* SPI引脚配置 */
#define NRF24L01_SCK_GPIO_CLK     RCC_APB2Periph_GPIOA
#define NRF24L01_SCK_GPIO_PORT    GPIOA
#define NRF24L01_SCK_GPIO_PIN     GPIO_Pin_2

#define NRF24L01_MOSI_GPIO_CLK    RCC_APB2Periph_GPIOA
#define NRF24L01_MOSI_GPIO_PORT   GPIOA
#define NRF24L01_MOSI_GPIO_PIN    GPIO_Pin_3

#define NRF24L01_MISO_GPIO_CLK    RCC_APB2Periph_GPIOA
#define NRF24L01_MISO_GPIO_PORT   GPIOA  
#define NRF24L01_MISO_GPIO_PIN    GPIO_Pin_4
/*外部可调用全局数组***********/

extern uint8_t NRF24L01_TxAddress[];
extern uint8_t NRF24L01_TxPacket[];

extern uint8_t NRF24L01_RxAddress[];
extern uint8_t NRF24L01_RxPacket[];

/***********外部可调用全局数组*/


/*函数声明*********************/

/*指令实现*/
uint8_t NRF24L01_ReadReg(uint8_t RegAddress);
void NRF24L01_ReadRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteReg(uint8_t RegAddress, uint8_t Data);
void NRF24L01_WriteRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_ReadRxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteTxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_FlushTx(void);
void NRF24L01_FlushRx(void);
uint8_t NRF24L01_ReadStatus(void);

/*功能函数*/
void NRF24L01_PowerDown(void);
void NRF24L01_StandbyI(void);
void NRF24L01_Rx(void);
void NRF24L01_Tx(void);

void NRF24L01_Init(void);
uint8_t NRF24L01_Send(uint8_t *DataArray, uint8_t DataSize);
uint8_t NRF24L01_Receive(uint8_t *DataArray, uint8_t DataSize);
void NRF24L01_UpdateRxAddress(void);

/*********************函数声明*/


#endif
