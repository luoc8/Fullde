/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "sys_def.h"
#include "delay.h"
#include "can.h"
#include "Init.h"
#include "LTC1867.h"
#include "spi2.h"
#include "ad5676.h"

// #ifdef RTE_Compiler_EventRecorder
// #include "EventRecorder.h"
// #endif

//消息队列id
osMessageQueueId_t can_re_message_id;
//控制块
osRtxMessageQueue_t Can_Re_Message;
//内存分区
u8 Can_Re_Buf[MEM_BLK_NUM][MEM_BLK_SIZE]; //10块，每块32 CanRxMsg大小为20

//开始任务定义
//线程id
osThreadId_t start_task_id;
//任务优先级
#define START_TASK_PRIO osPriorityNormal
//任务堆栈大小
#define START_STK_SIZE 256
//任务控制块
osRtxThread_t StartTaskTCB;
//任务堆栈
uint64_t START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *argument);

//CAN 接收任务
//线程id
osThreadId_t can_re_task_id;
//任务优先级
#define CAN_RE_TASK_PRIO osPriorityNormal1
//任务堆栈大小
#define CAN_RE_STK_SIZE 64
//任务控制块
osRtxThread_t CAN_Re_TaskTCB;
//任务堆栈
uint64_t CAN_RE_TASK_STK[CAN_RE_STK_SIZE];

//CAN 发送任务
//线程id
osThreadId_t can_se_task_id;
//任务优先级
#define CAN_SE_TASK_PRIO osPriorityNormal2
//任务堆栈大小
#define CAN_SE_STK_SIZE 64
//任务控制块
osRtxThread_t CAN_Se_TaskTCB;
//任务堆栈
uint64_t CAN_SE_TASK_STK[CAN_SE_STK_SIZE];

//数据采集任务
//线程id
osThreadId_t data_acq_task_id;
//任务优先级
#define DATA_ACQ_TASK_PRIO osPriorityNormal3
//任务堆栈大小
#define DATA_ACQ_STK_SIZE 64
//任务控制块
osRtxThread_t DATA_Acq_TaskTCB;
//任务堆栈
uint64_t DATA_ACQ_TASK_STK[DATA_ACQ_STK_SIZE];

osThreadId_t fault_task_id;
#define FAULT_TASK_PRIO osPriorityNormal4
#define FAULT_STK_SIZE 64
osRtxThread_t Fault_TaskTCB;
uint64_t FAULT_TASK_STK[FAULT_STK_SIZE];
/********************************************************************
*用户外设初始化
*参数：
*返回：
*********************************************************************/
void user_init(void)
{
    //中断分组配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4) ;
    //1秒定时，us级延时有用到这个时钟源，所以要先初始化
    TIM2_Init(9999, 7199);
    //引脚初始化为输入态
    //	Pcio_Init(0);
    //	DEVICE_ID=Read_Address();
    //引脚初始化为输出态
    Pcio_Init(1);
    //通道模块相关引脚初始化
    Io_Init();
    //CAN初始化 1000kps
    CAN_Mode_Init(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq, 2);
    //ADC初始化
    Ltc1867_Init();
    //SPI2初始化 flash与DAC分时复用SPI2
    SPI2_Init();
    //DAC初始化
    AD5676_Init();
    //1秒定时
    TIM3_Init(9999, 7199);

    read_correct();
    //	TIM4_Init(9999,7199);//1秒定时
}

/********************************************************************
*主函数入口
*参数：
*返回：
*********************************************************************/
int main(void)
{
    osThreadAttr_t thread_attr;
    osMessageQueueAttr_t messagequeue_attr;

    // System Initialization
    SystemCoreClockUpdate();

    user_init();

#ifdef RTE_Compiler_EventRecorder
    // Initialize and start Event Recorder
    EventRecorderInitialize(EventRecordError, 1U);
#endif
    // ...
    // Initialize CMSIS-RTOS
    osKernelInitialize();

    messagequeue_attr.name = "message queue";
    messagequeue_attr.attr_bits = 0;
    messagequeue_attr.cb_mem = &Can_Re_Message;
    messagequeue_attr.cb_size = sizeof(Can_Re_Message);
    messagequeue_attr.mq_mem = Can_Re_Buf;
    messagequeue_attr.mq_size = sizeof(Can_Re_Buf);

    can_re_message_id = osMessageQueueNew(MEM_BLK_NUM, sizeof(CanRxMsg), &messagequeue_attr);

    thread_attr.name = "start task";                 //名称
    thread_attr.attr_bits = osThreadDetached;        //创建在分离状态
    thread_attr.cb_mem = &StartTaskTCB;              //任务控制块
    thread_attr.cb_size = sizeof(StartTaskTCB);      //控制块大小
    thread_attr.stack_mem = &START_TASK_STK[0];      //任务栈
    thread_attr.stack_size = sizeof(START_TASK_STK); //任务栈大小
    thread_attr.priority = osPriorityNormal;         //优先级
    thread_attr.tz_module = 0;                       //？？？
    thread_attr.reserved = 0;                        //需为0
                                                     // Create application main thread
    start_task_id = osThreadNew(start_task, NULL, &thread_attr);
    // Start thread execution
    osKernelStart();
    for (;;)
    {
    }
}
void start_task(void *argument)
{
    osThreadAttr_t thread_attr;
    //调度上锁
    osKernelLock();
    //数据采集任务
    thread_attr.name = "data acq task";                 //名称
    thread_attr.attr_bits = osThreadDetached;           //创建在分离状态
    thread_attr.cb_mem = &DATA_Acq_TaskTCB;             //任务控制块
    thread_attr.cb_size = sizeof(DATA_Acq_TaskTCB);     //控制块大小
    thread_attr.stack_mem = &DATA_ACQ_TASK_STK[0];      //任务栈
    thread_attr.stack_size = sizeof(DATA_ACQ_TASK_STK); //任务栈大小
    thread_attr.priority = DATA_ACQ_TASK_PRIO;          //优先级
    thread_attr.tz_module = 0;                          //？？？
    thread_attr.reserved = 0;                           //需为0
    //创建数据采集任务
    data_acq_task_id = osThreadNew(data_acq_task, NULL, &thread_attr);

    //CAN接收数据处理任务
    thread_attr.name = "can re task";                 //名称
    thread_attr.cb_mem = &CAN_Re_TaskTCB;             //任务控制块
    thread_attr.stack_mem = &CAN_RE_TASK_STK[0];      //任务栈
    thread_attr.stack_size = sizeof(CAN_RE_TASK_STK); //任务栈大小
    thread_attr.priority = CAN_RE_TASK_PRIO;          //优先级
    //创建CAN接收数据处理任务
    can_re_task_id = osThreadNew(can_re_task, NULL, &thread_attr);

    //CAN发送数据任务
    thread_attr.name = "can se task";                 //名称
    thread_attr.cb_mem = &CAN_Se_TaskTCB;             //任务控制块
    thread_attr.stack_mem = &CAN_SE_TASK_STK[0];      //任务栈
    thread_attr.stack_size = sizeof(CAN_SE_TASK_STK); //任务栈大小
    thread_attr.priority = CAN_SE_TASK_PRIO;          //优先级
    //创建CAN发送数据任务
    can_se_task_id = osThreadNew(can_se_task, NULL, &thread_attr);

    //	//故障处理任务
    //	thread_attr.name = "fault task";
    //	thread_attr.cb_mem = &Fault_TaskTCB;
    //	thread_attr.stack_mem = &FAULT_TASK_STK[0];
    //	thread_attr.stack_size = sizeof(FAULT_TASK_STK);
    //	thread_attr.priority = FAULT_TASK_PRIO;
    //
    //	fault_task_id=osThreadNew(fault_dpe_task,NULL,&thread_attr);
    //解锁
    osKernelUnlock();
    //挂起开始任务
    osThreadSuspend(start_task_id);
}
