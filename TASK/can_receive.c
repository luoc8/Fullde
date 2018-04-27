#include "sys_def.h"
#include "common.h"
#include "module.h"
#include "ch.h"
#include "flash_if.h"
#include "crc8_16.h"
/*****************************变量声明*******************************/
CanRxMsg 			can_rx_msg;
u8 					receive_buf[RECEIVE_BUF_LEN];
RECEIVE_DEF 		re_data;
/*****************************函数声明*******************************/
//static void Clear_Buf(u8 *buf,u8 len);
static u8 Cheak_Crc(u8 *buf,u16 len);
static u8 Msg_Wr_To_Buf(CanRxMsg *msg);
static u8 Idn_Cmd(RECEIVE_DEF *data);
static u8 Idn_Protect_Cfg(RECEIVE_DEF *data);
static u8 Idn_Fault(RECEIVE_DEF *data);
static u8 Idn_Config(RECEIVE_DEF *data);
static u8 Check_Parameter(DEVER_CH *chx);
static u8 Idn_Crt_Cmd(RECEIVE_DEF *data);
static u8 Idn_Re_X_Cfg(RECEIVE_DEF *data);
static u8 Idn_Wr_Ad_Cfg(RECEIVE_DEF *data);
static u8 Idn_Wr_Da_Cfg(RECEIVE_DEF *data);
static u8 Write_Correct(CORRECT_DEF *correct, RECEIVE_DEF *data);
/********************************************************************
*CAN解析数据任务
*参数：
*返回：
*********************************************************************/
void can_re_task(void *argument)
{
	uint32_t err;
	CanRxMsg msg;
	u8 err_code = FB_OTHER_ERR;
	
	while(1)
	{
		err = osMessageQueueGet(can_re_message_id, &msg, NULL, osWaitForever);//一直阻塞等待本任务的信号量
		if(err != osOK)
		{
			break;
		}
		if(msg.FMI == 0x00)
		{
			err_code = Msg_Wr_To_Buf(&msg);
			
			se_data.idn = re_data.idn;
			se_data.ch_num = re_data.ch_num;
			
			if(err_code == FB_SUCCESS)
			{
				switch (re_data.idn)
				{
					case IDN_CMD:
						
						se_data.err_code = Idn_Cmd(&re_data);
						osThreadFlagsSet(can_se_task_id, SE_ACK);
						break;
					
					case IDN_PROTECT_CFG:
						
						se_data.err_code = Idn_Protect_Cfg(&re_data);
						osThreadFlagsSet(can_se_task_id, SE_ACK);
						break;
					
					case IDN_FAULT:
						
						se_data.err_code = Idn_Fault(&re_data);
						break;
					
					case IDN_CONFIG:
						
						se_data.err_code = Idn_Config(&re_data);
						osThreadFlagsSet(can_se_task_id, SE_ACK);
						break;
					case IDN_CRT_CMD:
						
						se_data.err_code = Idn_Crt_Cmd(&re_data);
						osThreadFlagsSet(can_se_task_id, SE_CRT_ACK);
						break;
					case IDN_RE_AD_CFG:
					case IDN_RE_DA_CFG:
						//TODO
						se_data.err_code = Idn_Re_X_Cfg(&re_data);
//						osThreadFlagsSet(can_se_task_id, SE_CRT_ACK);
						break;
					
					case IDN_WR_AD_CFG:
						//TODO
						if(re_data.parameter_page == 0xaa)
						{
							se_data.err_code = Wr_x_Correct(ADC_CORRECT_ADDRESS,&mod_adcorrect[0][0],SIZE_CORRECT_MEM);
						}
						else
						{
							se_data.err_code = Idn_Wr_Ad_Cfg(&re_data);
						}
						osThreadFlagsSet(can_se_task_id, SE_CRT_ACK);
						break;
					
					case IDN_WR_DA_CFG:
						//TODO
						if(re_data.parameter_page == 0xaa)
						{
							se_data.err_code = Wr_x_Correct(DAC_CORRECT_ADDRESS,&mod_dacorrect[0][0],SIZE_CORRECT_MEM);
						}
						else
						{
							se_data.err_code = Idn_Wr_Da_Cfg(&re_data);
						}
						osThreadFlagsSet(can_se_task_id, SE_CRT_ACK);
						break;
					
					default : break;
				}
			}
			else if(err_code == FB_CRC_ERR)
			{
				se_data.err_code = err_code;
				osThreadFlagsSet(can_se_task_id, SE_ACK);
			}
		}
		else if(msg.FMI == 0x01 || msg.FMI == 0x02)
		{
			err=osThreadFlagsSet(can_se_task_id, SE_HEARTBEAT);//置位心跳包发送	
			if(err!=osOK)
			{
				
			}
		}
	}
}
/********************************************************************
*解析CAN消息
*参数：	*msg Can_Rx_Msg
*返回：	errcode
*********************************************************************/
static u8 Msg_Wr_To_Buf(CanRxMsg *msg)
{
	UNION_EXTID extid;
	
	extid.extid = msg->ExtId;
	re_data.idn = extid.mix_extid.idn;
	re_data.parameter_page = extid.mix_extid.parameter_page;
	
	switch(re_data.idn)
	{
		case IDN_CMD:	
		case IDN_FAULT:
		case IDN_DATA_REQUEST:
			
			re_data.ch_num = msg->Data[0];
			memcpy(re_data.buf, &msg->Data, msg->DLC);
			re_data.len = msg->DLC;
		
			return FB_SUCCESS;
				
		case IDN_CRT_CMD:
		case IDN_WR_AD_CFG:
		case IDN_WR_DA_CFG:
			
			re_data.ch_num = extid.mix_extid.special_page;
			memcpy(re_data.buf, &msg->Data, msg->DLC);
			re_data.len = msg->DLC;
		
			return FB_SUCCESS;
		
		case IDN_PROTECT_CFG:
		case IDN_CONFIG:
			if(re_data.parameter_page == 0xaa)
			{
				re_data.ch_num = msg->Data[0];
				memcpy(re_data.buf, msg->Data, 8);
				re_data.len = 8;
			}
			else if(re_data.parameter_page == 0xff)//最后一帧
			{
				memcpy(&re_data.buf[re_data.len], msg->Data, 4);
				re_data.len += 4;
				memcpy(&re_data.crc_16, &msg->Data[6], 2);
				//CRC校验
				if( Cheak_Crc(re_data.buf, re_data.len)) 
				{
					return FB_SUCCESS;
				}					
					return FB_CRC_ERR;
			}
			break;
	}
	return FB_OTHER_ERR;
}
/********************************************************************
*控制命令
*参数：
*返回：
*********************************************************************/
static u8 Idn_Cmd(RECEIVE_DEF *data)
{
	u8 err_code = FB_OTHER_ERR;
	DEVER_CH *chx;
	
	switch (data->ch_num)
	{
		case CH_1:
			chx = &ch1_obj;
			break;
		case CH_2:
			chx = &ch2_obj;
			break;
		case CH_3:
			chx = &ch3_obj;
			break;
		case CH_4:
			chx = &ch4_obj;
			break;
		default : 
			return FB_NO_THIS_CH;
	}
	if(chx->info->ch_err == 0)
	{
		chx->info->cmd = data->buf[1];
		if(data->buf[1] == RUN)
		{
			chx->info->run_mode = data->buf[2];
		}
		
		err_code = Ch_Control(chx, chx->info->cmd);
		
		if(err_code != FB_SUCCESS)
		{
			return err_code;
		}
	}
	else
	{
		err_code = FB_OTHER_ERR;
	}
	se_data.state = chx->info->now_state;
	se_data.mode = chx->info->run_mode;
	
	return err_code;
}
/********************************************************************
*保护参数设置
*参数：
*返回：
*********************************************************************/
static u8 Idn_Protect_Cfg(RECEIVE_DEF *data)
{
	u8 err_code = FB_OTHER_ERR;
//	DEVER_CH *chx;
	CH_PROTECT *chx_protect;
	
	switch (data->ch_num)
	{
		case CH_1:
			chx_protect = ch1_obj.protect;
			break;
		case CH_2:
			chx_protect = ch2_obj.protect;
			break;
		case CH_3:
			chx_protect = ch3_obj.protect;
			break;
		case CH_4:
			chx_protect = ch4_obj.protect;
			break;
		default : 
			return FB_NO_THIS_CH;
	}
	
	chx_protect->vol_max = Get_U16(&data->buf[2]);
	chx_protect->vol_min = Get_U16(&data->buf[4]);
	chx_protect->cur_max = Get_U32(&data->buf[6]);
	chx_protect->temperture_max = Get_U16(&data->buf[10]);
	
	se_data.mode = PROTECT_CFG;
	
	if(chx_protect->vol_max > DEVICE_CH_V_MAX || chx_protect->cur_max > DEVICE_CH_I_MAX)
	{
		return FB_PARAMETER_ERR;
	}
	
	err_code = FB_SUCCESS;
	
	return err_code;
}
/********************************************************************
*故障处理
*参数：
*返回：
*********************************************************************/
static u8	Idn_Fault(RECEIVE_DEF *data)
{
	u8 err_code = FB_OTHER_ERR;
	DEVER_CH *chx;
	
	switch (data->ch_num)
	{
		case CH_1:
			chx = &ch1_obj;
			break;
		case CH_2:
			chx = &ch2_obj;
			break;
		case CH_3:
			chx = &ch3_obj;
			break;
		case CH_4:
			chx = &ch4_obj;
			break;
		default : 
			return FB_NO_THIS_CH;
	}
	chx->info->ch_err = Get_U16(&data->buf[4]);
	
	//TODO
				
	se_data.state = STOP;
	err_code = FB_SUCCESS;

	return err_code;
}
/********************************************************************
*工步参数设置
*参数：
*返回：
*********************************************************************/
static u8	Idn_Config(RECEIVE_DEF *data)
{
	u8 err_code = FB_OTHER_ERR;
	DEVER_CH *chx;

	switch (data->ch_num)
	{
		case CH_1:
			chx = &ch1_obj;
			break;
		case CH_2:
			chx = &ch2_obj;
			break;
		case CH_3:
			chx = &ch3_obj;
			break;
		case CH_4:
			chx = &ch4_obj;
			break;
		default : 
			return FB_NO_THIS_CH;
	}
	
	chx->info->run_mode = data->buf[1];
//	chx->info->step = data->buf[1];
	chx->info->set_vol = Get_U16(&data->buf[2]);
	chx->info->set_cur = Get_U32(&data->buf[4]);
	chx->info->set_power = Get_U32(&data->buf[8]);
//	chx->info->end_vol = Get_U32(&data->buf[14]);
//	chx->info->end_cur = Get_U32(&data->buf[18]);
//	chx->info->end_cap = Get_U32(&data->buf[22]);
//	chx->info->end_time = Get_U16(&data->buf[26]);
	err_code = Check_Parameter(chx);
	if(err_code == FB_SUCCESS)
	{
		chx->info->ch_err = 0;
	}
	else
	{
		chx->info->ch_err = 1;
	}
	se_data.mode = chx->info->run_mode;

	return err_code;
}
/********************************************************************
*参数检查
*参数：
*返回：
*********************************************************************/
static u8 Check_Parameter(DEVER_CH *chx)
{
	if(chx->protect->vol_max == 0 || chx->protect->cur_max == 0)
	{
		return FB_PARAMETER_ERR;
	}
	switch (chx->info->run_mode)
	{
		case CC_MODE:
		case CV_MODE:
		case CVC_MODE:
			//检查电压、电流参数是否超出保护最大值
			if(chx->info->set_vol > chx->protect->vol_max || chx->info->set_cur > chx->protect->cur_max)
			{
					//返回参数错误代码
					return FB_PARAMETER_ERR;
			}
			switch (chx->info->run_mode)
			{
				case CC_MODE:
					if(chx->info->set_vol == 0)
					{
						chx->info->set_vol = chx->protect->vol_max;
					}
					break;
				case CV_MODE:
					if(chx->info->set_cur == 0)
					{
						chx->info->set_cur = chx->protect->cur_max;
					}
					break;
				case CVC_MODE :
					break;
			}
			break;
		case DCC_MODE:
		case DCP_MODE:
		case DCV_MODE:					
			if(chx->info->set_vol < chx->protect->vol_min || chx->info->set_cur > chx->protect->cur_max)
			{
				return FB_PARAMETER_ERR;
			}
			switch (chx->info->run_mode)
			{
				case DCC_MODE:
					if(chx->info->set_vol == 0)
					{
						chx->info->set_vol = chx->protect->vol_min;
					}					
					break;
				case DCP_MODE:
					if(chx->info->set_cur == 0)
					{
						chx->info->set_cur = chx->protect->cur_max;
					}
					chx->info->set_vol = chx->info->set_power / chx->info->set_cur;
					break;
				case DCV_MODE:
					break;
			}
			break;
		case STEWING:
			chx->info->set_vol = 0;
			chx->info->set_cur = 0;
			break;
		default : 
			return FB_NO_THIS_MODE;
	}
	return FB_SUCCESS;
}
/********************************************************************
*校准控制
*参数：
*返回：
*********************************************************************/
static u8 Idn_Crt_Cmd(RECEIVE_DEF *data)
{
	u8 cmd, err_code = FB_OTHER_ERR;
	DEVER_MODULE *modulex;

	switch (data->ch_num)
	{
		case MODULE_1:
			modulex = &module1;
			break;
		case MODULE_2:
			modulex = &module2;
			break;
		case MODULE_3:
			modulex = &module3;
			break;
		case MODULE_4:
			modulex = &module4;
			break;
		case MODULE_5:
			modulex = &module5;
			break;
		case MODULE_6:
			modulex = &module6;
			break;
		case MODULE_7:
			modulex = &module7;
			break;
		case MODULE_8:
			modulex = &module8;
			break;
		default : 
			return FB_NO_THIS_CH;
	}
	
	cmd = data->buf[0];
	if(cmd == RUN)
	{
		modulex->info->run_mode = data->buf[1];
		modulex->info->set_vol = Get_U16(&data->buf[2]);
		modulex->info->set_cur = Get_U16(&data->buf[4]);
		
		correct_module = modulex;
		correct_en = 1;
	}
	else
	{
		correct_module = NULL;
		correct_en = 0;
	}
	err_code = Module_Control(modulex, cmd);
	
	if(err_code != FB_SUCCESS)
	{
		return err_code;
	}
	se_data.state = modulex->info->now_state;
	se_data.mode = modulex->info->run_mode;

	return err_code;
}
/********************************************************************
*读校准系数
*参数：
*返回：
*********************************************************************/
static u8 Idn_Re_X_Cfg(RECEIVE_DEF *data)
{
	u8 i,j,err_code = FB_SUCCESS;
	i = (data->parameter_page >> 4) & 0x0f;
	j = data->parameter_page & 0x0f;

	if(i > 3 || j > POINT_NUM - 1)
	{
		return FB_PARAMETER_ERR;
	}
	return err_code;
}

/********************************************************************
*写AD校准系数
*参数：
*返回：
*********************************************************************/
static u8 Idn_Wr_Ad_Cfg(RECEIVE_DEF *data)
{
	u8 err_code = FB_SUCCESS;
	CORRECT_DEF *correct;
	
	switch (data->ch_num)
	{
		case MODULE_1:
			correct = module1.ad_correct;
			break;
		case MODULE_2:
			correct = module2.ad_correct;
			break;
		case MODULE_3:
			correct = module3.ad_correct;
			break;
		case MODULE_4:
			correct = module4.ad_correct;
			break;
		case MODULE_5:
			correct = module5.ad_correct;
			break;
		case MODULE_6:
			correct = module6.ad_correct;
			break;
		case MODULE_7:
			correct = module7.ad_correct;
			break;
		case MODULE_8:
			correct = module8.ad_correct;
			break;
		default :
			return FB_NO_THIS_CH;
	}
	
	err_code = Write_Correct(correct,data);
	
	return err_code;
}
/********************************************************************
*写da校准系数
*参数：
*返回：
*********************************************************************/
static u8 Idn_Wr_Da_Cfg(RECEIVE_DEF *data)
{
	u8 err_code = FB_SUCCESS;
	CORRECT_DEF *correct;
	
	switch (data->ch_num)
	{
		case MODULE_1:
			correct = module1.da_correct;
			break;
		case MODULE_2:
			correct = module2.da_correct;
			break;
		case MODULE_3:
			correct = module3.da_correct;
			break;
		case MODULE_4:
			correct = module4.da_correct;
			break;
		case MODULE_5:
			correct = module5.da_correct;
			break;
		case MODULE_6:
			correct = module6.da_correct;
			break;
		case MODULE_7:
			correct = module7.da_correct;
			break;
		case MODULE_8:
			correct = module8.da_correct;
			break;
		default :
			return FB_NO_THIS_CH;
	}
	
	err_code = Write_Correct(correct,data);
	
	return err_code;
}
/********************************************************************
*校准参数写入结构体
*参数：
*返回：
*********************************************************************/
static u8 Write_Correct(CORRECT_DEF *correct, RECEIVE_DEF *data)
{
	u8 i,j,err_code = FB_SUCCESS;
	
	i = (data->parameter_page >> 4) & 0x0f;
	j = data->parameter_page & 0x0f;

	if(j > POINT_NUM - 1)
	{
		return FB_PARAMETER_ERR;
	}
	switch (i)
	{
		case 0:
			correct[j].k_cv1 = Get_Float(&data->buf[0]);
			correct[j].b_cv1 = Get_Float(&data->buf[4]);
			break;
		case 1:
			correct[j].k_ci = Get_Float(&data->buf[0]);
			correct[j].b_ci = Get_Float(&data->buf[4]);
			break;
		case 2:
			correct[j].k_dcv1 = Get_Float(&data->buf[0]);
			correct[j].b_dcv1 = Get_Float(&data->buf[4]);
			break;
		case 3:
			correct[j].k_dci = Get_Float(&data->buf[0]);
			correct[j].b_dci = Get_Float(&data->buf[4]);
			break;
		default : 
			return FB_PARAMETER_ERR;
	}
	return err_code;
}
/********************************************************************
*CAN1中断接收
*参数：
*返回：
*********************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	osStatus_t err;
		CanRxMsg msg;
	
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		CAN_Receive(CAN1, CAN_FIFO0, &msg);
		err = osMessageQueuePut(can_re_message_id, &msg, NULL, 0);
		if(err != osOK)
		{
			
		}
	}
	else if(CAN_GetITStatus(CAN1,CAN_IT_FF0))
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
	}
	else if(CAN_GetITStatus(CAN1,CAN_IT_FOV0))
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
		CAN_FIFORelease(CAN1,CAN_FIFO0);
	}
}
/********************************************************************
*清空制定长度的缓存区
*参数：	*buf	数组
				len		数据长度
*返回：
*********************************************************************/
//static void Clear_Buf(u8 *buf,u8 len)
//{
//	if(len<=RECEIVE_BUF_LEN)
//	{
//		memset(&buf[0],0,len);
//	}
//}
/********************************************************************
*CRC16校验
*参数：
*返回：
*********************************************************************/
static u8 Cheak_Crc(u8 *buf, u16 len)
{
	u16 crc_16 = 0,receive_crc = 0;
	
	receive_crc = re_data.crc_16;
	crc_16 = Get_Crc16(buf, len);
	if(crc_16 == receive_crc)
	{
		return 1;
	}
	return 0;
}

