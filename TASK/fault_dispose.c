#include "sys_def.h"

#if FAU_DIS_DBG  
#include "dbg.h"
#include "stdio.h"
#include "string.h"
u8 fau_dis_msg[30];
#define fau_dis_dbg_msg RTT_dbg_msg
#define fau_dis_dbg_err RTT_dbg_error
#else
#define fau_dis_dbg_msg (void)
#define fau_dis_dbg_err (void)
#endif
//u16 fault_code[DEVICE_CHANNAL_NUM];
//CAN1故障处理中断
//void CAN1_SCE_IRQHandler()
//{
//	
//}
//硬错误
void HardFault_Handler(void)
{
	while(1);
}	
//故障处理任务
//void fault_dpe_task(void *argument)
//{
//	osStatus_t err;
//	uint32_t flag_event;
//	while(1)
//	{
//		flag_event=osThreadFlagsWait(ERR_HEARTBEAT_TIMOUT+ERR_OVERVOLTAGE+ERR_OVERCURRENT+ERR_UNDERVOLTAGE+ERR_PARAMETER+ERR_HARD_FAULT,
//																	osFlagsWaitAny,osWaitForever);

//		switch(flag_event)
//		{
//			case ERR_HEARTBEAT_TIMOUT:
//				
//				break;
//			case ERR_OVERVOLTAGE:
//				break;
//			case ERR_OVERCURRENT:
//				break;
//			case ERR_OVERTEMPERATURE:
//				break;
//			case ERR_UNDERVOLTAGE:
//				break;
//			case ERR_PARAMETER:
//				break;
//			case ERR_HARD_FAULT:
//				break;
//			default :
//				break;
//		}
//	}
//}	

