#ifndef _OS_CFG_H
#define _OS_CFG_H
#include "rtx_os.h" 


/************************* 消息id ***********************************/
extern osMessageQueueId_t 	    can_re_message_id;

/************************* 任务控制块 ********************************/
extern osRtxThread_t 		    CAN_Re_TaskTCB;
extern osRtxThread_t 		    CAN_Se_TaskTCB;
extern osRtxThread_t 		    DATA_Acq_TaskTCB;
extern osRtxThread_t 		    Fault_TaskTCB;

/************************* 线程id ***********************************/
extern osThreadId_t 		    can_re_task_id;
extern osThreadId_t 		    can_se_task_id;
extern osThreadId_t 		    data_acq_task_id;
extern osThreadId_t			    fault_task_id;

/*********************** CAN_SE_FLAGGRP的标志 *************************/
#define SE_ACK     			    (uint32_t)0x0001	//控制指令回馈
#define SE_DATA         	    (uint32_t)0x0002	//数据发送
//#define SE_CONNECT				(uint32_t)0x0004
#define SE_HEARTBEAT    	    (uint32_t)0x0004	//心跳数据包发送
#define SE_FAULT_ERR    	    (uint32_t)0x0008	//错误信息发送
#define SE_CRT_ACK			    (uint32_t)0x0010	//写校验系数反馈
#define SE_CRT_DATA			    (uint32_t)0x0020	//返回读的校验

/*********************** FAULIT_FLAGGRP的标志 ************************/

#define ERR_HEARTBEAT_TIMOUT 	(uint32_t)0x0001	//心跳等待回应超时
#define ERR_OVERVOLTAGE      	(uint32_t)0x0002	//过压
#define ERR_OVERCURRENT      	(uint32_t)0x0004	//过流
#define ERR_OVERTEMPERATURE  	(uint32_t)0x0008	//过温
#define ERR_UNDERVOLTAGE     	(uint32_t)0x0010	//欠压
#define ERR_PARAMETER        	(uint32_t)0x0020	//参数错误
#define ERR_HARD_FAULT       	(uint32_t)0x0800	//系统跑飞

/************************* 任务 ****************************/
void can_re_task(void *argument);					//CAN 接收任务
void can_se_task(void *argument);					//CAN 发送任务
void data_acq_task(void *argument);					//数据采集任务
void fault_dpe_task(void *argument);

#endif
