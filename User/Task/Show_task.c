/**
  *********************************************************************
  * @file      Show_task.c/h
  * @brief     该任务显示角度等参数
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
#include <stdint.h>
#include "oled.h"
#include "as5600.h"

uint16_t angle;

void Show_task(void)
{
	OLED_Init();
	
	while(1)
	{
		angle = getAngle();
		OLED_ShowString(1, 1, "Angle: ");
		OLED_ShowNum(1, 8, angle, 3); 
	}
}
