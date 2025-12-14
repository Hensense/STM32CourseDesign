#include "xinlv.h"
#include "Sensor_Task.h"
#if XINLV_START_STATE
int BPM;                                // used to hold the pulse rate
int Signal;                            // holds the incoming raw data
int IBI = 600;                         // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
//unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0; // used to determine pulse timing
unsigned long lastBeatTime = 0;  // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded模拟范围的中间
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
unsigned char firstBeat = true;  // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;// used to seed rate array so we startup with reaso

/**
 * @brief   心率采集ADC初始化，这里使用ADC2
 * @param   none
 * @retval  none
 */
void XINLV_ADC_Init(void)
{
    ADC_InitTypeDef   ADC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(XINLV_CLK_ENABLE | RCC_APB2Periph_ADC2, ENABLE);  // 修改为PB0
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
    GPIO_InitStructure.GPIO_Pin = XINLV_GPIO_PIN;  // 选择PB0引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // 模拟输入模式
    GPIO_Init(XINLV_PORT, &GPIO_InitStructure);  // 初始化PB0引脚
    
    ADC_DeInit(ADC2);
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;         // 模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   // 模数转换工作在单次转换模式
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   // 转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; // 选择TIM3作为外部触发源
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;               // 顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC2, &ADC_InitStructure);    
    
    ADC_ExternalTrigConvCmd(ADC2, ENABLE); // 采用外部触发
    ADC_RegularChannelConfig(ADC2, XINLV_ADC_CHANNEL, 1, ADC_SampleTime_1Cycles5); // 修改为PB0对应的通道
    ADC_Cmd(ADC2, ENABLE);
    
    ADC_ResetCalibration(ADC2); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC2)); // 等待校准结束，校准结束状态为RESET
    ADC_StartCalibration(ADC2); // AD校准
    while(ADC_GetCalibrationStatus(ADC2)); // 等待校准结束    
}
/**
 * @brief   心率采集定时器触发初始化，这里使用TIM3作为触发源
 * @param   none
 * @retval  none
 */
void XINLV_TIMx_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 

    TIM_TimeBaseStructure.TIM_Period = arr;      
    TIM_TimeBaseStructure.TIM_Prescaler =psc;        
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    
    TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
    /*选择update event作为TRGO,利用TIM3触发ADC通道 */
    //每个定时周期结束后触发一次
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);                 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 
    /*先关闭等待使用*/ 
}
/**
 * @brief   心率传感器初始化 使用ADC2 TIM3
 * @param   none
 * @retval  none
 */
void XINLV_Init(void)
{
    XINLV_ADC_Init();
    XINLV_TIMx_Init(2000-1,72-1);   /* 定时周期：2ms */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}
/**
 * @brief   返回心率值
 * @param   none
 * @retval  none
 */
int XINLV_GetValue(void)
{
    return BPM;
}
#include "OLED.h"
#define OLED_HIGH       64      /* OLED液晶屏高度 */
#define OLED_WIDTH      128     /* OLED液晶屏宽度 */
#define OLED_ACCURACY   22     /* signal缩放倍数 0~4095 - 0 ~ 64*/
void Show_Sensor(uint16_t signal)
{
    static uint16_t rate_x = 0; /* 显示当前ADC采样值 */
    uint16_t rate_y = OLED_HIGH - signal / OLED_ACCURACY;
    if(rate_y >= OLED_HIGH)
    {
        rate_y = OLED_HIGH - 1;
    }
    if(rate_x < OLED_WIDTH)
    {
        OLED_DrawPoint(rate_x, rate_y);
    }
    else
    {
        rate_x = 0;
        OLED_Clear();
        OLED_Update();
        OLED_DrawPoint(rate_x, rate_y);
    }
    
    OLED_UpdateArea(rate_x, rate_y,1,1);
    OLED_Printf(0, 0, OLED_8X16, "%03d", BPM);
    OLED_UpdateArea(0,0,24,16);
    rate_x++;
}
 /*该源码由开源硬件提供*/
void TIM3_IRQHandler(void)
{
    uint16_t runningTotal=0;
    uint8_t i;
    uint16_t Num;
    static uint8_t update_count = 0;
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
    {
        //读取到的值右移2位，12位-->10位
        Signal = ADC_GetConversionValue(ADC2)/4;     // read the Pulse Senso
        update_count++;
        if(update_count >= 6)
        {
            update_count = 0;
            if(XinLv_DataInfo.XINLV_EcgShow)
            {
                Show_Sensor(Signal);    /* 显示心电图 */
            }
            
        }
        
        //Signal=Get_Adc_Average(ADC_Channel_0,1)>>2;//读取A/D转换数据
        sampleCounter += 2;                          // keep track of the time in mS with this variable
        Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise    

        //发现脉冲波的波峰和波谷
        //  find the peak and trough of the pulse wave
        if(Signal < thresh && Num > (IBI/5)*3)
        {   // avoid dichrotic noise by waiting 3/5 of last IBI
            if (Signal < T)
            {                        // T is the trough
                T = Signal;                           // keep track of lowest point in pulse wave 
            }
        }

        if(Signal > thresh && Signal > P)
        {        // thresh condition helps avoid noise
            P = Signal;                             // P is the peak
        }                                         // keep track of highest point in pulse wave

        //开始寻找心跳
        //当脉冲来临的时候，signal的值会上升
        //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
        // signal surges up in value every time there is a pulse
        if (Num > 200)//最大心率次数300次
        {                                 // avoid high frequency noise
            if( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) )
            {        
                Pulse = true;                               // set the Pulse flag when we think there is a pulse
                IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
                lastBeatTime = sampleCounter;               // keep track of time for next pulse
                //下一个部分是用来确保我们在启动时以一个实际的BPM值开始
                if(secondBeat)
                {                        // if this is the second beat, if secondBeat == TRUE
                    secondBeat = false;                  // clear secondBeat flag
                    for(i=0; i<=9; i++)
                    {                 // seed the running total to get a realisitic BPM at startup
                        rate[i] = IBI;                     
                    }
                }
                if(firstBeat)
                {                         // if it's the first time we found a beat, if firstBeat == TRUE
                    firstBeat = false;                   // clear firstBeat flag
                    secondBeat = true;                   // set the second beat flag
                    return;                              // IBI value is unreliable so discard it
                }   
                // keep a running total of the last 10 IBI values
                // runningTotal = 0;                          // clear the runningTotal variable    
                for(i=0; i<=8; i++)
                {                    // shift data in the rate array
                    rate[i] = rate[i+1];                  // and drop the oldest IBI value 
                    runningTotal += rate[i];              // add up the 9 oldest IBI values
                }

                rate[9] = IBI;                          // add the latest IBI to the rate array
                runningTotal += rate[9];                // add the latest IBI to runningTotal
                runningTotal /= 10;                     // average the last 10 IBI values 
                BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
                //      QS = true;                              // set Quantified Self flag 
                // QS FLAG IS NOT CLEARED INSIDE THIS ISR
            }
        }

        //脉冲开始下降
        if (Signal < thresh && Pulse == true)
        {   // when the values are going down, the beat is over
            Pulse = false;                         // reset the Pulse flag so we can do it again
            amp = P - T;                           // get amplitude of the pulse wave
            thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
            P = thresh;                            // reset these for next time
            T = thresh;
        }

        //没有检测到脉冲，设置默认值
        if (Num > 2500)
        {                         // if 2.5 seconds go by without a beat
            thresh = 512;                          // set thresh default
            P = 512;                               // set P default
            T = 512;                               // set T default
            lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
            firstBeat = true;                      // set these to avoid noise
            secondBeat = false;                    // when we get the heartbeat back
        }

    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
#endif
