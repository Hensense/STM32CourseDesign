#include "Sensor_Task.h"
__IO uint32_t Sensor_TimeCnt = 0;   /* 传感器数据采集任务运行时间 单位为Sensor_Task任务运行间隔；如运行间隔为1ms 单位则为1ms */
/******************************* 数据采集函数 开始 ************************************/
#if DHT11_ENABLE == _enable_
    DHT11_Data_TypeDef DHT11_Data;
    static void DHT11_GetData(void)
    {
        if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
        {
            //DHT11_Data.humi_int /* 湿度数据 */
            //DHT11_Data.temp_int /* 温度数据 */
        }
    }
#endif

#if BH1750_ENABLE == _enable_
    uint16_t Light_Value;
    static void BH1750_GetData(void)
    {
        Light_Value = BH1750_Get_Light();
    }
#endif

#if LDR_ENABLE == _enable_
    uint16_t LDR_Lux;
    static void LDR_GetData(void)
    {
         LDR_Lux = LDR_GetValue();
    }
   
#endif

#if IR_ENABLE == _enable_
    uint16_t IR_Flag;
    static void IR_GetData(void)
    {
        IR_Flag = IR_FireData();
    }
#endif

#if HW_ENABLE == _enable_
    uint8_t* HW_State;
    static void HW_GetData(void)
    {
        HW_State = HW_GetFlag();
    }
#endif

#if TS_ENABLE == _enable_
    uint16_t TS_Value;
    static void TS_GetData(void)
    {
        TS_Value = TS_GetValue();
    }
#endif

#if DS18B20_ENABLE == _enable_
    float DS18B20_Value;          /* 温度采集值，供外界使用 */
    static void DS18B20_GetData(void)
    {
        DS18B20_Value = DS18B20_Get_Temp() * 0.1;
    }
#endif
#if ADXL345_ENABLE == _enable_
    AdxAngle adxAngle;
    static void ADXL345_GetData(void)
    {
        ADXL345_GetAngle(&adxAngle.x,&adxAngle.y,&adxAngle.z);
    }
#endif
#if MAX30102_ENABLE == _enable_
    MaxXlXy maxXlXy;                     /* 心率血氧采集值，供外界使用 */
    static void MAX30102_GetData(void)
    {
        if(maxXlXy.int_flag)
        {
            maxXlXy.int_flag = 0;   /* 清除中断标志位 */
            MAX30102_GetXlXy(&maxXlXy.xl, &maxXlXy.xy);
        }
        else
        {
            maxXlXy.pre_time++;
            if(maxXlXy.pre_time > 30)   /* 30个MAX30102_RUN_SPACE间隔数据未更新，手已经松开 */
            {
                maxXlXy.pre_time = 0;
                maxXlXy.xl = 0;
                maxXlXy.xy = 0;
            }
        }
    }
#endif
#if XINLV_ENABLE == _enable_
    XinLv_DataStruct XinLv_DataInfo = {0};
    static void XINLV_GetData(void)
    {
        XinLv_DataInfo.XINLV_Value = XINLV_GetValue();
    }
#endif
#if BMP280_ENABLE == _enable_
    Bmp280Data bmp280Data;
    static void BMP280_GetData(void)
    {
        BMP280GetData(&bmp280Data.pressure, &bmp280Data.temperature, &bmp280Data.asl);
    }
#endif
#if MQ2_ENABLE == _enable_
    float MQ2_Value;
    static void MQ2_GetData(void)
    {
        MQ2_Value = MQ2_GetData_PPM();
    }
#endif
#if MQ3_ENABLE == _enable_
    float MQ3_Value;
    static void MQ3_GetData(void)
    {
        MQ3_Value = MQ3_GetValue();
    }
#endif
#if MQ4_ENABLE == _enable_
#include    "mq4.h"
    float MQ4_Value; 
    static void MQ4_GetData(void)
    {
        MQ4_Value = MQ4_GetPPM();
    }
#endif
#if MQ7_ENABLE == _enable_
    float MQ7_Value;
    static void MQ7_GetData(void)
    {
        MQ7_Value = MQ7_GetData_PPM();
    }
#endif
#if MQ135_ENABLE == _enable_
    float MQ135_Value;
    static void MQ135_GetData(void)
    {
        MQ135_Value = MQ135_GetData_PPM();
    }
#endif
#if BODY_HW_ENABLE == _enable_
    uint8_t BodyHw_State;
    static void BODY_HW_GetData(void)
    {
        BodyHw_State = BODY_HW_GetValue();
    }
#endif
#if SR04_ENABLE == _enable_
    uint32_t SR04_Value;
    static void SR04_GetData(void)
    {
        SR04_Value = SR04_GetDistance();
    }
#endif

#if CCS811_ENABLE == _enable_
    float CCS811_Value;              
    static void CCS811_GetData(void)
    {
        CCS811_Value = CCS811_GetValue();
    }
#endif
#if MFRC522_ENABLE == _enable_
    MFRC522_Data MFRC522_Value = {
        .AddCard = MFRC522_AddCard,
        .DeleteCard = MFRC522_DeleteCard,
        .DeleteAllCard = MFRC522_DeleteAllCard
    };
    static void MFRC522_GetData(void)
    {
        MFRC522_Value.Read_Flag = MFRC522_ReadCard(&MFRC522_Value.Read_Id[0]); //读取一张卡并返回读取状态
    }
    
#endif

#if GATE_GUARD_ENABLE == _enable_
    uint8_t Gate_Guard_Flag;           /* 0x00：关 0x01：开 0x02：保持开 */
    static void GATE_GUARD_GetData(void)
    {
        Gate_Guard_Flag = GATE_GUARD_GetState();
    }
#endif

#if BUTTON44_ENABLE == _enable_
    char BUTTON_KeyFlag;
    static void BUTTON44_GetData(void)
    {
        BUTTON_KeyFlag = Button4_4_Scan();
    }
#endif

#if FSR402_ENABLE == _enable_
    uint16_t FSR402_Value;
    static void FSR402_GetData(void)
    {
        FSR402_Value = FSR402_GetStress();
    }
#endif
#if SHT30_ENABLE == _enable_
    SHT30_Data SHT30_Value;
    static void SHT30_GetData(void)
    {
        SHT30_Read_Humiture(&SHT30_Value.Temp, &SHT30_Value.Humi);
    }
#endif
#if PM25_ENABLE == _enable_
    uint16_t PM25_Value;
    static void PM25_GetData(void)
    {
        PM25_Value = PM25_GetValue();
    }
#endif
#if HX711_ENABLE == _enable_
    float HX711_Value;
    static void HX711_GetData(void)
    {
        HX711_Value = HX711_GetWeight();
    }
#endif
#if JW01_ENABLE == _enable_
    uint16_t JW01_Value;
    static void JW01_GetData(void)
    {
        JW01_GetCo2(&JW01_Value);
    }
#endif
#if WATER_ENABLE == _enable_
    uint16_t WATER_Value;
    static void WATER_GetData(void)
    {
        WATER_Value = WATER_GetValue();
    }
#endif
#if RAIN_ENABLE == _enable_
    uint16_t RAIN_Value;
    static void RAIN_GetData(void)
    {
        RAIN_Value = RAIN_GetValue();
    }
#endif
#if TDS_ENABLE == _enable_
    float TDS_Value;
    static void TDS_GetData(void)
    {
        TDS_Value = TDS_GetData_PPM();
    }
#endif
#if TS300B_ENABLE == _enable_
    float TS300B_Value;
    static void TS300B_GetData(void)
    {
        TS300B_Value = TS300B_GetValue();
    }
#endif
#if PH_ENABLE == _enable_
    float PH_Value;
    static void PH_GetData(void)
    {
        PH_Value = PH_GetValue();
    }
#endif
#if JW013_ENABLE == _enable_
    float JW013_Value;
    static void JW013_GetData(void)
    {
        JW013_Value = JW013_GetCH2O();
    }
#endif
#if AS608_ENABLE == _enable_
    AS608_Operations AS608_Ctrler = {
        .AddFR = AS608_AddFR,
        .PressFR = AS608_PressFR,
        .DeleteFR = AS608_DeleteFR,
        .GetNumber = AS608_GetFRNumber
    };
#endif
#if MPU6050_ENABLE == _enable_
    MPU6050_DataStruct MPU6050_DataInfo = { 0 };
    static void MPU6050_GetData(void)
    {
        if(FALL_DEC_FLAG)
        {
            MPU6050_DataInfo.LastAccX = MPU6050_DataInfo.AccX;  /* 记录上次状态 */
            MPU6050_DataInfo.LastAccY = MPU6050_DataInfo.AccY;
        }
        MPU6050_GetValue(&MPU6050_DataInfo.AccX, &MPU6050_DataInfo.AccY, &MPU6050_DataInfo.AccZ,
                            &MPU6050_DataInfo.GyroX, &MPU6050_DataInfo.GyroY,&MPU6050_DataInfo.GyroZ);
    }
#endif
#if MLX90614_ENABLE == _enable_
    MLX90614_Operations MLX90614_Ctrler = {
        .GetTemp = MLX90614_GetTemperature
    };
#endif
#if INA226_ENABLE == _enable_
    INA226_DataStruct INA226_DataInfo;         /* 电压、电流、功率信息 */
    static void INA226_GetData(void)
    {
        INA226_DataInfo.Bus_Voltage = INA226_GetVoltage();
        INA226_DataInfo.Shunt_Voltage = INA226_GetShuntVoltage();
        INA226_DataInfo.Current = INA226_GetCurrent();
        INA226_DataInfo.Power = INA226_GetPower();
    }
#endif
#if GPS_ENABLE == _enable_
    GPS_DataStruct GPS_DataInfo;
    static void GPS_GetData(void)
    {
        GPS_GetPosition(&GPS_DataInfo.Latitude, &GPS_DataInfo.Longitude);
    }
#endif
#if SW420_ENABLE == _enable_
    float SW420_Flag;
    static void SW420_GetData(void)
    {
        SW420_Flag = SW420_GetFlag();
    }
#endif
#if D01_ENABLE == _enable_
    float D01_Value;
    static void D01_GetData(void)
    {
        D01_Value = D01_GetValue();
    }
#endif
#if SGP30_ENABLE == _enable_
    SGP30_DataStruct SGP30_DataInfo;
    static void SGP30_GetData(void)
    {
        SGP30_GetValue(&SGP30_DataInfo.Co2, &SGP30_DataInfo.Tvoc);
    }
#endif
#if MS1100_ENABLE == _enable_
    uint16_t MS1100_Value;
    static void MS1100_GetData(void)
    {
        MS1100_GetValue();
    }
#endif
#if COUNT_ENABLE == _enable_
    uint16_t Count_Value;
    static void Count_GetData(void)
    {
        Count_Value = CountSensor_Get();
    }
#endif
/******************************* 数据采集函数 结束 ************************************/
    
/******************************* 底层接口初始化 开始 **********************************/
/**
 * @brief   传感器底层接口初始化
 * @param   none
 * @retval  none
 */
void Sensor_Init(void)
{
#if DHT11_ENABLE == _enable_
    DHT11_Init();
#endif
    
#if BH1750_ENABLE == _enable_
    BH1750_Init();
#endif
    
#if LDR_ENABLE == _enable_
    LDR_Init();
#endif
#if IR_ENABLE == _enable_
    IR_Init();
#endif
#if HW_ENABLE == _enable_
    HW_Init();
#endif
#if TS_ENABLE == _enable_
    TS_Init();
#endif
#if DS18B20_ENABLE == _enable_
    DS18B20_Init();
#endif
#if ADXL345_ENABLE == _enable_
    ADXL345_Init();
#endif
#if MAX30102_ENABLE == _enable_
    MAX30102_Init();
#endif
#if XINLV_ENABLE == _enable_
    XINLV_Init();
#endif
#if BMP280_ENABLE == _enable_
    BMP280_Init();
#endif
#if MQ2_ENABLE == _enable_
    MQ2_Init();
#endif
#if MQ3_ENABLE == _enable_
    MQ3_Init();
#endif
#if MQ4_ENABLE == _enable_
    MQ4_Init();
#endif
#if MQ7_ENABLE == _enable_
    MQ7_Init();
#endif
#if MQ135_ENABLE == _enable_
    MQ135_Init();
#endif
#if BODY_HW_ENABLE == _enable_
    BODY_HW_Init();
#endif
#if SR04_ENABLE == _enable_
    SR04_Init(0XFFFF, 72-1);
#endif
#if CCS811_ENABLE == _enable_
    CCS811_Init();
#endif
#if MFRC522_ENABLE == _enable_
    MFRC522_Init();
#endif
#if GATE_GUARD_ENABLE == _enable_
    GATE_GUARD_Init();
#endif
#if BUTTON44_ENABLE == _enable_
    Button4_4_Init();
#endif
#if FSR402_ENABLE == _enable_
    FSR402_Init();
#endif
#if SHT30_ENABLE == _enable_
    SHT30_Init();
#endif
#if PM25_ENABLE == _enable_
    PM25_Init();
#endif
#if HX711_ENABLE == _enable_
    HX711_Init();
#endif
#if JW01_ENABLE == _enable_
    JW01_Init();
#endif
#if WATER_ENABLE == _enable_
    WATER_Init();
#endif
#if RAIN_ENABLE == _enable_
    RAIN_Init();
#endif
#if TDS_ENABLE == _enable_
    TDS_Init();
#endif
#if TS300B_ENABLE == _enable_
    TS300B_Init();
#endif
#if PH_ENABLE == _enable_
    PH_Init();
#endif
#if JW013_ENABLE == _enable_
    JW013_Init();
#endif
#if AS608_ENABLE == _enable_
    AS608_Init();
#endif
#if MPU6050_ENABLE == _enable_
    MPU6050_Init();
    if(FALL_DEC_FLAG)
    {
        MPU6050_GetValue(&MPU6050_DataInfo.AccX, &MPU6050_DataInfo.AccY, &MPU6050_DataInfo.AccZ,
                            &MPU6050_DataInfo.GyroX, &MPU6050_DataInfo.GyroY,&MPU6050_DataInfo.GyroZ);
        MPU6050_DataInfo.LastAccX = MPU6050_DataInfo.AccX;  /* 同步状态 */
        MPU6050_DataInfo.LastAccY = MPU6050_DataInfo.AccY;
    }
#endif
#if MLX90614_ENABLE == _enable_
    MLX90614_Init();
#endif
#if INA226_ENABLE == _enable_
    INA226_Init();
#endif
#if GPS_ENABLE == _enable_
    GPS_Init();
#endif
#if SW420_ENABLE == _enable_
    SW420_Init();
#endif
#if D01_ENABLE == _enable_
    D01_Init();
#endif
#if SGP30_ENABLE == _enable_
    SGP30_Init();
#endif
#if MS1100_ENABLE == _enable_
    MS1100_Init();
#endif
#if COUNT_ENABLE == _enable_
    CountSensor_Init();
#endif
}
/******************************* 底层接口初始化 结束 **********************************/

/******************************* 感器数据获取 开始 ************************************/
/**
 * @brief   传感器数据获取任务
 * @param   none
 * @retval  none
 */
void Sensor_Execute(void)
{
#if DHT11_ENABLE == _enable_
    if(Sensor_TimeCnt % DHT11_RUN_SPACE == 0)
    {
        DHT11_GetData();
    }
#endif
#if BH1750_ENABLE == _enable_
    if(Sensor_TimeCnt % BH1750_RUN_SPACE == 0)
    {
        BH1750_GetData();
    }
#endif
#if LDR_ENABLE == _enable_
    if(Sensor_TimeCnt % LDR_RUN_SPACE == 0)
    {
        delay_ms(20);
        LDR_GetData();
    }
#endif
#if IR_ENABLE == _enable_
    if(Sensor_TimeCnt % IR_RUN_SPACE == 0)
    {
        IR_GetData();
    }
#endif
#if HW_ENABLE == _enable_
    if(Sensor_TimeCnt % HW_RUN_SPACE == 0)
    {
        HW_GetData();
    }
#endif
#if TS_ENABLE == _enable_
    if(Sensor_TimeCnt % TS_RUN_SPACE == 0)
    {
        TS_GetData();
    }
#endif
#if DS18B20_ENABLE == _enable_
    if(Sensor_TimeCnt % DS18B20_RUN_SPACE == 0)
    {
        DS18B20_GetData();
    }
#endif
#if ADXL345_ENABLE == _enable_
    if(Sensor_TimeCnt % ADXL345_RUN_SPACE == 0)
    {
        ADXL345_GetData();
    }
#endif
#if MAX30102_ENABLE == _enable_
    if((Sensor_TimeCnt % MAX30102_RUN_SPACE == 0) || maxXlXy.int_flag)
    {
        MAX30102_GetData();
    }
#endif
#if XINLV_ENABLE == _enable_
    if(Sensor_TimeCnt % XINLV_RUN_SPACE == 0)
    {
        XINLV_GetData();
    }
#endif
#if BMP280_ENABLE == _enable_
    if(Sensor_TimeCnt % BMP280_RUN_SPACE == 0)
    {
       BMP280_GetData();
    }
#endif
#if MQ2_ENABLE == _enable_
    if(Sensor_TimeCnt % MQ2_RUN_SPACE == 0)
    {
       MQ2_GetData();
    }
#endif
#if MQ3_ENABLE == _enable_
    if(Sensor_TimeCnt % MQ3_RUN_SPACE == 0)
    {
       MQ3_GetData();
    }
#endif
#if MQ4_ENABLE == _enable_
    if(Sensor_TimeCnt % MQ4_RUN_SPACE == 0)
    {
       MQ4_GetData();
    }
#endif
#if MQ7_ENABLE == _enable_
    if(Sensor_TimeCnt % MQ7_RUN_SPACE == 0)
    {
       MQ7_GetData();
    }
#endif
#if MQ135_ENABLE == _enable_
    if(Sensor_TimeCnt % MQ135_RUN_SPACE == 0)
    {
       MQ135_GetData();
    }
#endif
#if BODY_HW_ENABLE == _enable_
    if(Sensor_TimeCnt % BODY_HW_RUN_SPACE == 0)
    {
       BODY_HW_GetData();
    }
#endif

#if SR04_ENABLE == _enable_
    if(Sensor_TimeCnt % SR04_RUN_SPACE == 0)
    {
       SR04_GetData();
    }
#endif
#if CCS811_ENABLE == _enable_
    if(Sensor_TimeCnt % CCS811_RUN_SPACE == 0)
    {
       CCS811_GetData();
    }
#endif
#if MFRC522_ENABLE == _enable_
    if(Sensor_TimeCnt % MFRC522_RUN_SPACE == 0)
    {
       MFRC522_GetData();
    }
#endif
#if GATE_GUARD_ENABLE == _enable_
    if(Sensor_TimeCnt % GATE_GUARD_RUN_SPACE == 0)
    {
       GATE_GUARD_GetData();
    }
#endif
#if BUTTON44_ENABLE == _enable_
    if(Sensor_TimeCnt % BUTTON44_RUN_SPACE == 0)
    {
       BUTTON44_GetData();
    }
#endif
#if FSR402_ENABLE == _enable_
    if(Sensor_TimeCnt % FSR402_RUN_SPACE == 0)
    {
       FSR402_GetData();
    }
#endif
#if SHT30_ENABLE == _enable_
    if(Sensor_TimeCnt % SHT30_RUN_SPACE == 0)
    {
       SHT30_GetData();
    }
#endif
#if PM25_ENABLE == _enable_
    if(Sensor_TimeCnt % PM25_RUN_SPACE == 0)
    {
       PM25_GetData();
    }
#endif
#if HX711_ENABLE == _enable_
    if(Sensor_TimeCnt % HX711_RUN_SPACE == 0)
    {
       HX711_GetData();
    }
#endif
#if JW01_ENABLE == _enable_
    if(Sensor_TimeCnt % JW01_RUN_SPACE == 0)
    {
       JW01_GetData();
    }
#endif
#if WATER_ENABLE == _enable_
    if(Sensor_TimeCnt % WATER_RUN_SPACE == 0)
    {
       WATER_GetData();
    }
#endif
#if RAIN_ENABLE == _enable_
    if(Sensor_TimeCnt % RAIN_RUN_SPACE == 0)
    {
       RAIN_GetData();
    }
#endif
#if TDS_ENABLE == _enable_
    if(Sensor_TimeCnt % TDS_RUN_SPACE == 0)
    {
       TDS_GetData();
    }
#endif
#if TS300B_ENABLE == _enable_
    if(Sensor_TimeCnt % TS300B_RUN_SPACE == 0)
    {
       TS300B_GetData();
    }
#endif
#if JW013_ENABLE == _enable_
    if(Sensor_TimeCnt % JW013_RUN_SPACE == 0)
    {
       JW013_GetData();
    }
#endif
#if AS608_ENABLE == _enable_
    if(Sensor_TimeCnt % AS608_RUN_SPACE == 0)
    {
        if(AS608_Ctrler.PressFR_Flag == 1)      /* 刷指纹 */
        {
            if(AS608_Ctrler.Press_Flag != 1)    /* 执行成功后暂停执行，等待标志位清零 */
            {
                AS608_Ctrler.Press_Flag = AS608_Ctrler.PressFR();   /* 刷指纹 - 返回检测结果 */
            }
        }
        else if(AS608_Ctrler.AddFR_Flag == 1)   /* 添加指纹 */
        {
            if(AS608_Ctrler.Add_Flag != 1)      /* 执行成功后暂停执行，等待标志位清零 */
            {
                AS608_Ctrler.Add_Flag = AS608_Ctrler.AddFR();
            }
        }
        else if(AS608_Ctrler.DeleteFR_Flag == 1)/* 删除指纹 */
        {
            if(AS608_Ctrler.Delete_Flag != 1)   /* 执行成功后暂停执行，等待标志位清零 */
            {
                AS608_Ctrler.Delete_Flag = AS608_Ctrler.DeleteFR(0xFFFF);
            }
        }
    }
#endif
#if MPU6050_ENABLE == _enable_
    if(Sensor_TimeCnt % MPU6050_RUN_SPACE == 0)
    {
        MPU6050_GetData();
    }
#endif
#if MLX90614_ENABLE == _enable_
    if(Sensor_TimeCnt % MLX90614_RUN_SPACE == 0)
    {
//        MLX90614_Ctrler.Temp = MLX90614_Ctrler.GetTemp(HUMAN_MODE); /* 获取人体温度 */
//        MLX90614_Ctrler.Temp = MLX90614_Ctrler.GetTemp(OBJECT_MODE);/* 获取物体温度 */
    }
#endif
#if INA226_ENABLE == _enable_
    if(Sensor_TimeCnt % INA226_RUN_SPACE == 0)
    {
        INA226_GetData();
    }
#endif
#if GPS_ENABLE == _enable_
    if(Sensor_TimeCnt % GPS_RUN_SPACE == 0)
    {
        GPS_GetData();
    }
#endif
#if SW420_ENABLE == _enable_
    if(Sensor_TimeCnt % SW420_RUN_SPACE == 0 || sw420_run_state == 1)
    {
        SW420_GetData();
    }
#endif
#if D01_ENABLE == _enable_
    if(Sensor_TimeCnt % D01_RUN_SPACE == 0)
    {
        D01_GetData();
    }
#endif
#if SGP30_ENABLE == _enable_
    if(Sensor_TimeCnt % SGP30_RUN_SPACE == 0)
    {
        SGP30_GetData();
    }
#endif
#if MS1100_ENABLE == _enable_
    if(Sensor_TimeCnt % MS1100_RUN_SPACE == 0)
    {
        MS1100_GetData();
    }
#endif
#if COUNT_ENABLE == _enable_
    if(Sensor_TimeCnt % COUNT_RUN_SPACE == 0)
    {
        Count_GetData();
    }
#endif
    Sensor_TimeCnt++;
}
/******************************* 感器数据获取 结束 ************************************/
