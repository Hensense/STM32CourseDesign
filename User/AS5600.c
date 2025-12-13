#include "AS5600.h"

void I2C_ScanDevices(I2C_HandleTypeDef *hi2c) {
    uint8_t found = 0;
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {  // I2C 地址范围：0x08 ~ 0x77
        HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, addr << 1, 3, I2C_TIMEOUT);

        if (status == HAL_OK) {
			printf("Device found at 0x%02X (7-bit: 0x%02X)\n", addr << 1, addr);
            found++;
        }
    }

    if (found == 0) {
		printf("No Found!\n");
    } else {
		printf("Found %d device(s).\n", found);
    }
}

/**
 * @brief AS5600芯片获取角度（归一化到 0~2π）
 * @param void
 * @return 输出当前角度 angle
 */
uint16_t getAngle(void)
{
	uint16_t angle = 0;
	uint8_t data[2] = {0};
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, AS5600_Add, Raw_Add, I2C_MEMADD_SIZE_8BIT, data, 2, 50);
	if(status != HAL_OK)
	{
	  Error_Handler();
	}
	angle = data[0] << 8;
	angle |= data[1];
	return angle;
}

