#ifndef __CAN_H
#define __CAN_H	     
#include "stm32f10x.h"
							    										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp); 
u8 Can_Send_Msg(u32 extid,u8* msg,u8 len);						
u8 Can_Receive_Msg(u8 *buf);							
#endif

















