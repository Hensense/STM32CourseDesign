#include "Action_Task.h"
Action_Operations Action_Ctrl = {0};

void Action_Key_Process(void);          /* 按键处理任务 */
void Action_OLED_Update(void);          /* OLED液晶显示界面更新 */
void Action_Limit_Press(void);          /* 阈值检测 - 执行动作*/
void Action_Data_Press(void);           /* 监测数据 - 执行动作 */
/**
 * @brief   动作任务：负责处理各个任务获取的数据 执行动作
 * @param   none
 * @retval  none
 */
void Action_Execute(void)
{
    Action_Ctrl.Run_TimeStamp++;
    /* 1.KEY处理 */
    Action_Ctrl.Key_Value = Key_Scan(); /* 获取按下按键键值 */
    Action_Key_Process();               /* 执行按键动作 */
    /* 2.OLED显示更新 MENU_UPDATE_SPACE时间更新一次，单位为taskScheduler.Action_Task.eventSpace时间间隔 */
    if(Action_Ctrl.Run_TimeStamp % MENU_UPDATE_SPACE == 0)  
    {
        Action_OLED_Update();
    }
    /* 3.阈值检测 - 动作执行 */
    Action_Limit_Press();
    /* 4.数据检测 - 动作执行 */
    Action_Data_Press();
}

/**
 * @brief   按键处理任务：切换菜单页/设置阈值/操控执行器/......
 * @param   none
 * @retval  none
 */
void Action_Key_Process(void)
{
    if(Action_Ctrl.Key_Value == KEY1_PRESS)         /* KEY1：切换菜单页 */
    {
        if(Action_Ctrl.Menu_Page < 1)
        {
            Action_Ctrl.Menu_Page++;
        }
        else
        {
            Action_Ctrl.Menu_Page = MENU_MAIN_PAGE; /* 返回首页 */
        }
    }
    else if(Action_Ctrl.Key_Value == KEY2_PRESS)
    {
        if(MFRC522_Value.Read_Flag == 0x01 || MFRC522_Value.Read_Flag == 0x02)
        {
            MFRC522_Value.AddCard(MFRC522_Value.Read_Id);
        }
    }
    else if(Action_Ctrl.Key_Value == KEY3_PRESS)
    {
        if(MFRC522_Value.Read_Flag == 0x01 || MFRC522_Value.Read_Flag == 0x02)
        {
            MFRC522_Value.DeleteCard(MFRC522_Value.Read_Id);
        }
    }
    else if(Action_Ctrl.Key_Value == KEY4_PRESS)
    {
        MFRC522_Value.DeleteAllCard();
    }
}
/**
 * @brief   OLED液晶显示界面更新：数据显示/阈值显示/执行器状态/......
 * @param   none
 * @retval  none
 */
void Action_OLED_Update(void)
{
    OLED_Clear();
    /* 这里填充显示数据.... */
    switch((Memu_Items)Action_Ctrl.Menu_Page)
    {
        case MENU_MAIN_PAGE:{       /* OLED液晶屏显示首页 */
            if(MFRC522_Value.Read_Flag == 0x01 || MFRC522_Value.Read_Flag == 0x02){
                OLED_Printf(0,0,OLED_8X16,"ID:0x%02X%02X%02X%02X",MFRC522_Value.Read_Id[0],
                    MFRC522_Value.Read_Id[1],MFRC522_Value.Read_Id[2],MFRC522_Value.Read_Id[3]);
				//MCU通信，使用串口发送当前ID卡号
				printf("0x%02X%02X%02X%02X\n",MFRC522_Value.Read_Id[0],
                    MFRC522_Value.Read_Id[1],MFRC522_Value.Read_Id[2],MFRC522_Value.Read_Id[3]);
            }
            else
            {
                OLED_Printf(0,0,OLED_8X16,"ID:0x00000000");
            }
            OLED_Printf(0,16,OLED_8X16,"Add:Key2");
            OLED_Printf(0,32,OLED_8X16,"Delete:Key3");
            OLED_Printf(0,48,OLED_8X16,"DeleteAll:Key4");
        }
            break;
        case MENU_PAGE_ONE:{       /* OLED液晶屏显示第一页 */
            for(uint8_t i = 0; i < MFRC522_Value.Save_IdNumber; i++)
            {
                OLED_Printf(0,i*16,OLED_8X16,"ID%d:0x%02X%02X%02X%02X",i,
                    MFRC522_Value.Save_IdArray[i][0],MFRC522_Value.Save_IdArray[i][1],
                    MFRC522_Value.Save_IdArray[i][2],MFRC522_Value.Save_IdArray[i][3]);
            }
        }
            break;
        case MENU_PAGE_TWO:{       /* OLED液晶屏显示第二页 */
            
        }
            break;
        case MENU_PAGE_THREE:{     /* OLED液晶屏显示第三页 */
            
        }
            break;
        default:
            break;
    }
    OLED_Update();
}
/**
 * @brief   监测数据，执行动作
 * @param   none
 * @retval  none
 */
void Action_Data_Press(void)
{
    
}
/**
 * @brief   阈值检测，执行动作
 * @param   none
 * @retval  none
 */
void Action_Limit_Press(void)
{
    
}
#if WIFI_ENABLE
/**
 * @brief   处理Onenet下发的控制命令
 * @param   none
 * @retval  none
 */
void WIFI_DataProcess(void)
{
    if(WIFI_Ctrler.Update_Flag == 1)
    {
        WIFI_Ctrler.Update_Flag = 0;    /* 清空标志 允许接收新的数据帧 */
        /* 灯光控制 */
        if(WIFI_Ctrler.Led_State_Flag == 1)
        {
            WIFI_Ctrler.Led_State_Flag = 0;
            if(WIFI_Ctrler.Led_State)
            {
                
            }
            else
            {
                
            }
        }
        /* 风扇控制 */
        if(WIFI_Ctrler.Fan_State_Flag == 1)
        {
            WIFI_Ctrler.Fan_State_Flag = 0;
            //WIFI_Ctrler.Fan_State
             
        }
        /* 烘干控制 */
        if(WIFI_Ctrler.Humidifier_State_Flag == 1)
        {
            WIFI_Ctrler.Humidifier_State_Flag = 0;
            //WIFI_Ctrler.Humidifier_State
        }
        //.............
    }
}
void Onenet_SetPacket(void)
{
    Wifi_Handler.Tx_Length = 0;
    /* 1.添加数据头 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "{\"id\":\"123\",\"params\":{");
    /* 2.添加上报属性 */
    /* 2.1 Bool类型上传 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Led_Ctrl\":{\"value\":%s},",WIFI_Ctrler.Led_State ? "true":"false");
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Fan_Ctrl\":{\"value\":%s},",WIFI_Ctrler.Fan_State ? "true":"false");
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Humidifier_Ctrl\":{\"value\":%s},",WIFI_Ctrler.Humidifier_State ? "true":"false");
    /* 2.2 String类型上传 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"String\":{\"value\":\"%s\"},","Hello Onenet");
    /* 2.3 整数/浮点数上传 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Temp\":{\"value\":%d},",25);
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Humi\":{\"value\":%.1f},",80.5);
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Temp_Limit_Up\":{\"value\":%d},",WIFI_Ctrler.Temp_Limit_Up);
    /* 2.4 最后一条物模型数据 如需添加新的物模型请在该数据之上，复制示例上传 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "\"Humi_Limit_Down\":{\"value\":%d}",WIFI_Ctrler.Humi_Limit_Down);
    /* 3.添加数据尾 */
    Wifi_Handler.Tx_Length += snprintf((char*)Wifi_Handler.Tx_Buff + Wifi_Handler.Tx_Length,
                                    WIFI_USART_TXBUFF_MAX - Wifi_Handler.Tx_Length,
                                    "}}\r\n\r\n");
}
#endif
