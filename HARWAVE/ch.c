#include "sys_def.h"
#include "module.h"
#include "ch.h"
#include "delay.h"

static void LED_Control(u16 led,FlagStatus state);

/********************************************************************
*通道各模块配置
*参数：
*返回：
*********************************************************************/
void Ch_Config(DEVER_CH *chx, u32 vol, u32 cur)
{
	u32 num,cur_val = 0;
	DEVER_MODULE *modulex;
	MODULE_INFO *module_info;
	u16 temp[CH_MODULE_NUM];
	u8 i;
	
	memset(temp, 0, CH_MODULE_NUM * 2);
	
	if( cur != 0)
	{
		num = cur / DEVICE_MODULE_I_MAX + 1;
		num = num > CH_MODULE_NUM ? CH_MODULE_NUM : num;
		cur_val = cur / num;
		
		for(i = 0; i < (num - 1); i++)
		{
			temp[i] = cur_val;
		}
		temp[num - 1] = cur - cur_val * (num - 1);
	}
	
	for(i = 0; i < CH_MODULE_NUM; i++)
	{
		//获取通道下的模块
		modulex = *(&chx->module1 + i);
		module_info = modulex->info;
		//模块参数配置
		module_info->set_vol = vol;
		module_info->set_cur = temp[i];
		module_info->run_mode = chx->info->run_mode;
	}
}
/********************************************************************
*刷新通道状态和led状态
*参数：
*返回：
*********************************************************************/
void Refresh_Ch_Status(DEVER_CH *chx, u8 cmd)
{
	CH_RESOURCES *chx_res;
	chx_res = chx->ch_res;
	if(cmd == RUN || cmd == CONTINUE)
	{
		switch(chx->info->run_mode)
		{
			//充电模式
			case CC_MODE:
			case CV_MODE:
			case CVC_MODE:
				
				LED_Control(chx_res->led_1, SET);
				LED_Control(chx_res->led_2 | chx_res->led_4, RESET);
				break;
			//放电模式	
			case DCC_MODE:
			case DCP_MODE:
			case DCV_MODE:
		
				LED_Control(chx_res->led_2, SET);
				LED_Control(chx_res->led_1 | chx_res->led_4, RESET);
				break;
			//静置模式	
			case STEWING:
				LED_Control(chx_res->led_1 | chx_res->led_2 | chx_res->led_4, RESET);
				break;

			default : 
				break;	
		}
		chx->info->now_state = RUN;
		//使能数据发送
		chx->info->ch_en = 1;
	}
	else if(cmd == STOP)
	{
		LED_Control(chx_res->led_4, SET);
		LED_Control(chx_res->led_1 | chx_res->led_2, RESET);
		
		chx->info->sys_times = 0;
		chx->info->now_state = STOP;
		//失能数据发送
		chx->info->ch_en = 0;
	}
	else if(cmd == PAUSE && chx->info->now_state == RUN)
	{
		LED_Control(chx_res->led_1 | chx_res->led_2 | chx_res->led_4, RESET);
		
		chx->info->now_state = PAUSE;
	}
}
/********************************************************************
*通道控制
*参数：
*返回：
*********************************************************************/
u8 Ch_Control(DEVER_CH *chx, u8 cmd)
{
	DEVER_MODULE *modulex;
	u8 i,err_code = FB_SUCCESS;

	Ch_Config(chx, chx->info->set_vol, chx->info->set_cur);
	for(i = 0; i < CH_MODULE_NUM; i++)
	{
		//获取通道下的模块
		modulex = *(&chx->module1 + i);
		err_code = Module_Control(modulex, cmd);
		if(err_code != FB_SUCCESS)
		{
			return err_code;
		}
	}
	if(err_code == FB_SUCCESS)
	{
		Refresh_Ch_Status(chx, cmd);
	}
	return err_code;
}

/*******************************************************
*led状态灯控制
*参数：	led 要操作的灯
		state：
			SET	开
			RESET 关
*返回：
********************************************************/
static void LED_Control(u16 led, FlagStatus state)
{
	if(state)
	{
		LED_SET(led);
	}
	else
	{
		LED_RESET(led);
	}
	LE_LED_H;
	delay_us(1);
	LE_LED_L;
}

