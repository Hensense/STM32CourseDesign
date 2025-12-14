#ifndef __SENSOR_TASK_H
#define __SENSOR_TASK_H

/* ***************************头文件引入 开始******************************* */
#include "main.h"
/* ***************************头文件引入 结束******************************* */

/* ***************************宏定义 开始*********************************** */
/* _enable_:使能某个传感器 _disable_:失能某个传感器 */
#define GATE_GUARD_ENABLE   _disable_       /* MC38门磁模块：IO输入，任意引脚即可 */
#define HW_ENABLE           _disable_       /* TRCT5000红外传感器：IO输入，任意引脚即可 */
#define BODY_HW_ENABLE      _disable_       /* HC-SR505人体红外传感器：IO输入，任意IO脚即可 */
#define SW420_ENABLE        _disable_       /* SW420震动传感器：IO输入，任意IO脚即可 */
#define COUNT_ENABLE        _disable_       /* 红外计次传感器：IO输入，任意IO脚即可 */

#define DHT11_ENABLE        _disable_       /* 温湿度传感器：单总线，仅需一个IO引脚，可任意使用空闲IO引脚 */
#define DS18B20_ENABLE      _disable_       /* 温度传感器：单总线，仅需一个IO引脚，可任意使用空闲IO引脚 */

#define LDR_ENABLE          _enable_       /* 光敏传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define IR_ENABLE           _disable_       /* 火焰传感器：ADC采集/数字采集，需要使用带有ADC通道的引脚或普通IO引脚 */
#define TS_ENABLE           _disable_       /* 土壤湿度传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define XINLV_ENABLE        _disable_       /* 心率传感器：ADC采集，需要使用带有ADC2通道的引脚和TIM3 */
#define MQ2_ENABLE          _disable_       /* 烟雾浓度传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define MQ3_ENABLE          _disable_       /* 酒精浓度传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define MQ4_ENABLE          _disable_       /* 甲烷浓度传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define MQ7_ENABLE          _disable_       /* 一氧化碳传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define MQ135_ENABLE        _disable_       /* 空气质量传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define FSR402_ENABLE       _disable_       /* 薄膜压力传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define PM25_ENABLE         _disable_       /* GP2Y1010AU0F粉尘浓度传感器：ADC采集，需要使用带有ADC通道的引脚和一个IO引脚 */
#define WATER_ENABLE        _disable_       /* 水位传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define RAIN_ENABLE         _disable_       /* 雨滴传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define TDS_ENABLE          _disable_       /* 水质检测传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define TS300B_ENABLE       _disable_       /* 浊度传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define PH_ENABLE           _disable_       /* PH传感器：ADC采集，需要使用带有ADC通道的引脚 */
#define MS1100_ENABLE       _disable_       /* MS1100甲醛传感器：ADC采集，需要使用带有ADC通道的引脚 */

#define BH1750_ENABLE       _disable_       /* 光照强度传感器：IIC通信，需要两个IO引脚，可任意使用空闲IO引脚 */
#define ADXL345_ENABLE      _disable_       /* 加速度传感器：IIC通信，需要两个IO引脚，可任意使用空闲IO引脚 */
#define MAX30102_ENABLE     _disable_       /* 心率血氧传感器：IIC通信，需要两个IO引脚和一个输入引脚，可任意使用空闲的三个IO引脚 */
#define BMP280_ENABLE       _disable_       /* 气压传感器：IIC通信，需要两个IO引脚，可任意使用空闲IO引脚 */
#define CCS811_ENABLE       _disable_       /* 二氧化碳传感器：IIC通信，WAK脚低电平唤醒，需要三个任意IO口 */
#define MPU6050_ENABLE      _disable_       /* MPU6050六轴姿态传感器：IIC通信，需要任意两个IO引脚 */
#define MLX90614_ENABLE     _disable_       /* MLX90614红外测温传感器：IIC通信，需要任意两个IO引脚 */
#define INA226_ENABLE       _disable_       /* INA226功率监控模块：IIC通信，需要任意两个IO引脚 */
#define SHT30_ENABLE        _disable_       /* SHT30温湿度传感器（测量精度，范围更大）：IIC通信，需要两个IO引脚，可任意使用空闲IO引脚 */
#define SGP30_ENABLE        _disable_       /* SGP30气体传感器（空气质量甲醛二氧化碳）模块：IIC通信，需要两个IO引脚，可任意使用空闲IO引脚 */

#define JW01_ENABLE         _disable_       /* JW01二氧化碳传感器：USART3串口通信，可切换为USART1/2 */
#define JW013_ENABLE        _disable_       /* JW013三合一传感器 甲醛模块：USART3串口通信，可切换为USART1/2 */
#define AS608_ENABLE        _disable_       /* AS608指纹模块：USART3串口通信，可切换为USART1/2，需要一个检测引脚，一个TIM */
#define GPS_ENABLE          _disable_       /* VK2828U7G5LF GPS定位模块：USART3串口通信，可切换为USART1/2 */
#define D01_ENABLE          _disable_       /* D01粉尘浓度传感器模块：USART3串口通信，可切换为USART1/2 */

#define HX711_ENABLE        _disable_       /* 电子称传感器：需要任意两个IO引脚 */
#define SR04_ENABLE         _disable_       /* 超声波测距传感器：需要一个IO引脚和一个带有输入捕获通道的IO引脚 */
#define MFRC522_ENABLE      _enable_       /* RC522读卡器：SPI通信+RST引脚，需要五个任意IO引脚 */
#define BUTTON44_ENABLE     _disable_       /* 按键矩阵4*4：需要八个任意IO引脚 */
/* ***************************宏定义 结束*********************************** */

/* ***************************预编译 开始*********************************** */
/* 注意：标志类变量使用后需要清除 部分根据应用场景可以不用清除 */
#if DHT11_ENABLE == _enable_
#include    "dht11.h"
#define     DHT11_RUN_SPACE         1000    /* DHT11温湿度数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      DHT11_Data_TypeDef DHT11_Data;
#endif

#if BH1750_ENABLE == _enable_
#include    "BH1750.h"
#define     BH1750_RUN_SPACE        50      /* BH1750光照强度数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      uint16_t Light_Value;
#endif

#if LDR_ENABLE == _enable_
#include    "LDR.h"
#define     LDR_RUN_SPACE           100     /* LDR光敏传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      uint16_t LDR_Lux;               /* 获取到的光照强度，供外界使用 */
#endif

#if IR_ENABLE == _enable_
#include    "IR.h"
#define     IR_RUN_SPACE            350    /* IR传感器火焰数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      uint16_t IR_Flag;              /* 获取到的火焰标志/AD采样值，供外界使用 */
#endif

#if HW_ENABLE == _enable_
#include    "HW.h"
#define     HW_RUN_SPACE            20     /* 光电红外传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      uint8_t* HW_State;             /* 物体遮挡状态数组，供外界使用 */
#endif

#if TS_ENABLE == _enable_
#include    "TS.h"
#define     TS_RUN_SPACE            500     /* 土壤湿度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      uint16_t TS_Value;              /* 土壤湿度采集值，供外界使用 */
#endif

#if DS18B20_ENABLE == _enable_
#include    "ds18b20.h"
#define     DS18B20_RUN_SPACE       1000    /* 温度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern      float DS18B20_Value;            /* 温度采集值，供外界使用 */
#endif

#if ADXL345_ENABLE == _enable_
#include    "adxl345.h"
#define     ADXL345_RUN_SPACE       25      /* 加速度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
typedef struct{
    float x;
    float y;
    float z;
}AdxAngle;  /* 角度结构体 */
extern AdxAngle adxAngle;                   /* 角度采集值，供外界使用 */
#endif

#if MAX30102_ENABLE == _enable_
#include    "max30102.h"
#define     MAX30102_RUN_SPACE      100     /* 心率血氧传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
typedef struct{
    uint8_t xl;         /* 心率值 */
    uint8_t xy;         /* 血氧值 */
    uint8_t int_flag;   /* 中断标志 使用后需清除 */
    uint8_t pre_time;   /* 上次数据更新距离当前时间 */
}MaxXlXy;  
extern MaxXlXy maxXlXy;                     /* 心率血氧采集值，供外界使用 */
#endif

#if XINLV_ENABLE == _enable_
#include    "XINLV.h"
#define     XINLV_RUN_SPACE         100     /* 心率传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    uint8_t XINLV_EcgShow;                  /* 心电图显示标志位 当此位置为1时，不允许调用OLED操作函数，否则会影响心电图显示 */
    int XINLV_Value;                        /* 采集心率值，供外界使用 */
}XinLv_DataStruct;
extern XinLv_DataStruct XinLv_DataInfo;     /* 心率数据值和心电图显示控制标志位 */
#endif

#if BMP280_ENABLE == _enable_
#include    "bmp280.h"
#define     BMP280_RUN_SPACE        500     /* 气压传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
typedef struct{
    float pressure;     /* 气压值 */
    float temperature;  /* 环境温度值 */
    float asl;          /* 海拔高度 */
}Bmp280Data;  
extern Bmp280Data bmp280Data;               /* BMP280采集数据，供外界使用 */
#endif

#if MQ2_ENABLE == _enable_
#include    "mq2.h"
#define     MQ2_RUN_SPACE           500     /* 烟雾浓度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float MQ2_Value;                     /* 烟雾浓度(ppm)，供外界使用 */
#endif
#if MQ3_ENABLE == _enable_
#include    "mq3.h"
#define     MQ3_RUN_SPACE           500     /* 酒精浓度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float MQ3_Value;                     /* 酒精浓度，供外界使用 */
#endif
#if MQ4_ENABLE == _enable_
#include    "mq4.h"
#define     MQ4_RUN_SPACE           500     /* 甲烷浓度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float MQ4_Value;                     /* 甲烷浓度(ppm)，供外界使用 */
#endif
#if MQ7_ENABLE == _enable_
#include    "mq7.h"
#define     MQ7_RUN_SPACE           500     /* 一氧化碳浓度传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float MQ7_Value;                     /* 一氧化碳浓度(ppm)，供外界使用 */
#endif

#if MQ135_ENABLE == _enable_
#include    "mq135.h"
#define     MQ135_RUN_SPACE         500     /* 空气质量传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float MQ135_Value;                   /* 空气质量(ppm)，供外界使用 */
#endif

#if BODY_HW_ENABLE == _enable_
#include    "body_hw.h"
#define     BODY_HW_RUN_SPACE       200     /* 人体红外传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern uint8_t BodyHw_State;                /* 人体红外检测结果，0x01（有人），0x00（无人） */
#endif

#if SR04_ENABLE == _enable_
#include    "SR04.h"
#define     SR04_RUN_SPACE          300    /* 超声波测距传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern uint32_t SR04_Value;                /* 超声波传感器采集距离值，供外界使用 */
#endif

#if CCS811_ENABLE == _enable_
#include    "ccs811.h"
#define     CCS811_RUN_SPACE        200    /* 二氧化碳传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float CCS811_Value;                 /* 二氧化碳传感器采集数据值（ppm），供外界使用 */
#endif

#if MFRC522_ENABLE == _enable_
#include    "MFRC522.h"
#define     MFRC522_SAVE_IDMAX      10     /* 记录存放最大的ID卡数 */
#define     MFRC522_RUN_SPACE       200    /* MFRC522读卡器读卡采集间隔 时间单位为Sensor_Task执行间隔 */
typedef struct{
    uint8_t Read_Flag;                     /* 读卡标志位 使用后需要清除 第一次读到卡：0x01， 连续读到卡：0x02，没有读到卡：0x03*/
    uint8_t Read_Id[4];                    /* 存放读到卡的ID */
    uint8_t Save_IdNumber;                 /* 当前保存ID的数目 */
    uint8_t Save_IdArray[MFRC522_SAVE_IDMAX][4];/* 保存ID号的数组 最多保存MFRC522_SAVE_IDMAX个ID号 */
    uint8_t (*AddCard)(uint8_t*);          /* 存储ID卡号 */
    uint8_t (*DeleteCard)(uint8_t*);       /* 删除ID卡号 */
    void (*DeleteAllCard)(void);           /* 删除所有ID卡号 */
}MFRC522_Data;
extern MFRC522_Data MFRC522_Value;         /* MFRC522读卡信息，供外界使用 */
#endif

#if GATE_GUARD_ENABLE == _enable_
#include    "gate_guard.h"
#define     GATE_GUARD_RUN_SPACE    10    /* 门磁模块扫描间隔 时间单位为Sensor_Task执行间隔 注意这里的时间同样为滤波时间 尽量处于10~20之间 */
extern uint8_t Gate_Guard_Flag;           /* 0x00：关 0x01：开 0x02：保持开 */
#endif
#if BUTTON44_ENABLE == _enable_
#include    "button4_4.h"
#define     BUTTON44_RUN_SPACE      20      /* 按键矩阵间隔 时间单位为Sensor_Task执行间隔 注意这里的时间同样为滤波时间 尽量处于5~20之间 */
extern char BUTTON_KeyFlag;                 /* 返回按键按下标志 */
#endif
#if FSR402_ENABLE == _enable_
#include    "FSR402.h"
#define     FSR402_RUN_SPACE        200   /* 薄膜压力传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern uint16_t FSR402_Value;             /* 传感器采集压力值 */
#endif
#if SHT30_ENABLE == _enable_
#include    "sht30.h"
#define     SHT30_RUN_SPACE         500   /* 温湿度传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
typedef struct
{
    int Temp;       /* 温度：-40~125°C */
    uint16_t Humi;  /* 湿度：0~100% RH */
}SHT30_Data;
extern SHT30_Data SHT30_Value;
#endif
#if PM25_ENABLE == _enable_
#include    "PM25.h"
#define     PM25_RUN_SPACE          500   /* 粉尘浓度传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern uint16_t PM25_Value;               /* PM2.5浓度值，供外界使用 */
#endif
#if HX711_ENABLE == _enable_
#include    "hx711.h"
#define     HX711_RUN_SPACE         100   /* 电子秤传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern float HX711_Value;                 /* 电子秤采集重量值，供外界使用 */
#endif
#if JW01_ENABLE == _enable_
#include    "JW01.h"
#define     JW01_RUN_SPACE          300   /* 二氧化碳传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern uint16_t JW01_Value;               /* 二氧化碳浓度，供外界使用 */
#endif
#if WATER_ENABLE == _enable_
#include    "water.h"
#define     WATER_RUN_SPACE         300   /* 水位传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern uint16_t WATER_Value;              /* 水位数据，供外界使用 */
#endif
#if RAIN_ENABLE == _enable_
#include    "rain.h"
#define     RAIN_RUN_SPACE          300   /* 水位传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern uint16_t RAIN_Value;               /* 水位数据，供外界使用 */
#endif
#if TDS_ENABLE == _enable_
#include    "tds.h"
#define     TDS_RUN_SPACE           200   /* 水质传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern float TDS_Value;                   /* 水质数据，供外界使用 */
#endif
#if TS300B_ENABLE == _enable_
#include    "ts_300b.h"
#define     TS300B_RUN_SPACE        200   /* 浊度传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern float TS300B_Value;                /* 水质浊度数据，供外界使用 */
#endif
#if PH_ENABLE == _enable_
#include    "ph.h"
#define     PH_RUN_SPACE            200   /* PH传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern float PH_Value;                    /* PH数据，供外界使用 */
#endif
#if JW013_ENABLE == _enable_
#include    "JW013.h"
#define     JW013_RUN_SPACE         300   /* 甲醛传感器数据采集间隔 时间单位为Sensor_Task执行间隔  */
extern float JW013_Value;                 /* 甲醛浓度，供外界使用 */
#endif
#if AS608_ENABLE == _enable_
#include    "as608.h"
#define     AS608_RUN_SPACE         150   /* 指纹模块数据采集间隔 时间单位为Sensor_Task执行间隔  */
typedef struct{
    uint8_t Press_Flag;                     /* 查询指纹成功标志，使用后需要清除 0x01：代表查询到指纹 0x02：查询指纹失败 0x03 没有指纹按下 */
    uint8_t Add_Flag;                       /* 添加指纹成功标志位 */
    uint8_t Delete_Flag;                    /* 删除指纹成功标志位 */
    uint8_t PressFR_Flag;                   /* 刷指纹标志位 */
    uint8_t AddFR_Flag;                     /* 添加指纹标志位 */
    uint8_t DeleteFR_Flag;                  /* 删除指纹标志位 */
    uint8_t (*AddFR)(void);
    uint8_t (*PressFR)(void);
    uint8_t (*DeleteFR)(uint16_t);
    uint8_t (*GetNumber)(void);
}AS608_Operations;
extern AS608_Operations AS608_Ctrler;       /* 指纹模块控制结构体，可以执行刷指纹，添加指纹，删除指纹操作 */
#endif
#if MPU6050_ENABLE == _enable_
#include "MPU6050.h"
#define FALL_DEC_FLAG               0       /* 是否使能跌倒检测 0 不使用 1 使用 */
#define FALL_DOWN_LIMIT             1000    /* 跌倒检测阈值 */
#define MPU6050_RUN_SPACE           100     /* MPU6050六轴姿态传感器执行间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
    int16_t LastAccX;               /* 上次的X轴加速度值 */
    int16_t LastAccY;               /* 上次的Y轴加速度值 */
#if FALL_DEC_FLAG
    uint8_t Fall_Down_Flag;         /* 跌倒标志 */
#endif
}MPU6050_DataStruct;
extern MPU6050_DataStruct MPU6050_DataInfo; /* 姿态数据 提供给外界使用 */
#endif
#if MLX90614_ENABLE == _enable_
#include "mlx90614.h"
#define MLX90614_RUN_SPACE          500     /* 红外测温执行间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    float Temp;                             /* 红外测温检测温度值 */
    float (*GetTemp)(TempDetect_Mode);
}MLX90614_Operations;
extern MLX90614_Operations MLX90614_Ctrler; 
#endif
#if INA226_ENABLE == _enable_
#include "ina226.h"
#define INA226_RUN_SPACE            200     /* 电压、电流、功率获取间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    float Bus_Voltage;                      /* 总电压 */
    float Shunt_Voltage;                    /* 分流电阻两端的电压差 */
    float Current;                          /* 测量电流 */
    float Power;                            /* 测量功率 */
}INA226_DataStruct;
extern INA226_DataStruct INA226_DataInfo;   /* 电压、电流、功率信息 */
#endif
#if GPS_ENABLE == _enable_                  
#include "GPS.h"
#define GPS_RUN_SPACE               150     /* 经度，维度获取间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    float Latitude;                         /* 经度 */
    float Longitude;                        /* 维度 */
}GPS_DataStruct;
extern GPS_DataStruct GPS_DataInfo;
#endif
#if SW420_ENABLE == _enable_
#include    "sw420.h"
#define     SW420_RUN_SPACE         1       /* 震动传感器数据采集间隔 时间单位为Sensor_Task执行间隔 */
extern float SW420_Flag;                    /* 检测结果，0x01（震动），0x02（未震动），0x00（状态保持不变） */
#endif
#if D01_ENABLE == _enable_
#include    "D01.h"
#define     D01_RUN_SPACE           500     /* 粉尘浓度传感器数据获取间隔 时间单位为Sensor_Task执行间隔 */
extern float D01_Value;                     /* 粉尘浓度值 */
#endif
#if SGP30_ENABLE == _enable_
#include    "SGP30.h"
#define     SGP30_RUN_SPACE         200     /* SGP30气体传感器数据获取间隔 时间单位为Sensor_Task执行间隔 */
typedef struct
{
    uint16_t Co2;       /* 二氧化碳浓度 */
    uint16_t Tvoc;      /* 总挥发性有机化合物浓度 */
}SGP30_DataStruct;
extern SGP30_DataStruct SGP30_DataInfo;
#endif
#if MS1100_ENABLE == _enable_
#include    "ms1100.h"
#define     MS1100_RUN_SPACE        200     /* 甲醛传感器数据获取间隔 时间单位为Sensor_Task执行间隔 */
extern uint16_t MS1100_Value;               /* 甲醛ADC采样值 - 未转换 */
#endif
#if COUNT_ENABLE == _enable_
#include    "CountSensor.h"
#define     COUNT_RUN_SPACE        50       /* 计次传感器数据获取间隔 时间单位为Sensor_Task执行间隔 */
extern uint16_t Count_Value;                /* 计次计数值 */
#endif
/* ***************************预编译 结束********************************** */

/* ***************************枚举类型定义 开始***************************** */

/* ***************************枚举类型定义 结束***************************** */

/* ***************************共用体类型定义 开始*************************** */

/* ***************************共用体类型定义 结束*************************** */

/* ***************************结构体类型定义 开始*************************** */

/* ***************************结构体类型定义 结束*************************** */

/* ***************************外部变量声明 开始***************************** */

/* ***************************外部变量声明 结束***************************** */

/* ***************************函数接口声明 开始***************************** */
void Sensor_Init(void);     /* 传感器底层接口初始化 */
void Sensor_Execute(void);  /* 传感器数据获取任务 */
/* ***************************函数接口声明 结束***************************** */

#endif
