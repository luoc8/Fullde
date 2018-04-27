#include "sys_def.h"
#include "can.h"
#include "common.h"
#include "crc8_16.h"
static u8 send_buf[SEND_BUF_LEN];
SEND_DATA se_data =
{	
	.extid			= 0,
	.buf			= send_buf,
	.sys_times		= 0,
	.idn			= 0,
	.ch_num			= 0,			
	.state			= STOP,					
	.mode			= 0,
	.len			= 0,
	.err_code		= 0
};
TIME_DEF times =
{
	.tim_se_data    = 1,	
	.tim_hb_err	    = 15,						
	.data_se_times  = 0,				
	.hb_err_times	= 0				
};


static void Send_Msg(SEND_DATA *data);
static void Msg_Ack(SEND_DATA *data);
//static void Msg_Connect(SEND_DATA *data);
static void Msg_Sample(SEND_DATA *data, CH_MEASURE *chx_measure);
static void Msg_Heartbeat(SEND_DATA *data);
static void Msg_Correct_Sample(SEND_DATA *data, MODULE_MEASURE *modx_measure);
static void Msg_Correct_Ack(SEND_DATA *data);
/********************************************************************
*CAN数据发送任务
*参数：
*返回：
*********************************************************************/
void can_se_task(void *argument)
{	
	uint32_t flag_event;//事件标志
	while(1)
	{
		flag_event=osThreadFlagsWait(0x3f,osFlagsWaitAny,osWaitForever);//获取已触发的事件标志 一直阻塞等待
		switch(flag_event)
		{
			case SE_ACK://回应
				Msg_Ack(&se_data);
				Send_Msg(&se_data);
				break;
//			case SE_CONNECT://连接
//				Msg_Connect(&se_data);
//				Send_Msg(&se_data);
//				break;
			case SE_DATA://数据发送
				if(ch1_obj.info->ch_en == 1)
				{
					se_data.sys_times = ch1_obj.info->sys_times;
					Msg_Sample(&se_data, ch1_obj.measure);
					Send_Msg(&se_data);
				}
				if(ch2_obj.info->ch_en == 1)
				{
					se_data.sys_times = ch2_obj.info->sys_times;
					Msg_Sample(&se_data, ch2_obj.measure);
					Send_Msg(&se_data);
				}
				if(ch3_obj.info->ch_en == 1)
				{
					se_data.sys_times = ch3_obj.info->sys_times;
					Msg_Sample(&se_data, ch3_obj.measure);
					Send_Msg(&se_data);
				}
				if(ch4_obj.info->ch_en == 1)
				{
					se_data.sys_times = ch4_obj.info->sys_times;
					Msg_Sample(&se_data, ch4_obj.measure);
					Send_Msg(&se_data);
				}
				break;
			case SE_HEARTBEAT:
				Msg_Heartbeat(&se_data);
				Send_Msg(&se_data);
				break;
			case SE_FAULT_ERR:
				break;
			case SE_CRT_ACK:
				Msg_Correct_Ack(&se_data);
				Send_Msg(&se_data);
				break;
			case SE_CRT_DATA:
//				Msg_Correct_Sample(&se_data);
				Send_Msg(&se_data);
				break;
			default : break;		
		}		
	}
}
/********************************************************************
*发送消息
*参数：
*返回：
*********************************************************************/
static void Send_Msg(SEND_DATA *data)
{
	u8 msg[8];
	UNION_EXTID extid;
	
	extid.extid = data->extid;
	if(data->len > 8)
	{
		extid.mix_extid.parameter_page = 0xaa;
		memcpy(msg, data->buf, 8);
		
		Can_Send_Msg(extid.extid, msg, 8);
		
		extid.mix_extid.parameter_page = 0xff;
		memcpy(msg,&data->buf[8],8);
		
		Can_Send_Msg(extid.extid, msg, 8);
	}
	else
	{
		memcpy(msg, data->buf, data->len);
		
		Can_Send_Msg(extid.extid, msg, data->len);
	}
}
/********************************************************************
*应答消息
*参数：
*返回：
*********************************************************************/
static void Msg_Ack(SEND_DATA *data)
{
	UNION_EXTID extid;
	
	extid.extid = 0;
	
	extid.mix_extid.idn = re_data.idn;
	extid.mix_extid.device_id = DEVICE_ID;
	extid.mix_extid.special_page = 0x4f;
	
	data->extid = extid.extid;
	
	data->buf[0] = data->ch_num;
	data->buf[1] = data->state;
	data->buf[2] = data->mode;
	data->buf[3] = data->err_code;
	
	data->len = 4;
}
/********************************************************************
*联机消息
*参数：
*返回：
*********************************************************************/
//static void Msg_Connect(SEND_DATA *data)
//{
//	
//}
/********************************************************************
*采样消息
*参数：
*返回：
*********************************************************************/
static void Msg_Sample(SEND_DATA *data, CH_MEASURE *chx_measure)
{
	u16 crc_16;
	UNION_EXTID extid;
	
	extid.extid = 0;
	
	extid.mix_extid.idn = IDN_DATA_REQUEST;
	extid.mix_extid.device_id = DEVICE_ID;
	extid.mix_extid.special_page = 0x4f;
	
	data->extid = extid.extid;
	
	data->buf[0] = data->ch_num;
	data->buf[1] = data->state;
	data->buf[2] = data->mode;
	U32_To_Byte(chx_measure->cur, &data->buf[4]);
	U16_To_Byte(chx_measure->vol_1, &data->buf[3]);
	
	data->buf[8] = data->ch_num;
	data->buf[9] = 0;
	U16_To_Byte(chx_measure->temperture, &data->buf[10]);
	U16_To_Byte(data->sys_times, &data->buf[12]);
	
	crc_16 = Get_Crc16(data->buf, 14);
//	U16_To_Byte(crc_16, &data->buf[14]);
	data->buf[14] = (u8)crc_16;
	data->buf[15] = (u8)(crc_16 >> 8);
	
	data->len = 16;
}
/********************************************************************
*心跳消息
*参数：
*返回：
*********************************************************************/
static void Msg_Heartbeat(SEND_DATA *data)
{
	UNION_EXTID extid;
	
	extid.extid = 0;
	
	extid.mix_extid.idn = IDN_HEARTBEAT;
	extid.mix_extid.device_id = DEVICE_ID;
	extid.mix_extid.special_page = 0x4f;
	
	data->extid = extid.extid;
	
	data->len = 0;
}
/********************************************************************
*校准用的采样数据
*参数：
*返回：
*********************************************************************/
static void Msg_Correct_Sample(SEND_DATA *data, MODULE_MEASURE *modx_measure)
{
	UNION_EXTID extid;
	
	extid.extid = 0;
	
	extid.mix_extid.idn = IDN_CRT_DATA;
	extid.mix_extid.parameter_page = re_data.parameter_page;
	extid.mix_extid.special_page = data->ch_num;
	extid.mix_extid.device_id = DEVICE_ID;
	
	data->extid = extid.extid;
	
	data->buf[0] = data->state;
	data->buf[1] = data->mode;
	U16_To_Byte(modx_measure->vol_1, &data->buf[2]);
	U16_To_Byte(modx_measure->cur, &data->buf[4]);
	
	data->len = 6;
}
/********************************************************************
*校准模式下的回应
*参数：
*返回：
*********************************************************************/
static void Msg_Correct_Ack(SEND_DATA *data)
{
	UNION_EXTID extid;
	
	extid.extid = 0;
	
	extid.mix_extid.idn = data->idn;
	extid.mix_extid.special_page = data->ch_num;
	extid.mix_extid.device_id = DEVICE_ID;
	
	data->extid = extid.extid;
	
	data->buf[0] = data->state;
	data->buf[1] = data->mode;
	data->buf[2] = data->err_code;
	
	data->len = 3;
}
//定时器3中断服务程序		    
void TIM3_IRQHandler(void)
{ 		
//	osStatus_t err;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
		times.data_se_times++;
//		times.hb_err_tims++;
		if(times.data_se_times>=times.tim_se_data)
		{
			times.data_se_times=0;
			if(ch1_obj.info->ch_en == 1 || ch2_obj.info->ch_en == 1 || 
				 ch3_obj.info->ch_en == 1 || ch4_obj.info->ch_en == 1)
				osThreadFlagsSet(can_se_task_id,SE_DATA);
		}
		if(times.hb_err_times>=times.tim_hb_err)
		{
//			times.hb_err_tims=0;
		}
	}
}
//定时器2中断
void TIM2_IRQHandler(void)
{ 		
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
		if(ch1_obj.info->now_state == RUN)
		{
			ch1_obj.info->sys_times++;
		}
		if(ch2_obj.info->now_state == RUN)
		{
			ch2_obj.info->sys_times++;
		}
		if(ch3_obj.info->now_state == RUN)
		{
			ch3_obj.info->sys_times++;
		}
		if(ch4_obj.info->now_state == RUN)
		{
			ch4_obj.info->sys_times++;
		}
	}
}
