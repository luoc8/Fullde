#include "sys_def.h"
#include "module.h"
#include "ad5676.h"
#include "delay.h"
static u16 Convert_Vol_Dac(MODULE_CORRECT *correct, u32 val);
static u16 Convert_Cur_Dac(MODULE_CORRECT *correct, u32 val);
static u8 Write_Channel(u32 write_val, u8 channel, u8 dac_n);
static void Ch_En(u16 en, FlagStatus state);
static void Mode_Select(u16 mode, FlagStatus state);
static void Set_Da_Correct(DEVER_MODULE *modulex, u16 vol, u16 cur);
//static u8 Fan_Off (void);
/********************************************************************
*单模块配置
*参数：
*返回：
*********************************************************************/
u8 Module_Config(DEVER_MODULE *modulex, u32 vol, u32 cur)
{
	u8 err_code = FB_SUCCESS;
	u16 vol_dac,cur_dac;
	MODULE_RESOURES *modulex_res;
	
	modulex_res = modulex->resoures;
	
	//TODO:配置模块检准码
	Set_Da_Correct(modulex, vol, cur);
	
	vol_dac = Convert_Vol_Dac(&modulex_correct, vol);
	cur_dac = Convert_Cur_Dac(&modulex_correct, cur);
	
	err_code &= Write_Channel(vol_dac, modulex_res->da_vol, modulex_res->dac_n);
	err_code &= Write_Channel(cur_dac, modulex_res->da_cur, modulex_res->dac_n);
	
	return err_code;
}
/********************************************************************
*单模块控制
*参数：
*返回：
*********************************************************************/
u8 Module_Control(DEVER_MODULE *modulex, u8 cmd)
{
	u8 err_code = FB_OTHER_ERR;
	
	osKernelLock();
	if(cmd == RUN || cmd == CONTINUE)
	{
		err_code = Module_Start(modulex);
	}
	else if(cmd == STOP)
	{
		err_code = Module_Stop(modulex);
	}
	else if(cmd == PAUSE && modulex->info->now_state == RUN)
	{
		err_code = Module_Pause(modulex);
	}
	osKernelUnlock();
	
	return err_code;
}
/********************************************************************
*运行模块
*参数：
*返回：
*********************************************************************/
u8 Module_Start(DEVER_MODULE *modulex)
{	
	u8 err_code = FB_OTHER_ERR;
	MODULE_RESOURES *modulex_res;
	
	modulex_res = modulex->resoures;
	
	switch(modulex->info->run_mode)
	{
		//充电模式
		case CC_MODE:
		case CV_MODE:
		case CVC_MODE:
		case AD_CRT_MODE:
			
			Ch_En(modulex_res->en, RESET);
			Mode_Select(modulex_res->mode, SET);
			Ch_En(modulex_res->en, SET);
			err_code = Module_Config(modulex, modulex->info->set_vol, modulex->info->set_cur);
		
			break;
		//放电模式	
		case DCC_MODE:
		case DCP_MODE:
		case DCV_MODE:
		case AD_DCRT_MODE:
			
			Ch_En(modulex_res->en, RESET);
			Mode_Select(modulex_res->mode, RESET);
			Ch_En(modulex_res->en, SET);
			err_code = Module_Config(modulex, modulex->info->set_vol, modulex->info->set_cur);				

			break;
		//静置模式	
		case STEWING:
			err_code = Module_Config(modulex, 0, 0);
			
			Ch_En(modulex_res->en, RESET);
			break;
		case DA_CRT_MODE:
			
			Ch_En(modulex_res->en, RESET);
			Mode_Select(modulex_res->mode, SET);
			Ch_En(modulex_res->en, SET);
			
			err_code = Write_Channel(modulex->info->set_vol, modulex_res->da_vol, modulex_res->dac_n);
			err_code &= Write_Channel(modulex->info->set_cur, modulex_res->da_cur, modulex_res->dac_n);
			break;
		case DA_DCRT_MODE:
			
			Ch_En(modulex_res->en, RESET);
			Mode_Select(modulex_res->mode, RESET);
			Ch_En(modulex_res->en, SET);
			
			err_code = Write_Channel(modulex->info->set_vol, modulex_res->da_vol, modulex_res->dac_n);
			err_code &= Write_Channel(modulex->info->set_cur, modulex_res->da_cur, modulex_res->dac_n);
			break;
		
		default : 
			return FB_NO_THIS_CMD;		
	}
	
	FAN_ON;
	modulex->info->now_state = RUN;
	
	return err_code;
}
/********************************************************************
*停止模块
*参数：
*返回：
*********************************************************************/
u8 Module_Stop(DEVER_MODULE *modulex)
{
	u8 err_code = FB_OTHER_ERR;
	MODULE_RESOURES *modulex_res;
	
	modulex_res = modulex->resoures;
	
	CS_Control(modulex_res->cs, RESET);
	
	err_code = Module_Config(modulex, 0, 0);
			
	Ch_En(modulex_res->en, RESET);
	Mode_Select(modulex_res->mode, RESET);
	if(pc0_7_odr.en_odr == 0)
	{
		FAN_OFF;
	}
	modulex->info->now_state = STOP;
	
	return err_code;
}
/********************************************************************
*暂停模块
*参数：
*返回：
*********************************************************************/
u8 Module_Pause(DEVER_MODULE *modulex)
{	
	u8 err_code = FB_OTHER_ERR;
	MODULE_RESOURES *modulex_res;
	
	modulex_res = modulex->resoures;
	
	err_code = Module_Config(modulex, 0, 0);
			
	Ch_En(modulex_res->en, RESET);
	
	modulex->info->now_state = PAUSE;
	
	return err_code;
}
/*******************************************************
*将电压值转成DAC值
*参数：	val 电压值
*返回：	val DAC值
********************************************************/
static u16 Convert_Vol_Dac(MODULE_CORRECT *correct, u32 val)
{
	float temp;

	if(val == 0)
	{
		return (u16)val;
	}
	temp = ((float)val) * correct->k_dav + correct->b_dav;
	if(temp < 1E-6)
	{
		temp = 0;
	}
	val = (u32)temp;
	return (u16)val;
}
/*******************************************************
*将电流值转换成DAC值
*参数：	val 电流值
*返回：	val DAC值
********************************************************/
static u16 Convert_Cur_Dac(MODULE_CORRECT *correct, u32 val)
{
	float temp;

	if(val == 0)
	{
		return (u16)val;
	}
	temp =((float)val) * correct->k_dai + correct->b_dai;
	if(temp < 1E-6)
	{
		temp = 0;
	}
	val = (u32)temp;
	return (u16)val;
}
/*******************************************************
*单通道写DAC
*参数：	write_val DAC值 
		channal 通道号
*返回：	0 成功
		1 失败
********************************************************/
static u8 Write_Channel(u32 write_val, u8 channel, u8 dac_n)
{
	u32 read_val;
	u8 i, err_code = FB_OTHER_ERR;
	for(i = 0;i < 2; i++)
	{
		AD5676_WriteFunction(dac_n, AD5676_CMD_WR_DAC_REG, channel, write_val);
		read_val = AD5676_ReadBack(channel, dac_n);
		if(read_val == write_val)
		{
			return FB_SUCCESS;
		}
		else 
		{
			err_code = FB_W_DAC_ERR;
		}
	}
	return err_code;
}
/*******************************************************
*单个模块使能
*参数：
*返回：
********************************************************/
static void Ch_En(u16 en, FlagStatus state)
{
	if(state)
	{
		EN_SET(en);
	}
	else
	{
		EN_RESET(en);
	}
	LE_EN_H;
	delay_us(1);
	LE_EN_L;
}
/*******************************************************
*单模块模式选择
*参数：	 
*返回：
********************************************************/
static void Mode_Select(u16 mode, FlagStatus state)
{
	if(state)
	{
		MODE_SET(mode);
	}
	else
	{
		MODE_RESET(mode);
	}
	LE_MODE_H;
	delay_us(1);
	LE_MODE_L;
}
/*******************************************************
*同步整流控制
*参数：	cs 要操作模块
		state：
		SET	使能
		RESET 失能
*返回：
********************************************************/
void CS_Control(u16 cs, FlagStatus state)
{
	if(state)
	{
		CS_SET(cs);
	}
	else
	{
		CS_RESET(cs);
	}
	LE_CS_H;
	delay_us(1);
	LE_CS_L;
}
/********************************************************************
*设置ADC校准系数
*参数：
*返回：
*********************************************************************/
static void Set_Da_Correct(DEVER_MODULE *modulex, u16 vol, u16 cur)
{
	//TODO
	MODULE_INFO *info;
	CORRECT_DEF *da_correct;
	info = modulex->info;
	da_correct = modulex->da_correct;
	
	vol = (u8)(vol / DA_V_POINT_VALUE);
	vol = vol > (POINT_NUM - 1) ? (POINT_NUM - 1) : vol;
	
	cur = (u8)(cur / DA_I_POINT_VALUE);
	cur = cur > (POINT_NUM - 1) ? (POINT_NUM - 1) : cur;
	switch (info->run_mode)
	{
		case CC_MODE:
		case CV_MODE:
		case CVC_MODE:
			modulex_correct.k_dav = da_correct[vol].k_cv1;
			modulex_correct.b_dav = da_correct[vol].b_cv1;
			modulex_correct.k_dai = da_correct[cur].k_ci;
			modulex_correct.b_dai = da_correct[cur].b_ci;
			break;
		case DCC_MODE:
		case DCP_MODE:
		case DCV_MODE:
		case STEWING:
			modulex_correct.k_dav = da_correct[vol].k_dcv1;
			modulex_correct.b_dav = da_correct[vol].b_dcv1;
			modulex_correct.k_dai = da_correct[cur].k_dci;
			modulex_correct.b_dai = da_correct[cur].b_dci;
			break;
	}
	if(modulex_correct.k_dav < 1E-5)
	{
		modulex_correct.k_dav = mod_correct_def.k_dav;
		modulex_correct.b_dav = mod_correct_def.b_dav;
	}
	if(modulex_correct.k_dai < 1E-5)
	{
		modulex_correct.k_dai = mod_correct_def.k_dai;
		modulex_correct.b_dai = mod_correct_def.b_dai;
	}
}
u8 Stop_All_Module(void)
{
	u8 err_code = FB_SUCCESS;
	
	err_code &= Module_Stop(&module1);
	err_code &= Module_Stop(&module2);
	err_code &= Module_Stop(&module3);
	err_code &= Module_Stop(&module4);
	err_code &= Module_Stop(&module5);
	err_code &= Module_Stop(&module6);
	err_code &= Module_Stop(&module7);
	err_code &= Module_Stop(&module8);
	return err_code;
}
