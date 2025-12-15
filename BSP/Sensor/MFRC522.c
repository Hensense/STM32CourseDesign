#include "MFRC522.h"
#if MFRC522_START_STATE
#define     MAXRLEN 18
#define     RC522_DELAY()  delay_us(2)


void MFRC522_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    /* 配置 SPI_RC522_SPI 引脚：SDA */
    GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MFRC522_GPIO_SDA_PORT, &GPIO_InitStructure);

    /* 配置 SPI_RC522_SPI 引脚：SCK */
    GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MFRC522_GPIO_SCK_PORT, &GPIO_InitStructure);

    /* 配置 SPI_RC522_SPI 引脚：MOSI */
    GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MFRC522_GPIO_MOSI_PORT, &GPIO_InitStructure);

    /* 配置 SPI_RC522_SPI 引脚：MISO */
    GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_MISO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MFRC522_GPIO_MISO_PORT, &GPIO_InitStructure);

    /* 配置 SPI_RC522_SPI 引脚：RST */
    GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MFRC522_GPIO_RST_PORT, &GPIO_InitStructure);
    
}
    

/////////////////////////////////////////////////////////////////////
//功能描述?〈覯FRC522的某一寄存器读一个字节数据
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char Read_MFRC522(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MFRC522_SCK_L;
         RC522_DELAY();
     MFRC522_SDA_L;  //选中，即片选
         RC522_DELAY();
     ucAddr = ((Address<<1)&0x7E)|0x80; //寄存器地址左移 1 位，只保留 bit6~bit1，把 bit7 置 1

     for(i=8;i>0;i--) //发送读寄存器地址字节
     {
             
         if(ucAddr&0x80)  //检查最高位是1还是0
                 {
                        MFRC522_MOSI_H;
                 }
                 else
                 {
                        MFRC522_MOSI_L;
                 }
                 RC522_DELAY();
         MFRC522_SCK_H;   //拉高时钟，这一位有效
                 RC522_DELAY();
         ucAddr <<= 1;    //从高到低地去发送下一位
         MFRC522_SCK_L;   //时钟拉低，发送结束
                 RC522_DELAY();
     }

     for(i=8;i>0;i--)  //把数据字节读回来
     {
         MFRC522_SCK_H; //拉高时钟
             RC522_DELAY();
         ucResult <<= 1; //给结果腾出一位位置
         ucResult|= MFRC522_MISO_READ;  //读取MISO当前电平，并拼接到res最低位
         MFRC522_SCK_L;  //时钟拉低，读取结束
             RC522_DELAY();
     }

     MFRC522_SCK_H;  //收尾稳定总线
         RC522_DELAY();
     MFRC522_SDA_H;  //片选结束，结束传输
         RC522_DELAY();
     return ucResult;
}



/////////////////////////////////////////////////////////////////////
//功能描述?∠騇FRC522的某一寄存器写一个字节数据
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void Write_MFRC522(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

    MFRC522_SCK_L;
    MFRC522_SDA_L;
    ucAddr = ((Address<<1)&0x7E); //寄存器地址左移 1 位，只保留 bit6~bit1，bit7此时为0

    for(i=8;i>0;i--) //发送地址控制字节
    {
        if(ucAddr&0x80)  //取最高位
        {
            MFRC522_MOSI_H;
        }
        else
        {
            MFRC522_MOSI_L;
        }
        RC522_DELAY();
        MFRC522_SCK_H; //拉高时钟，此位有效
        RC522_DELAY();
        ucAddr <<= 1; //准备下一位
        MFRC522_SCK_L; //结束发送
        RC522_DELAY();
    }

    for(i=8;i>0;i--)
    {
        if(value&0x80) //每次取value的最高位
        {
            MFRC522_MOSI_H;
        }
        else
        {
            MFRC522_MOSI_L;
        }
        RC522_DELAY();
        MFRC522_SCK_H; //时钟拉高，此位有效
        RC522_DELAY();
        value <<= 1;  //准备下一位
        MFRC522_SCK_L; //结束发送
        RC522_DELAY();
    }

    MFRC522_SCK_H; //收尾稳定总线
    RC522_DELAY();
    MFRC522_SDA_H; //片选结束
    RC522_DELAY();
}


/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Reset(void) 
{
    uint8_t retry = 10;                  /* 软复位后等待芯片就绪的重试次数 */
    //unsigned char i;
    MFRC522_RST_H;                       /* RST先拉高：保持非复位状态 */
    delay_us (1);                        /* 稍作延时，保证电平稳定 */
    MFRC522_RST_L;                       /* RST拉低：触发硬复位脉冲 */
    delay_us (1);                        /* 保持一小段时间 */
    MFRC522_RST_H;                       /* RST再拉高：结束硬复位 */
    delay_us (1);                        /* 让芯片从复位状态恢复 */

    //MFRC522_RST_H;
    Write_MFRC522(CommandReg,0x0F);       /* Soft Reset：写CommandReg=0x0F */
    while((Read_MFRC522(CommandReg) & 0x10) && (retry--)); /* 等待PowerDown位清零，表示芯片启动完成 */

    delay_us (1);                        /* 再延时一点，避免紧接着配置寄存器 */


    Write_MFRC522(ModeReg,0x3D);            /* 通讯模式设置：CRC初始值0x6363（Mifare卡要求） */
    Write_MFRC522(TReloadRegL,30);          /* 定时器重装载值低字节 */
    Write_MFRC522(TReloadRegH,0);           /* 定时器重装载值高字节 */
    Write_MFRC522(TModeReg,0x8D);           /* 定时器模式配置 */
    Write_MFRC522(TPrescalerReg,0x3E);      /* 定时器分频配置 */
    Write_MFRC522(TxAutoReg,0x40);          /* 自动发射/调制相关配置（常见默认值） */
    return MI_OK;
}


/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg,tmp | mask);  // set bit mask
}



/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & ~mask);  // clear bit mask
} 


/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char MFRC522_ToCard(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    Write_MFRC522(ComIEnReg,irqEn|0x80);    //PCD_TRANSCEIVE模式 无命令改变
    ClearBitMask(ComIrqReg,0x80);            //IRQ开漏输出
    Write_MFRC522(CommandReg,PCD_IDLE);  //取消当前命令
    SetBitMask(FIFOLevelReg,0x80);        //清除FIFO Flash 内ErrReg  BufferOvfl标志
    
    for (i=0; i<InLenByte; i++)
    {   
                Write_MFRC522(FIFODataReg, pInData[i]);    //把数据存到FIFO
        }
    Write_MFRC522(CommandReg, Command);   //发送FIFO内容
   
    
    if (Command == PCD_TRANSCEIVE)
    {    
                SetBitMask(BitFramingReg,0x80);  //立即停止定时器
        }
    
        n = Read_MFRC522(ComIrqReg);
    i = 1500;//根据时钟频率调整，操作M1卡最大等待时间25ms
    do 
    {
         n = Read_MFRC522(ComIrqReg);
            
         i--;
            
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
          
    if (i!=0)
    {    
         if(!(Read_MFRC522(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
                   n = Read_MFRC522(FIFOLevelReg);
                  lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = Read_MFRC522(FIFODataReg);    }
            }
         }
         else
         {   
                    status = MI_ERR;   
                 }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   Write_MFRC522(CommandReg,PCD_IDLE); 
   return status;
}



//开启天线  
//每次启动或关闭天线发射之间应至少有1ms的间隔
void MFRC522_AntennaOn(void)
{
    unsigned char i;
    i = Read_MFRC522(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


//关闭天线
void MFRC522_AntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}


//功能描述?⒂肕F522计算CRC
//输入参数??pIndata--要读数CRC的数据??len--数据长度??pOutData--计算的CRC结果
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    Write_MFRC522(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   Write_MFRC522(FIFODataReg, *(pIndata+i));   }
    Write_MFRC522(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}


/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Halt(void)  //暂未使用
{
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}




/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Request(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   Write_MFRC522(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;  

     }
   
   return status;
}



//功    能：防冲突检测?《寥⊙≈锌ㄆ?的卡序列号
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK 
char MFRC522_Anticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    Write_MFRC522(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
         for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];

         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_SelectTag(unsigned char *pSnr)  //暂未使用
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
        ucComMF522Buf[i+2] = *(pSnr+i);
        ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码  暂未使用
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char MFRC522_AuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = MFRC522_ToCard(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char MFRC522_Read(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
char MFRC522_Write(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}
/**
 * @brief   读卡函数，当读到卡后，只会返回一次卡ID，拿开卡后，再次把卡放到读卡器上，才会再次返回卡号
 * @param   Card_Id :
 * @retval  第一次读到卡：0x01， 连续读到卡：0x02，没有读到卡：0x03
 */
uint8_t MFRC522_ReadCard(uint8_t* Card_Id)
{
    static uint8_t pre_Status = 0;       // 存储上一次的读卡状态
    static uint8_t lastCardId[4] = {0};  // 存储上一次读到的卡号
    uint8_t status;
    uint8_t buf[20];            /* 存放卡序号 */
    uint8_t read_number = 3;    /* 读卡次数 */
    uint8_t read_id = 0;        /* 读到卡标志 */

    while(read_number--)
    {
        status = MFRC522_Request(PICC_REQALL, buf); // 寻卡
        if (status != MI_OK)
        {    
            MFRC522_Reset();
            MFRC522_AntennaOff(); 
            MFRC522_AntennaOn(); 
//            delay_ms(10);       // 增加短暂延迟
            continue;
        }

        status = MFRC522_Anticoll(buf); // 防冲突
        if (status != MI_OK)
        {    
//            delay_ms(10);       // 增加短暂延迟
            continue;
        }

        // 检查是否为新卡（与上一次卡号不同）
        uint8_t isNewCard = memcmp(buf, lastCardId, 4) != 0;//如果str1和str2的前n个字节完全相同，则返回0

        if (isNewCard || pre_Status == 0)
        {
            // 存储卡号
            for(uint8_t i = 0; i < 4; i++)
            {
                Card_Id[i] = buf[i];
                lastCardId[i] = buf[i]; // 更新最后一次卡号
            }
            pre_Status = 1;
            read_id = 1;
            break;
        }
        else
        {
            // 同一张卡持续存在
            read_id = 1;
            pre_Status = 2;
            break;
        }
    }

    if(read_id != 0)
    {
        if(pre_Status == 1)
        {
            return 0x01; /* 第一次读到卡 */
        }
        else
        {
            return 0x02; /* 重复读卡 */
        }
    }
    else
    {
        pre_Status = 0;
        return 0x03;    /* 没有读到卡 */
    }
}
/*********************************** 卡号操作函数 ******************************************/
#include "Sensor_Task.h"
/**
 * @brief   添加ID卡号
 * @param   Card_Id : 需要添加的ID卡号
 * @retval  0x00 存入成功 0x01 当前ID号已经存过 0x02 无剩余空间，存储失败
 */
uint8_t MFRC522_AddCard(uint8_t* Card_Id)
{
    uint8_t query = 0x00;   /* 查询是否已经存入ID数组 */
    /* 1.查询是否已经存入 */
    for(uint8_t i = 0; i < MFRC522_Value.Save_IdNumber; i++)
    {
        query = memcmp(MFRC522_Value.Save_IdArray[i], Card_Id, 4);
        if(query == 0)
        {
            return 0x01;    /* 标记ID已经存在 */
        }
    }
    /* 2.查询是否有剩余空间 */
    if(MFRC522_Value.Save_IdNumber >= MFRC522_SAVE_IDMAX)
    {
        return 0x02;        /* ID空间已经存满 */
    }
    /* 3.将ID卡号存入剩余空间 */
    for(uint8_t i = 0; i < 4; i++)
    {
        MFRC522_Value.Save_IdArray[MFRC522_Value.Save_IdNumber][i] = Card_Id[i];
    }
    MFRC522_Value.Save_IdNumber++;
    return 0x00;
}
/**
 * @brief   删除ID卡号
 * @param   Card_Id : 需要删除的ID卡号
 * @retval  0x00 删除成功 0x01 没有在数组中查询到该ID，删除失败 0x02 数组中无ID
 */
uint8_t MFRC522_DeleteCard(uint8_t* Card_Id)
{
    uint8_t query = 0x00;   /* 查询是否已经存入ID数组 */
    /* 1.查询当前ID数是否为0 */
    if(MFRC522_Value.Save_IdNumber == 0)
    {
        return 0x02;
    }
    /* 2.删除ID 挪动数组 */
    for(uint8_t i = 0; i < MFRC522_Value.Save_IdNumber; i++)
    {
        query = memcmp(MFRC522_Value.Save_IdArray[i], Card_Id, 4);
        if(query == 0)
        {
            /* 找到ID 删除 */
            memset(MFRC522_Value.Save_IdArray[i],0,4);
            if(MFRC522_Value.Save_IdNumber < MFRC522_SAVE_IDMAX)
            {
                /* 挪动后边的数据 */
                memmove(MFRC522_Value.Save_IdArray[i],MFRC522_Value.Save_IdArray[i+1],
                        (MFRC522_Value.Save_IdNumber - (i+1))*4);
                break;
            }
            
        }
    }
    if(query == 0)
    {
        if(MFRC522_Value.Save_IdNumber > 0)
        {
            MFRC522_Value.Save_IdNumber--;
        }
        return 0x00;    /* 删除成功 */
    }
    else
    {
        return 0x02;    /* 没有在数组中找到该ID */
    }
}
/**
 * @brief   清空所有存储的ID号
 * @param   none
 * @retval  none
 */
void MFRC522_DeleteAllCard(void)
{
    MFRC522_Value.Save_IdNumber = 0;
    memset(MFRC522_Value.Save_IdArray,0, sizeof(MFRC522_Value.Save_IdArray));
}
#endif
