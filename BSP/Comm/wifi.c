#include "wifi.h"
#include "cJSON.h"
#include "Comm_Task.h"
#include "Action_Task.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
/* WIFI驱动 */
#if WIFI_NETWORK_START_STATE
WIFI_Handler Wifi_Handler = {0};    /* 定义WIFI控制器 */
void WIFI_DataProcess(void);        /* 接收命令处理 */
/**
 * @brief   WIFI串口初始化
 * @param   none
 * @retval  none
 */
void WIFI_UARTInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    /* 1.1 使能时钟 */
    RCC_APB2PeriphClockCmd(WIFI_GPIO_CLK, ENABLE);
    if (WIFI_USARTx == 1)
        RCC_APB2PeriphClockCmd(WIFI_USART_CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(WIFI_USART_CLK, ENABLE);
    /* 1.2 配置GPIO */
    GPIO_InitStruct.GPIO_Pin = WIFI_GPIO_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WIFI_GPIO_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = WIFI_GPIO_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(WIFI_GPIO_RX_PORT, &GPIO_InitStruct);

    /* 1.3 配置USART */
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(WIFI_USART_UX, &USART_InitStruct);

    /* 1.4 配置中断 */
    USART_ITConfig(WIFI_USART_UX, USART_IT_RXNE, ENABLE);
    USART_ITConfig(WIFI_USART_UX, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = WIFI_USART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  /* 注：抢占优先级需要设置为最高，防止USB中断打断数据接收 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* 1.5 使能USART */
    USART_Cmd(WIFI_USART_UX, ENABLE);
}
/**
 * @brief   WIFI模块初始化 连接结果通过USB发送到USB主机设备
 *  @note   1.初始化底层通信接口（USART1/2/3） 
 *          2.连接WIFI热点 
 *          3.使用MQTT协议连接Onenet
 * @param   none
 * @retval  none
 */
void WIFI_Init(void)
{
    OLED_Clear();
    OLED_Printf(28,0,OLED_8X16, "WIFI_Init");
    OLED_Printf(0,16,OLED_8X16, "AT+RST...");
    OLED_Update();
    Wifi_Handler.Error_Code.Bytes = 0;
    /* 2.1 复位WIFI模块 */
    if(WIFI_SendATCommand("AT+RST\r\n", "", 1, 1500) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_RST_ERROR = true;   /* 记录错误代码 */
        return ;                                            /* 终止函数 */
    }
    OLED_Printf(0,32,OLED_8X16, "ATE0...");
    OLED_Update();
    /* 2.2 关闭WIFI模块回显 减少数据量 */
    if(WIFI_SendATCommand("ATE0\r\n", "OK", 3, 50) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_ATE0_ERROR = true;  /* 记录错误代码 */
        return ;                                            /* 终止函数 */
    }
    OLED_Printf(0,48,OLED_8X16, "AT+CWMODE=1...");
    OLED_Update();
    /* 2.3 配置WIFI模块为客户端工作模式 可以连接WIFI热点 */
    if(WIFI_SendATCommand("AT+CWMODE=1\r\n", "OK", 3, 50) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_CWMODE_ERROR = true;/* 记录错误代码 */
        return ;                                            /* 终止函数 */
    }
    OLED_Clear();
    OLED_Printf(28,0,OLED_8X16, "WIFI_Init");
    OLED_Printf(0,16,OLED_8X16, "AT+CWJAP...");
    OLED_Update();
    /* 2.4 连接WIFI热点 */
    sprintf((char*)Wifi_Handler.Tx_Buff, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "OK", 3, 2000) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_CWJAP_ERROR = true; /* 记录错误代码 */
        return ;                                            /* 终止函数 */
    }
    Wifi_Handler.Connect.Bits.Wifi_Connect = true;          /* 标记WIFI已连接 */
    OLED_Printf(0,32,OLED_8X16, "AT+MQTTUSERCFG...");
    OLED_Update();
    /* 3.1 配置MQTT连接参数 */
    sprintf((char*)Wifi_Handler.Tx_Buff, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", WIFI_ONENET_DEVICE_NAME, WIFI_ONENET_PRODUCT_ID, WIFI_ONENET_TOKEN);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "OK", 3, 100) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_MQTTUSERCFG_ERROR = true;   /* 记录错误代码 */
        return ;                                                    /* 终止函数 */
    }
    OLED_Printf(0,48,OLED_8X16, "AT+MQTTCONN...");
    OLED_Update();
    /* 3.2 连接Onenet服务器 */
    sprintf((char*)Wifi_Handler.Tx_Buff, "AT+MQTTCONN=0,\"%s\",%d,%d\r\n", MQTT_HOST_URL, MQTT_HOST_PORT, 1);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "OK", 3, 500) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_MQTTCONN_ERROR = true;  /* 记录错误代码 */
        return ;                                                /* 终止函数 */
    }
    Wifi_Handler.Connect.Bits.Onenet_Connect = true;    /* 标记Onenet服务器已连接 */
    OLED_Clear();
    OLED_Printf(28,0,OLED_8X16, "WIFI_Init");
    OLED_Printf(0,16,OLED_8X16, "AT+MQTTSUB1...");
    OLED_Update();
    /* 3.3 订阅设备属性上报响应主题 */
    sprintf((char*)Wifi_Handler.Tx_Buff, "AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",1\r\n", WIFI_ONENET_PRODUCT_ID, WIFI_ONENET_DEVICE_NAME);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "OK", 3, 100) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_MQTTSUB_ERROR = true;   /* 记录错误代码 */
        return ;                                                /* 终止函数 */
    }
    Wifi_Handler.Connect.Bits.Subscribe_GetReply = true;/* 标记已订阅设备属性上报响应主题 */
    OLED_Printf(0,32,OLED_8X16, "AT+MQTTSUB2...");
    OLED_Update();
    /* 3.4 订阅设备属性设置请求 */
    sprintf((char*)Wifi_Handler.Tx_Buff, "AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/set\",1\r\n", WIFI_ONENET_PRODUCT_ID, WIFI_ONENET_DEVICE_NAME);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "OK", 3, 100) != WIFI_OK)
    {
        Wifi_Handler.Error_Code.Bits.AT_MQTTSUB_ERROR = true;   /* 记录错误代码 */
        return ;                                                /* 终止函数 */
    }
    Wifi_Handler.Connect.Bits.Subscribe_SetTopic = true;/* 标记已订阅设备属性设置请求 */
}
/**
 * @brief   WIFI中断回调函数 用于接收数据
 * @param   none
 * @retval  noen
 */
void WIFI_USART_IRQHandler(void)
{
    /* 处理接收中断 */
    if (USART_GetITStatus(WIFI_USART_UX, USART_IT_RXNE) != RESET)
    {
#if USB_LOG_ENABLE
        /* 发送接收到的数据到主机 */
        USART_To_USB_Send_Data();
#endif
        /* 将接收到的数据存入WIFI数据接收缓冲区 */
        Wifi_Handler.Rx_Buff[Wifi_Handler.Rx_Length++] = USART_ReceiveData(WIFI_USART_UX);
        if(Wifi_Handler.Rx_Length >= WIFI_USART_RXBUFF_MAX)
        {
            Wifi_Handler.Rx_Length = 0;
        }
        if(Wifi_Handler.Rx_Length >= 2 && (Wifi_Handler.Rx_Buff[Wifi_Handler.Rx_Length-2] == '\r' && Wifi_Handler.Rx_Buff[Wifi_Handler.Rx_Length-1] == '\n'))
        {
            Wifi_Handler.Rx_DataFlag = 1;   /* 标记数据接收完成 */
            if(WIFI_GetOnenetCommand() == WIFI_OK)
            {
                WIFI_ProcessOnenetCommand();    /* 响应Onenet 解析命令 */
                WIFI_DataProcess();             /* 解析命令后及时处理命令 */
                /* 发送Onenet命令响应 */
                Wifi_Handler.Response_Flag = 1;
                Wifi_Handler.Update_Property = 1;   /* 立即进行一次物模型数据更新 */
            }
        }
    }
    /* 处理溢出错误 */
    if (USART_GetFlagStatus(WIFI_USART_UX, USART_FLAG_ORE) != RESET)
    {
        (void)USART_ReceiveData(WIFI_USART_UX);
    }
    /* 处理空闲中断 */
    if(USART_GetITStatus(WIFI_USART_UX, USART_IT_IDLE))
    {
        (void)WIFI_USART_UX->DR;        /* 清除空闲中断标志 */
//        Wifi_Handler.Rx_Length = 0;     /* 置位接收指针 */
    }
}
/**
 * @brief   清空WIFI接收数据缓冲区 置位接收长度
 * @param   none
 * @retval  none
 */
void WIFI_ClearRxBuffer(void)
{
    memset(Wifi_Handler.Rx_Buff, 0, sizeof(Wifi_Handler.Rx_Buff));
    Wifi_Handler.Rx_Length = 0;
}
/**
 * @brief   向WIFI模块发送一字节
 * @param   byte    :    要发送的数据
 * @retval  none
 */
void WIFI_SendByte(uint8_t byte)
{
    USART_SendData(WIFI_USART_UX, byte);
    while(USART_GetFlagStatus(WIFI_USART_UX, USART_FLAG_TXE) == RESET); /* 等待数据发送完成 */
}
/**
 * @brief   向WIFI模块发送字符串
 * @param   str ：字符串数组
 * @retval  none
 */
void WIFI_SendString(char *str)
{
    while(*str != '\0')
    {
        WIFI_SendByte(*str++);
    }
}
/**
 * @brief   等待数据接收完成 本函数需要间隔2~10ms再次调用
 * @param   none
 * @retval  
 *  @note   WIFI_REV_WAIT    :   正在等待数据接收
 *          WIFI_REV_OK      :   数据接收完成
 */
uint8_t WIFI_WaitReceive(void)
{
    static uint32_t Rx_cntPre = 0;
    if(Wifi_Handler.Rx_Length == 0)         /* 正在等待接收 */
        return WIFI_REV_WAIT;
    if(Wifi_Handler.Rx_Length == Rx_cntPre) /* 本次值和上次值相同 数据接收完毕 */
    {
        Wifi_Handler.Rx_Length = 0;
        return WIFI_REV_OK;
    }
    Rx_cntPre = Wifi_Handler.Rx_Length;     /* 记录上次接收值 */
    return WIFI_REV_WAIT;
}
/**
 * @brief   获取数据接收完成标志位
 * @param   none
 * @retval  1：数据已接收完成 0：数据未接收完成
 */
uint8_t WIFI_GetRxCompleteFlag(void)
{
    uint8_t rec_flag = Wifi_Handler.Rx_DataFlag;
    Wifi_Handler.Rx_DataFlag = 0;
    return rec_flag;
}
/**
 * @brief   检测是否收到Onenet下发下来的控制命令
 * @param   none
 * @retval  WIFI_OK : 收到控制命令 WIFI_FALSE : 没有收到控制命令
 */
uint8_t WIFI_GetOnenetCommand(void)
{
    if(strstr((char*)Wifi_Handler.Rx_Buff, WIFI_ONENET_REVEIVE_SET_TOPIC) != NULL)
        return WIFI_OK;
    return WIFI_FALSE;
}
/**
 * @brief   检测发布的命令是否是Onenet响应命令（上传）
 * @param   none
 * @retval  WIFI_OK : 收到控制命令 WIFI_FALSE : 没有收到控制命令
 */
uint8_t WIFI_GetOnenetResponseCommand(void)
{
    if(strstr((char*)Wifi_Handler.Tx_Buff, WIFI_ONENET_SEND_SET_TOPIC_RESPONSE) != NULL)
        return WIFI_OK;
    return WIFI_FALSE;
}
/**
 * @brief   向WIFI模块发送AT指令
 * @param   command      :   AT指令字符串数组
 * @param   response     :   接收响应字符串
 * @param   retry        :   重试次数
 * @param   delay_xms    :   最小等待延时时间 最少需要等待该时间才能判断数据是否接收完成
 * @retval  
 */
uint8_t WIFI_SendATCommand(char* command, char *response, uint8_t retry, uint16_t delay_xms)
{
#if USB_LOG_ENABLE
    USB_SendData("Send AT Command:");
    USB_SendData(command);
    USB_SendData("Receive Moudle Response:\r\n");
#endif
    uint16_t timeOut = 50;  /* 接收响应超时时间 */
    while(retry--)
    {
        /* 1.发送AT指令 */
        WIFI_SendString(command);
        delay_ms(delay_xms);
        /* 2.等待响应 */
        while(timeOut--)
        {
            if(WIFI_GetOnenetCommand() == WIFI_OK && WIFI_GetOnenetResponseCommand() != WIFI_OK) 
                return WIFI_FALSE;   /* 发送AT命令时接收到Onenet下发控制信息 终止本次命令接收 */
            if(WIFI_WaitReceive() == WIFI_REV_OK)  /* 等待数据接收完成 */
            {
                if(strstr((const char *)Wifi_Handler.Rx_Buff, response) != NULL)        /* 查找响应数据 */
                {
                    WIFI_ClearRxBuffer(); /* 清空接收缓冲区 */
                    return WIFI_OK;     /* 指令发送成功 */
                }
            }
            if(timeOut == 0)
            {
                #if USB_LOG_ENABLE
                USB_SendData("Error response:\r\n");               /* 打印错误信息 */
                USB_SendData((char*)Wifi_Handler.Rx_Buff);
                #endif
                WIFI_ClearRxBuffer();
                timeOut = 50;  /* 重新等待 */
                break;          /* 进行下一次指令发送 */
            }
            delay_ms(10);
        }
    }
    return WIFI_FALSE;  /* 指令发送失败 */
}
/**
 * @brief   获取服务器连接状态
 *  @note   mqtt断开连接：+MQTTDISCONNECTED:0
 *          mqtt恢复连接：+MQTTCONNECTED:0,1,"mqtts.heclouds.com","1883","",1
 * @param   none
 * @retval  MQTT服务器连接状态 WIFI_FALSE：断开连接 WIFI_OK：已连接 0xFF：没有收到连接状态
 */
uint8_t WIFI_GetMQTTConnectionStatus(void)
{
    if(strstr((char*)Wifi_Handler.Rx_Buff, "+MQTTDISCONNECTED:0") != NULL) return WIFI_FALSE;
    if(strstr((char*)Wifi_Handler.Rx_Buff, "+MQTTCONNECTED:0,1,\"mqtts.heclouds.com\",\"1883\",\"\",1") != NULL) return WIFI_OK;
    return 0xFF;
}
/**
 * @brief   获取WIFI连接情况
 *  @note   WIFI恢复连接：WIFI GOT IP
 *          WIFI断开连接：WIFI DISCONNECT
 * @param   none
 * @retval  WIFI连接状态 WIFI_FALSE：断开连接 WIFI_OK：已连接 0xFF：没有收到连接状态
 */
uint8_t WIFI_GetWifiConnectionStatus(void)
{
    if(strstr((char*)Wifi_Handler.Rx_Buff, "WIFI DISCONNECT") != NULL) return WIFI_FALSE;
    if(strstr((char*)Wifi_Handler.Rx_Buff, "WIFI GOT IP") != NULL) return WIFI_OK;
    return 0xFF;
}
/**
 * @brief   收到Onenet下发的控制命令后回复
 * @param   none
 * @retval  none
 */
void WIFI_SendOnenetCommandResponse(uint32_t response_id, uint16_t response_code, char* response_mag)
{
    sprintf((char*)Wifi_Handler.Tx_Buff, 
        "AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/set_reply\",\"{\\\"id\\\":\\\"%d\\\"\\,\\\"code\\\":%d\\,\\\"msg\\\":\\\"%s\\\"}\",0,0\r\n",
        WIFI_ONENET_PRODUCT_ID, WIFI_ONENET_DEVICE_NAME, response_id, response_code, response_mag);
    WIFI_SendString((char*)Wifi_Handler.Tx_Buff);
}

/**
 * @brief   处理WIFI模块接收到的数据帧 接受完一帧数据后调用本函数
 * @retval  none
 */
void WIFI_ParseRxData(void)
{
    /* 获取WIFI连接状态 */
    if(WIFI_GetWifiConnectionStatus() == WIFI_FALSE)  Wifi_Handler.Connect.Bits.Wifi_Connect = false;
    if(WIFI_GetWifiConnectionStatus() == WIFI_OK)  Wifi_Handler.Connect.Bits.Wifi_Connect = true;
    /* 获取Onenet服务器连接状态 */
    if(WIFI_GetMQTTConnectionStatus() == WIFI_FALSE)  Wifi_Handler.Connect.Bits.Onenet_Connect = false;
    if(WIFI_GetMQTTConnectionStatus() == WIFI_OK)  Wifi_Handler.Connect.Bits.Onenet_Connect = true;
    WIFI_ClearRxBuffer(); /* 清除接收数据缓冲区 */
}
/**
 * @brief   使用MQTT发布长数据指令上报数据 封包发送数据至Onenet 可以封装多个属性数据上传 
 *  @note       第一次发送：AT+MQTTPUBRAW=0,"$sys/es7E8xqWSM/stm32/thing/property/post",54,0,0
 *              第二次发送：{"id":"123","params":{"Led_Ctrl":{"value":false},"Fan_Ctrl":{"value":false},"Place":{"value":"Hello Onenet"},"Temp":{"value":30},"Humi":{"value":91},"Temp_Limit_Up":{"value":38},"Temp_Limit_Down":{"value":18}}}
 * @param   CallbackFunc Onet_SetPacket ：函数指针，指向一个设置发送属性和数据的函数
 * @retval  none
 */
void WIFI_Publish_Property(CallbackFunc Onenet_SetPacket)
{
    uint8_t  AT_Command[256] = {0}; /* 存放MQTT发布长数据指令 */
    /* 1.清空发送数据缓存区 */
    memset((char*)Wifi_Handler.Tx_Buff, 0, sizeof(Wifi_Handler.Tx_Buff));
    /* 2.封装数据，存入缓冲区 */
    Onenet_SetPacket();
//#if USB_LOG_ENABLE
//    USB_Printf("\r\nOnenet Data Packet Lentgh:%d\r\n",Wifi_Handler.Tx_Length);
//    USB_Printf("Onenet Data Packet:\r\n");
//    USB_SendString((char*)Wifi_Handler.Tx_Buff);
//    USB_Printf("\r\n");
//#endif
    /* 3.发送AT命令进入上报长数据模式 */
    sprintf((char*)AT_Command,"AT+MQTTPUBRAW=0,\"$sys/%s/%s/thing/property/post\",%d,0,0\r\n"
                ,WIFI_ONENET_PRODUCT_ID, WIFI_ONENET_DEVICE_NAME, Wifi_Handler.Tx_Length);
    if(WIFI_SendATCommand((char*)AT_Command, "OK", 1, 5) == WIFI_OK);
    else return ;   /* AT指令错误 终止本次数据发送 */
    /* 4.发送JSON字符串 如果等待响应会造成200~500ms的延时*/
    //WIFI_SendString((char*)Wifi_Handler.Tx_Buff);
    if(WIFI_SendATCommand((char*)Wifi_Handler.Tx_Buff, "{\"id\":\"123\",\"code\":200,\"msg\":\"success\"}", 1, 150) == WIFI_OK)
        Wifi_Handler.Connect.Bits.Publish = true;
    else Wifi_Handler.Connect.Bits.Publish = false;/* 标记数据发布失败 */
}
/**
 * @brief   处理Onenet下发的控制命令
 *  @note   控制命令示例：+MQTTSUBRECV:0,"$sys/es7E8xqWSM/stm32/thing/property/set",53,
 *                          {"id":"3","version":"1.0","params":{"Led_Ctrl":true}}
 *          需要提取id（用于响应命令）和控制命令（执行动作）
 * @param   none
 * @retval  none
 */
void WIFI_ProcessOnenetCommand(void)
{
    char* jsonStart = strchr((char*)Wifi_Handler.Rx_Buff, '{'); /* 查找JSON字符串 */
    if(jsonStart == NULL)
    {
        return;     /* 没有找到JSON字符串 */
    }
    cJSON* root = cJSON_Parse(jsonStart);   /* 解析JSON字符串 */
    if(root == NULL) 
    {
        return;     /* 解析JSON字符串失败：可能是栈空间过小/数据接收不完整 */
    }
    /* 提取id字段数据 */
    cJSON* item_id = cJSON_GetObjectItem(root, "id");
    if(cJSON_IsString(item_id) && (item_id->valuestring != NULL))
    {
        Wifi_Handler.Response_Id = atoi(item_id->valuestring);    /* 获取id 转为整形 */
    }
    /* 提取params字段数据 提取控制指令 */
    cJSON* item_params = cJSON_GetObjectItem(root, "params");
    if(item_params)
    {
        /***************************开始处理接收到的信息******************************/
        /* 解析灯光控制命令 */
        cJSON* Led_State = cJSON_GetObjectItem(item_params, "Led_Ctrl");
        if(Led_State){
            WIFI_Ctrler.Led_State = Led_State->valueint;
            WIFI_Ctrler.Led_State_Flag = 1;
        }
        /* 解析风扇控制命令 */
        cJSON* Fan_State = cJSON_GetObjectItem(item_params, "Fan_Ctrl");
        if(Fan_State){
            WIFI_Ctrler.Fan_State = Fan_State->valueint;
            WIFI_Ctrler.Fan_State_Flag = 1;
        }
        /* 解析加湿器控制命令 */
        cJSON* Humidifier_State = cJSON_GetObjectItem(item_params, "Humidifier_Ctrl");
        if(Humidifier_State){
            WIFI_Ctrler.Humidifier_State = Humidifier_State->valueint;
            WIFI_Ctrler.Humidifier_State_Flag = 1;
        }
        /* 解析温度上限数值 */
        cJSON* Temp_Limit_Up = cJSON_GetObjectItem(item_params, "Temp_Limit_Up");
        if(Temp_Limit_Up){
            WIFI_Ctrler.Temp_Limit_Up = Temp_Limit_Up->valueint;
        }
        /* 解析湿度下限数值 */
        cJSON* Humi_Limit_Down = cJSON_GetObjectItem(item_params, "Humi_Limit_Down");
        if(Humi_Limit_Down){
            WIFI_Ctrler.Humi_Limit_Down = Humi_Limit_Down->valueint;
        }
        //此处添加新的需要解析的物模型数据..........
        /***************************结束处理接收到的信息******************************/
    }
    cJSON_Delete(root); /* 删除JSON对象 */
    WIFI_Ctrler.Update_Flag = 1;  /* 标记完成一帧ONENET下发命令的接收 */
}
#endif

