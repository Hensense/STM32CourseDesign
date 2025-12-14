#include "as608.h"
#if AS608_START_STATE
//接收状态
//bit15，    接收完成标志
//bit14，    接收到0x0d
//bit13~0，    接收到的有效字节数目
uint16_t AS608_USART_RX_STA=0;       //接收状态标记
/*********************************TIM4********************************************/
void AS608_TIMx_Init(uint16_t arr, uint16_t psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 根据宏定义选择时钟使能
    #if AS608_TIMx == 1
        RCC_APB2PeriphClockCmd(AS608_TIM_RCC, ENABLE);
    #elif AS608_TIMx == 2 || AS608_TIMx == 3 || AS608_TIMx == 4
        RCC_APB1PeriphClockCmd(AS608_TIM_RCC, ENABLE);
    #endif

    // 定时器初始化
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(AS608_TIM, &TIM_TimeBaseStructure);

    TIM_ITConfig(AS608_TIM, TIM_IT_Update, ENABLE);

    // 中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = AS608_TIM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(AS608_TIM, ENABLE);
}

// 定时器中断服务程序
void AS608_TIM_IRQHandler(void) {
    if (TIM_GetITStatus(AS608_TIM, TIM_IT_Update) != RESET) {
        AS608_USART_RX_STA |= 1 << 15;    // 标记接收完成
        TIM_ClearITPendingBit(AS608_TIM, TIM_IT_Update);
        TIM_Cmd(AS608_TIM, DISABLE);  // 关闭定时器
    }
}
/*********************************TIM4********************************************/

/*********************************USART1******************************************/
#define AS608_USART_REC_LEN             400     //定义最大接收字节数 200

uint8_t AS608_USART_RX_BUF[AS608_USART_REC_LEN];     //接收缓冲,最大AS608_USART_REC_LEN个字节.

void AS608_USARTx_Init(uint32_t bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 根据宏定义选择时钟使能
    #if AS608_USARTx == 1
        RCC_APB2PeriphClockCmd(AS608_RCC_APBPeriph | AS608_GPIO_RCC, ENABLE);
    #elif AS608_USARTx == 2 || AS608_USARTx == 3
        RCC_APB2PeriphClockCmd(AS608_GPIO_RCC, ENABLE);
        RCC_APB1PeriphClockCmd(AS608_RCC_APBPeriph, ENABLE);
    #endif

    //USART_TX GPIO初始化
    GPIO_InitStructure.GPIO_Pin = AS608_GPIO_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(AS608_GPIO_TX_PORT, &GPIO_InitStructure);

    //USART_RX GPIO初始化
    GPIO_InitStructure.GPIO_Pin = AS608_GPIO_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(AS608_GPIO_RX_PORT, &GPIO_InitStructure);  

    //USART NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = AS608_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                 //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(AS608_USART_UX, &USART_InitStructure);       //初始化串口
    USART_ITConfig(AS608_USART_UX, USART_IT_RXNE, ENABLE);  //开启串口接受中断
    USART_Cmd(AS608_USART_UX, ENABLE);                      //使能串口
    
    AS608_USART_RX_STA=0;         //清零
}

void AS608_USART_IRQHandler(void)                    //串口1中断服务程序
{
    uint8_t res;        
    if(USART_GetITStatus(AS608_USART_UX, USART_IT_RXNE) != RESET)//接收到数据
    {     
        res =USART_ReceiveData(AS608_USART_UX);        
        if((AS608_USART_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
        {         
            if(AS608_USART_RX_STA<AS608_USART_REC_LEN)        //还可以接收数据
            {          
                TIM_SetCounter(AS608_TIM,0);//计数器清空                  
                if(AS608_USART_RX_STA==0)
                    TIM_Cmd(AS608_TIM, ENABLE);  //使能定时器4 
                AS608_USART_RX_BUF[AS608_USART_RX_STA++]=res;        //记录接收到的值     
            }else 
            {
                AS608_USART_RX_STA|=1<<15;                    //强制标记接收完成
            } 
        }
    }
    USART_ClearITPendingBit(AS608_USART_UX, USART_IT_RXNE);
}
/*********************************USART1******************************************/


uint32_t AS608Addr = 0XFFFFFFFF; //默认
//初始化PA6为下拉输入            
//读摸出感应状态(触摸感应时输出高电平信号)
void AS608_Init(void)
{   
    AS608_USARTx_Init(57600);
    AS608_TIMx_Init(1000-1,7200-1);         //100ms中断
    TIM_Cmd(AS608_TIM, DISABLE);            //关闭定时器
    
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(AS608_PS_GPIO_CLK, ENABLE);    

    //初始化读状态引脚GPIOA
    GPIO_InitStructure.GPIO_Pin = AS608_PS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(AS608_PS_GPIO_PORT, &GPIO_InitStructure);//初始化GPIO    
    /* 读取系统连接参数 */
    AS608_Check();              // AS608参数获取
}
//串口发送一个字节
static void MYUSART_SendData(uint8_t data)
{
    while((AS608_USART_UX->SR&0X40)==0); 
    AS608_USART_UX->DR = data;
}
//发送包头
static void SendHead(void)
{
    MYUSART_SendData(0xEF);
    MYUSART_SendData(0x01);
}
//发送地址
static void SendAddr(void)
{
    MYUSART_SendData(AS608Addr>>24);
    MYUSART_SendData(AS608Addr>>16);
    MYUSART_SendData(AS608Addr>>8);
    MYUSART_SendData(AS608Addr);
}
//发送包标识,
static void SendFlag(uint8_t flag)
{
    MYUSART_SendData(flag);
}
//发送包长度
static void SendLength(int length)
{
    MYUSART_SendData(length>>8);
    MYUSART_SendData(length);
}
//发送指令码
static void Sendcmd(uint8_t cmd)
{
    MYUSART_SendData(cmd);
}
//发送校验和
static void SendCheck(uint16_t check)
{
    MYUSART_SendData(check>>8);
    MYUSART_SendData(check);
}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
static uint8_t *JudgeStr(uint16_t waittime)
{
    char *data;
    uint8_t str[8];
    str[0]=0xef;                    str[1]=0x01;
    str[2]=AS608Addr>>24;    str[3]=AS608Addr>>16;        
    str[4]=AS608Addr>>8;    str[5]=AS608Addr;                
    str[6]=0x07;                    str[7]='\0';
    AS608_USART_RX_STA=0;
    while(--waittime)
    {
        delay_ms(1);
        if(AS608_USART_RX_STA&0X8000)//接收到一次数据
        {
            AS608_USART_RX_STA=0;
            data=strstr((const char*)AS608_USART_RX_BUF,(const char*)str);
            if(data)
                return (uint8_t*)data;    
        }
    }
    return 0;
}
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。 
//模块返回确认字
uint8_t PS_GetImage(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x01);
    temp =  0x01+0x03+0x01;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2             
//参数:BufferID --> charBuffer1:0x01    charBuffer1:0x02                                                
//模块返回确认字
uint8_t PS_GenChar(uint8_t BufferID)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x04);
    Sendcmd(0x02);
    MYUSART_SendData(BufferID);
    temp = 0x01+0x04+0x02+BufferID;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
uint8_t PS_Match(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x03);
    temp = 0x01+0x03+0x03;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。            
//参数:  BufferID @ref CharBuffer1    CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
uint8_t PS_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x08);
    Sendcmd(0x04);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(StartPage>>8);
    MYUSART_SendData(StartPage);
    MYUSART_SendData(PageNum>>8);
    MYUSART_SendData(PageNum);
    temp = 0x01+0x08+0x04+BufferID
    +(StartPage>>8)+(uint8_t)StartPage
    +(PageNum>>8)+(uint8_t)PageNum;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
    {
        ensure = data[9];
        p->pageID   =(data[10]<<8)+data[11];
        p->mathscore=(data[12]<<8)+data[13];    
    }
    else
        ensure = 0xff;
    return ensure;    
}
//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2    
//说明:  模块返回确认字
uint8_t PS_RegModel(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x05);
    temp = 0x01+0x03+0x05;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;        
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。            
//参数:  BufferID @ref charBuffer1:0x01    charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x06);
    Sendcmd(0x06);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(PageID>>8);
    MYUSART_SendData(PageID);
    temp = 0x01+0x06+0x06+BufferID
    +(PageID>>8)+(uint8_t)PageID;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;    
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
uint8_t PS_DeletChar(uint16_t PageID,uint16_t N)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x07);
    Sendcmd(0x0C);
    MYUSART_SendData(PageID>>8);
    MYUSART_SendData(PageID);
    MYUSART_SendData(N>>8);
    MYUSART_SendData(N);
    temp = 0x01+0x07+0x0C
    +(PageID>>8)+(uint8_t)PageID
    +(N>>8)+(uint8_t)N;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
uint8_t PS_Empty(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x0D);
    temp = 0x01+0x03+0x0D;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
uint8_t PS_WriteReg(uint8_t RegNum,uint8_t DATA)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x05);
    Sendcmd(0x0E);
    MYUSART_SendData(RegNum);
    MYUSART_SendData(DATA);
    temp = RegNum+DATA+0x01+0x05+0x0E;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    if(ensure==0)
        printf("\r\n设置参数成功！");
    //else
        //printf("\r\n%s",EnsureMessage(ensure));
    return ensure;
}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数（波特率，包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数（16bytes）
uint8_t PS_ReadSysPara(SysPara *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x0F);
    temp = 0x01+0x03+0x0F;
    SendCheck(temp);
    data=JudgeStr(1000);
    if(data)
    {
        ensure = data[9];
        p->PS_max = (data[14]<<8)+data[15];
        p->PS_level = data[17];
        p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
        p->PS_size = data[23];
        p->PS_N = data[25];
    }        
    else
        ensure=0xff;
    if(ensure==0x00)
    {
//        printf("\r\n模块最大指纹容量=%d",p->PS_max);
//        printf("\r\n对比等级=%d",p->PS_level);
//        printf("\r\n地址=%x",p->PS_addr);
//        printf("\r\n波特率=%d",p->PS_N*9600);
    }
    //else 
            //printf("\r\n%s",EnsureMessage(ensure));
    return ensure;
}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
uint8_t PS_SetAddr(uint32_t PS_addr)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x07);
    Sendcmd(0x15);
    MYUSART_SendData(PS_addr>>24);
    MYUSART_SendData(PS_addr>>16);
    MYUSART_SendData(PS_addr>>8);
    MYUSART_SendData(PS_addr);
    temp = 0x01+0x07+0x15
    +(uint8_t)(PS_addr>>24)+(uint8_t)(PS_addr>>16)
    +(uint8_t)(PS_addr>>8) +(uint8_t)PS_addr;                
    SendCheck(temp);
    AS608Addr=PS_addr;//发送完指令，更换地址
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;    
        AS608Addr = PS_addr;
    if(ensure==0x00)
        //printf("\r\n设置地址成功！");
    //else
    //    printf("\r\n%s",EnsureMessage(ensure));
    return ensure;
    return ensure;
}
//功能： 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
//    该记事本逻辑上被分成 16 个页。
//参数:  NotePageNum(0~15),Byte32(要写入内容，32个字节)
//说明:  模块返回确认字
uint8_t PS_WriteNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
    uint16_t temp;
    uint8_t  ensure,i;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(36);
    Sendcmd(0x18);
    MYUSART_SendData(NotePageNum);
    for(i=0;i<32;i++)
     {
         MYUSART_SendData(Byte32[i]);
         temp += Byte32[i];
     }
    temp =0x01+36+0x18+NotePageNum+temp;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
        ensure=data[9];
    else
        ensure=0xff;
    return ensure;
}
//读记事PS_ReadNotepad
//功能：  读取FLASH用户区的128bytes数据
//参数:  NotePageNum(0~15)
//说明:  模块返回确认字+用户信息
uint8_t PS_ReadNotepad(uint8_t NotePageNum,uint8_t *Byte32)
{
    uint16_t temp;
    uint8_t  ensure,i;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x04);
    Sendcmd(0x19);
    MYUSART_SendData(NotePageNum);
    temp = 0x01+0x04+0x19+NotePageNum;
    SendCheck(temp);
    data=JudgeStr(2000);
    if(data)
    {
        ensure=data[9];
        for(i=0;i<32;i++)
        {
            Byte32[i]=data[10+i];
        }
    }
    else
        ensure=0xff;
    return ensure;
}
//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//          若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//          很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x08);
    Sendcmd(0x1b);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(StartPage>>8);
    MYUSART_SendData(StartPage);
    MYUSART_SendData(PageNum>>8);
    MYUSART_SendData(PageNum);
    temp = 0x01+0x08+0x1b+BufferID
    +(StartPage>>8)+(uint8_t)StartPage
    +(PageNum>>8)+(uint8_t)PageNum;
    SendCheck(temp);
    data=JudgeStr(2000);
     if(data)
    {
        ensure=data[9];
        p->pageID     =(data[10]<<8) +data[11];
        p->mathscore=(data[12]<<8) +data[13];
    }
    else
        ensure=0xff;
    return ensure;
}
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t  *data;
    SendHead();
    SendAddr();
    SendFlag(0x01);//命令包标识
    SendLength(0x03);
    Sendcmd(0x1d);
    temp = 0x01+0x03+0x1d;
    SendCheck(temp);
    data=JudgeStr(200);
    if(data)
    {
        ensure=data[9];
        *ValidN = (data[10]<<8) +data[11];
    }        
    else
        ensure=0xff;
    
    if(ensure==0x00)
    {
        //printf("\r\n有效指纹个数=%d",(data[10]<<8)+data[11]);
    }
    //else
        //printf("\r\n%s",EnsureMessage(ensure));
    return ensure;
}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）    
uint8_t PS_HandShake(uint32_t *PS_Addr)
{
    SendHead();
    SendAddr();
    MYUSART_SendData(0X01);
    MYUSART_SendData(0X00);
    MYUSART_SendData(0X00);    
    delay_ms(200);
    if(AS608_USART_RX_STA&0X8000)//接收到数据
    {        
        if(//判断是不是模块返回的应答包                
                AS608_USART_RX_BUF[0]==0XEF
                &&AS608_USART_RX_BUF[1]==0X01
                &&AS608_USART_RX_BUF[6]==0X07
            )
            {
                *PS_Addr=(AS608_USART_RX_BUF[2]<<24) + (AS608_USART_RX_BUF[3]<<16)
                                +(AS608_USART_RX_BUF[4]<<8) + (AS608_USART_RX_BUF[5]);
                AS608_USART_RX_STA=0;
                return 0;
            }
        AS608_USART_RX_STA=0;                    
    }
    return 1;        
}


SysPara AS608Para;
uint16_t ValidN;
void AS608_Check(void)
{
    uint8_t cnt = 0;
    volatile uint8_t ensure;
    while(PS_HandShake(&AS608Addr))//与AS608模块握手
    {
        cnt++;if(cnt>10)break;
        delay_ms(400);
    }

    ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
    if(ensure!=0x00)
    {
        /* 错误 */
    }
    ensure=PS_ReadSysPara(&AS608Para);  //读参数 
}
/**
 * @brief   刷指纹函数 只会返回一次查询成功结果 直到下次按下查询
 * @param   none
 * @retval  0:上次查询指纹成功至今还未松开 1:指纹查询成功 2:指纹查询失败 3:没有检测到指纹 
 */
uint8_t AS608_PressFR(void)
{
    static uint8_t press_state = 0;     /* 指纹按下状态 */
    if(PS_Sta && press_state == 0)                  /* 检测PS_Sta状态，高电平为有手指按下 */
    {
        SearchResult seach;     /* 存放指纹搜索结果 */
        uint8_t ensure;         /* 确认字 */
        ensure = PS_GetImage(); /* 获取图像 */
        if(ensure == 0x00)      /* 获取图像成功 */
        {
            ensure=PS_GenChar(CharBuffer2); /* 生成特征 */
            if(ensure == 0x00)  /* 生成特征成功 */
            {
                ensure=PS_HighSpeedSearch(CharBuffer2,0,AS608Para.PS_max,&seach);/* 搜索指纹 */
                if(ensure == 0x00)/* 搜索指纹成功 */
                {
                    //seach.pageID;   /* 页码ID */
                    //seach.mathscore;/* 匹配分数 */
                    press_state = 1;    /* 成功扫描到指纹 - 下次扫描指纹：本次松开后 */
                    return 1;
                }
            }
        }
        return 2;
    }
    else if(press_state == 1 && PS_Sta)
    {
        /* 手指还没有松开 */
        return 0x00;
    }
    else
    {
        if(press_state == 1)
        {
            press_state = 0;    /* 可以扫描下一次指纹 */
        }
    }
    return 0x03;
}
/**
 * @brief   录入指纹
 * @param   none
 * @retval  0:录入指纹失败 1:录入指纹成功 2:没有检测到指纹
 */
uint8_t AS608_AddFR(void)
{
    uint8_t ensure;
    if(PS_Sta)
    {
        ensure=PS_GetImage();
        if(ensure==0x00) 
        {
            ensure=PS_GenChar(CharBuffer1);         /* 指纹特征1 */
            if(ensure==0x00)
            {
                /* 第二次按指纹前可以加入延时 这里没有添加 */
                if(PS_Sta)
                {
                    ensure=PS_GetImage();
                    if(ensure == 0x00)
                    {
                        ensure=PS_GenChar(CharBuffer2); /* 指纹特征2 */
                        if(ensure == 0x00)
                        {
                            ensure = PS_Match();        /* 匹配两次指纹是否相同 */
                            if(ensure == 0x00)
                            {
                                ensure=PS_RegModel();   /* 生成指纹模板 */
                                if(ensure==0x00) 
                                {
                                    PS_ValidTempleteNum(&ValidN);               /* 读库指纹个数 */
                                    ensure=PS_StoreChar(CharBuffer2,ValidN + 1);/* 储存模板 */
                                    if(ensure == 0x00)
                                    {
                                        return 0x01;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return 0x00;
    }
    return 0x02;
}
/**
 * @brief   删除指定的指纹ID或者清空指纹库
 * @param   delete_id   :   需要删除的指纹ID，0xFFFF删除所有指纹
 * @retval  1:删除成功 0:删除失败
 */
uint8_t AS608_DeleteFR(uint16_t delete_id)
{
    uint8_t ensure;
    if(delete_id==0xFFFF)
        ensure=PS_Empty();  /* 删除所有指纹 */
    else 
        ensure = PS_DeletChar(delete_id,1); /*  删除单个指纹 */
    if(ensure == 0x00)
    {
        return 1;
    }
    return 0;
}
/**
 * @brief   获取当前指纹库中指纹的个数
 * @param   none
 * @retval  返回当前指纹数量
 */
uint8_t AS608_GetFRNumber(void)
{
    PS_ValidTempleteNum(&ValidN);
    return ValidN;
}
#endif

