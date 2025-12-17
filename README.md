## RC522 与卡片的通信工作流程

整个流程通常包括 **寻卡（Request）、防冲突（Anti-collision）、选卡（Select）** 和 **数据交换（Authentication & Read/Write）** 四个主要阶段。

### 阶段一：寻卡与唤醒 (Request)

这是通信的起始阶段，读写器向工作区域内的所有卡片发送请求。

1. **发送寻卡命令：** RC522 读写器发送一个寻卡命令（如 `REQA` 或 `WUPA`），请求处于空闲或休眠状态的卡片响应。
2. **卡片响应：** 范围内所有遵循协议的卡片接收到能量和命令后，会被唤醒并返回一个简单的信号（如 `ATQA` 响应），表明自己的存在和类型。

### 阶段二：防冲突（Anti-collision）

由于电磁场内可能存在多张卡片，读写器必须从中识别并选中一张进行通信，防止多卡同时响应造成冲突。

1. **读取 UID：** 读写器发送防冲突命令，要求卡片发送其 **唯一标识符（UID）**。

2. **冲突检测与解析：** 如果多张卡片同时发送 UID，会发生冲突。

3. MFRC522 通过检测**曼彻斯特编码（Manchester Coding）**的错误来识别冲突。

   - 原理：当多个标签（Tag）同时发送数据时，如果某一位上，有的标签发送“0”，有的发送“1”，电平会相互抵消或叠加，导致读写器（PCD）无法解析该位，从而判定为**冲突位（Collision Bit）**。

   - **广播请求 (Request):** MFRC522 发送防碰撞命令（如 `0x93`），要求所有在场卡片回传其 UID（唯一标识符）。

     **同步回传与定位:** 所有卡片同步发送 UID。MFRC522 接收数据，直到检测到第一个**冲突位**（即某一位上有卡片发0，有卡片发1）。

     - *例如：在第 3 位检测到冲突。*

     **二元选择 (Selection):** MFRC522 决定在该冲突位选择“0”还是“1”（通常默认选1或由程序指定），并发送命令：“**只有第 3 位是 1 的卡片继续，其他的闭嘴**”。**指令：** `0x93` + `NVB=3` + `101`

     **迭代筛选:** 符合条件的卡片继续回传剩余的 UID 位。MFRC522 重复上述过程，逐位解决冲突，直到获得一个完整的、无冲突的 UID。

### 阶段三：选卡 (Select)

在成功读取一张卡片的 UID 后，读写器需要正式选择该卡片。

1. **发送选卡命令：** 读写器发送该卡片的完整 UID。
2. **卡片响应：** 被选中的卡片回复一个确认信息（如 `SAK`），并进入 **活动状态（Active）**，准备进行下一步的数据操作。其他卡片则返回空闲状态或休眠。

### 阶段四：数据交换（Authentication & Operation）

这是流程的最终目的，读写器与被选中的卡片进行数据的安全读写。

1. **密钥认证（Authentication）：** （对于 MIFARE Classic 等具有存储器的卡片）读写器必须先使用正确的密钥对目标扇区进行认证。
    - 读写器发送认证命令（指定扇区地址和密钥类型）。
    - 卡片使用密钥和读写器进行加密握手。
    - 只有认证成功，才能访问该扇区的数据块。
2. **数据操作：**
    - **读取数据：** 读写器发送读取命令，卡片返回指定数据块的内容。
    - **写入数据：** 读写器发送写入命令和数据，卡片将数据写入目标数据块。

### 阶段五：停止通信 (Halt)

- **停止命令：** 数据操作完成后，读写器可以发送 `HALT` 命令，使当前活动的卡片进入休眠状态。
- **结束通信：** 读写器拉高 **NSS/CS** 信号线，结束本次 SPI 通信。

# 调用链

## RC522

APP\Sensor_Task.h：启用模块

BSP\Sensor\MFRC522.c：编写函数

APP\Sensor_Task.c：Sensor_Execute()中确认模块启用后，初始化数据结构，间隔轮询获取卡数据，有三处

System\Task.c：Task_Scheduler(void) 调用Sensor_Execute()

User\main.c：调用Task_Init() ，轮询Task_Scheduler(void)

## 光敏传感器

APP\Sensor_Task.h：启用模块

BSP\Sensor\LDR.c：编写函数

APP\Sensor_Task.c：Sensor_Execute()中确认模块启用后，初始化，间隔轮询获取光照强度，有三处

System\Task.c：Task_Scheduler(void) 调用Sensor_Execute()

APP\Actuator_Task.c：Actuator_Execute(void) 中根据光照强度的不同，更改PB13的电平

User\main.c：调用Task_Init() ，轮询Task_Scheduler(void)

## 串口

HardWare\Serial.c：编写函数

APP\Action_Task.c：在Action_OLED_Update(void)中，刷新OLED的时候顺便串口输出卡号

```
//MCU通信，使用串口发送当前ID卡号
				printf("0x%02X%02X%02X%02X\n",MFRC522_Value.Read_Id[0],
                    MFRC522_Value.Read_Id[1],MFRC522_Value.Read_Id[2],MFRC522_Value.Read_Id[3]);
```

User\main.c：调用Serial_Init()

## OLED

驱动部分省略

按键1 跳转至MENU_PAGE_ONE，显示已存储的卡号

按键2 存储当前识别的卡号

按键3 在存储区删除当前识别的卡号

按键4 删除所有已存储的卡号

APP\Action_Task.c：

Action_Key_Process(void) 处理按键对应的传感器任务，Action_OLED_Update(void) 处理OLED的显示