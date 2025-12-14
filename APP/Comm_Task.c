#include "Comm_Task.h"
__IO uint32_t Comm_TimeCnt = 0;   /* 数据检查间隔，检查是否接受到新的数据 单位为Comm_Task任务运行间隔 */
/******************************* 数据处理函数 开始 ************************************/
#if WIFI_ENABLE  ==  _enable_
    WIFI_Operations WIFI_Ctrler = {
        .Temp_Limit_Up = 30,
        .Humi_Limit_Down = 60
    };
    /**
     * @brief   处理Onenet下发的控制命令
     * @param   none
     * @retval  none
     */
    __weak void WIFI_DataProcess(void)
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
                    //LED_ON();
                }
                else
                {
                    //LED_OFF();
                }
            }
            /* 风扇控制 */
            if(WIFI_Ctrler.Fan_State_Flag == 1)
            {
                WIFI_Ctrler.Fan_State_Flag = 0;
                //WIFI_Ctrler.Fan_State;
            }
            /* 加湿器控制 */
            if(WIFI_Ctrler.Humidifier_State_Flag == 1)
            {
                WIFI_Ctrler.Humidifier_State_Flag = 0;
                //WIFI_Ctrler.Humidifier_State;
            }
            //.............
        }
    }
    /**
     * @brief   设置发送到Onenet云平台的数据
     * @param   none
     * @retval  none
     */
    __weak void Onenet_SetPacket(void)     /* 弱定义 可在任意.c文件中重定义 */
    {
        char text[48];
        /* 准备数据 */
        char Place[] = "hello world";
        
        memset(text, 0, sizeof(text));
        sprintf(text, "\\\"Temp\\\":{\\\"value\\\":%d}\\,", 25);
        strcat((char*)Wifi_Handler.Tx_Buff,text);
        
        memset(text, 0, sizeof(text));
        sprintf(text, "\\\"Humi\\\":{\\\"value\\\":%d}\\,", 88);
        strcat((char*)Wifi_Handler.Tx_Buff,text);
        
        memset(text, 0, sizeof(text));
        sprintf(text, "\\\"Place\\\":{\\\"value\\\":\\\"%s\\\"}",Place);
        strcat((char*)Wifi_Handler.Tx_Buff,text);
    }
#endif
#if HC05_ENABLE  ==  _enable_
    HC05_DataStruct HC05_DataInfo = {
        .Send = HC05_Printf,
    };
    static void HC05_DataProcess(void)
    {
        if(HC05_DataInfo.Update_Flag == 1)
        {
            HC05_DataInfo.Update_Flag = 0;    /* 清空标志 允许接收新的数据帧 */
            if(HC05_DataInfo.Led_State_Flag == 1)
            {
                HC05_DataInfo.Led_State_Flag = 0;
                //HC05_DataInfo.Led_State;
            }
        }
    }
#endif
#if NRF24L01_ENABLE  ==  _enable_
#include "NRF24L01.h"
    NRF24L01_Operations NRF24L01_Ctrler = 
    {
        .ReceiveFlag = 0,
        .SendFlag = 0,
        .RxPacket = {0},
        .TxPacket = {0},
        .Send = NRF24L01_Send,
        .Receive = NRF24L01_Receive
    };
    
#endif
#if ZIGBEE_ENABLE  ==  _enable_
    ZIGBEE_Operations ZIGBEE_Ctrler = {
        .Send = ZIGBEE_Printf,
    };
    static void ZIGBEE_DataProcess(void)
    {
        if(ZIGBEE_Ctrler.Update_Flag == 1)
        {
            ZIGBEE_Ctrler.Update_Flag = 0;    /* 清空标志 允许接收新的数据帧 */
            //ZIGBEE_Ctrler.Temp;   /* 温度数据 */
            //ZIGBEE_Ctrler.Humi;   /* 湿度数据 */
        }
    }
#endif
#if LORA_ENABLE  ==  _enable_
    LORA_Operations LORA_Ctrler = {
        .Send = LORA_Printf,
    };
    static void LORA_DataProcess(void)
    {
        if(LORA_Ctrler.Update_Flag == 1)
        {
            LORA_Ctrler.Update_Flag = 0;    /* 清空标志 允许接收新的数据帧 */
            //LORA_Ctrler.Temp;   /* 温度数据 */
            //LORA_Ctrler.Humi;   /* 湿度数据 */
        }
    }
#endif
#if IR_REMOTE_ENABLE  ==  _enable_
    IR_REMOTE_DataStruct IR_REMOTE_DataInfo;
    static void IR_REMOTE_GetData(void)
    {
        IR_REMOTE_DataInfo.Update_Flag = IR_Remote_GetDataFlag();
    }
#endif
/******************************* 数据处理函数 结束 ************************************/
/**
 * @brief   通信接口初始化
 * @param   none
 * @retval  none
 */
void Comm_Init(void)
{
#if WIFI_ENABLE  ==  _enable_
    WIFI_UARTInit();
    uint8_t Key_Value;
    while(1)
    {
        WIFI_Init();
        if(Wifi_Handler.Error_Code.Bytes == 0x00)
        {
            OLED_Clear();
            OLED_Printf(0,0,OLED_8X16,"WIFI_Init_Success");
            OLED_Update();
            delay_ms(1000);
            break;
        }
        else
        {
            OLED_Clear();
            OLED_Printf(0,0,OLED_8X16,"WIFI_Init_Failed");
            OLED_Printf(0,16,OLED_8X16,"KEY1 Retry");
            OLED_Printf(0,32,OLED_8X16,"KEY2 Run Main");
            OLED_Update();
            while(1)
            {
                Key_Value = Key_Scan();
                if(Key_Value == KEY1_PRESS)
                {
                    break;
                }
                else if(Key_Value == KEY2_PRESS)
                {
                    goto wifi_init_over;
                }
                
            }
        }
    }
    wifi_init_over:{
        
    }
#endif
#if HC05_ENABLE  ==  _enable_
    HC05_Init();
#endif
#if NRF24L01_ENABLE  ==  _enable_
    NRF24L01_Init();
#endif
#if ZIGBEE_ENABLE  ==  _enable_
    ZIGBEE_Init();
#endif
#if LORA_ENABLE  ==  _enable_
    LORA_Init();
#endif
#if IR_REMOTE_ENABLE  ==  _enable_
    IR_Remote_Init();
#endif
}
/**
 * @brief   通信任务
 * @param   none
 * @retval  none
 */
void Comm_Execute(void)
{
    Comm_TimeCnt++;
#if WIFI_ENABLE  ==  _enable_
    if(Comm_TimeCnt % WIFI_RUN_SPACE == 0 || Wifi_Handler.Update_Property)
    {
        /* 多模型上传示例 */
        static uint8_t WIFI_RunTime = 0;
        WIFI_RunTime++;
        if(WIFI_RunTime >= 30 || Wifi_Handler.Update_Property)    /* 3s上传一次数据 */
        {
            WIFI_RunTime = 0;
            Wifi_Handler.Update_Property = 0;
            if(Wifi_Handler.Connect.Bits.Onenet_Connect == true)    /* 检查是否连接到Onenet服务器 */
            {
                WIFI_Publish_Property(Onenet_SetPacket);
            }
        } 
    }
#endif
#if HC05_ENABLE  ==  _enable_
    if(Comm_TimeCnt % HC05_RUN_SPACE == 0)
    {
        HC05_DataProcess();
        /* 发送示例 */
//        char str[] = "hello world";
//        HC05_DataInfo.Send("str:%s\r\n", str);
    }
#endif
#if NRF24L01_ENABLE  ==  _enable_
    if(Comm_TimeCnt % NRF24L01_RUN_SPACE == 0)
    {
//        /* 发送示例 */
//        for(uint8_t i = 0; i < NRF24L01_TX_PACKET_WIDTH; i++)
//        {
//            NRF24L01_Ctrler.TxPacket[i] = i;
//        }
//        NRF24L01_Ctrler.SendFlag = NRF24L01_Ctrler.Send(NRF24L01_Ctrler.TxPacket, NRF24L01_TX_PACKET_WIDTH);
//        if(NRF24L01_Ctrler.SendFlag == 1)
//        {
//            NRF24L01_Ctrler.SendFlag = 0;
//            /* 发送成功 */
//            memset(NRF24L01_Ctrler.TxPacket, 0, NRF24L01_TX_PACKET_WIDTH);
//            
//            //...............
//        }
//        /* 接收示例 */
//        NRF24L01_Ctrler.ReceiveFlag = NRF24L01_Ctrler.Receive(NRF24L01_Ctrler.RxPacket, NRF24L01_RX_PACKET_WIDTH);
//        if(NRF24L01_Ctrler.ReceiveFlag == 1)
//        {
//            NRF24L01_Ctrler.ReceiveFlag = 0;
//            /* 这里处理接收到的数据 NRF24L01_Ctrler.RxPacket */
//            //...............
//            
//            memset(NRF24L01_Ctrler.RxPacket, 0, NRF24L01_RX_PACKET_WIDTH);
//        }
        
    }
#endif
#if ZIGBEE_ENABLE  ==  _enable_
    if(Comm_TimeCnt % ZIGBEE_RUN_SPACE == 0)
    {
        ZIGBEE_DataProcess();
        /* 发送示例 */
//        char str[] = "Temp:34,Humi:86";
//        ZIGBEE_Ctrler.Send("%s\r\n", str);
    }
#endif
#if LORA_ENABLE  ==  _enable_
    if(Comm_TimeCnt % LORA_RUN_SPACE == 0)
    {
        LORA_DataProcess();
        /* 发送示例 */
//        char str[] = "Temp:34,Humi:86";
//        LORA_Ctrler.Send("%s\r\n", str);
    }
#endif
#if IR_REMOTE_ENABLE  ==  _enable_
    if(Comm_TimeCnt % IR_REMOTE_RUN_SPACE == 0)
    {
        IR_REMOTE_GetData();
        /* 数据处理示例 */
        if(IR_REMOTE_DataInfo.Update_Flag == 1)
        {
            IR_REMOTE_DataInfo.Update_Flag = 0;
            IR_REMOTE_DataInfo.Command = IR_Remote_GetCommand();
            IR_REMOTE_DataInfo.Address = IR_Remote_GetAddress();
            if(IR_REMOTE_DataInfo.Command == 0x40)
            {
                
            }
            else if(IR_REMOTE_DataInfo.Command == 0x46)
            {
                
            }
            //...........后续控制命令根据需求设置
        }
    }
#endif
}
