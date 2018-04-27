#include "sys_def.h"
#include "LTC1867.h"
#include "delay.h"

#define ADC_BUF_LEN 6  //每通道保存值个数

DEVER_MODULE *correct_module = NULL;
//校准模式
u8 correct_en = 0;

static u16 adc_buf[ADC_BUF_LEN];

#define ADC_FILTER_CNT 6  //滑动平均滤波数据长度

static u16 filter_v12[ADC_FILTER_CNT];
static u16 filter_v22[ADC_FILTER_CNT];
static u16 filter_v32[ADC_FILTER_CNT];
static u16 filter_v42[ADC_FILTER_CNT];
static u16 filter_v52[ADC_FILTER_CNT];
static u16 filter_v62[ADC_FILTER_CNT];
static u16 filter_v72[ADC_FILTER_CNT];
static u16 filter_v82[ADC_FILTER_CNT];

static u16 filter_v1[ADC_FILTER_CNT];
static u16 filter_v2[ADC_FILTER_CNT];
static u16 filter_v3[ADC_FILTER_CNT];
static u16 filter_v4[ADC_FILTER_CNT];
static u16 filter_v5[ADC_FILTER_CNT];
static u16 filter_v6[ADC_FILTER_CNT];
static u16 filter_v7[ADC_FILTER_CNT];
static u16 filter_v8[ADC_FILTER_CNT];

static u16 filter_i1[ADC_FILTER_CNT];
static u16 filter_i2[ADC_FILTER_CNT];
static u16 filter_i3[ADC_FILTER_CNT];
static u16 filter_i4[ADC_FILTER_CNT];
static u16 filter_i5[ADC_FILTER_CNT];
static u16 filter_i6[ADC_FILTER_CNT];
static u16 filter_i7[ADC_FILTER_CNT];
static u16 filter_i8[ADC_FILTER_CNT];

static u16 Get_Adc_Val(u16 *adc_buf, u16 *filter);
static void Set_Adv_Correct(DEVER_MODULE *modulex, u16 val);
static void Set_Adi_Correct(DEVER_MODULE *modulex, u16 val);
static u32 Get_Voltage(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter);
static u32 Get_Assist_Voltage(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter);
static u32 Get_Current(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter);
//static u32 Get_Temperature(MODULE_CORRECT *correct, u16 adc_val);
static void Set_Cs(DEVER_MODULE *modulex);
//static void Power_Control(DEVER_CH *chx, u32 vol);
static void Normal_Measures(void);
static void Adc_Correct(DEVER_MODULE *modulex);
static void Get_Ch_Measure(DEVER_CH *chx);
static void V_Sample(void);
static void I_Sample(void);
static void V2_Sample(void);
/********************************************************************
*通道数据采集
*参数：
*返回：
*********************************************************************/
void data_acq_task(void *argument)
{
	while(1)
	{
		if(correct_en != 1)
		{
			Normal_Measures();
		}
		else
		{
			if(correct_module != NULL)
			{
				Adc_Correct(correct_module);
			}
		}
		osDelay(2);
	}
}
/********************************************************************
*正常模式下采样数据
*参数：
*返回：
*********************************************************************/
static void Normal_Measures(void)
{
	V_Sample();
	I_Sample();

	Get_Ch_Measure(&ch1_obj);
	Get_Ch_Measure(&ch2_obj);
	Get_Ch_Measure(&ch3_obj);
	Get_Ch_Measure(&ch4_obj);
}
static void V_Sample(void)
{
	u8 i;
	
	AD_1_H;
	AD_2_H;
	osDelay(1);
	
	//电压
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module1.resoures->ad_vol_1);
	}
	module1.measure->vol_1 = Get_Voltage(&module1, adc_buf, filter_v1);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module2.resoures->ad_vol_1);
	}
	module2.measure->vol_1 = Get_Voltage(&module2, adc_buf, filter_v2);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module3.resoures->ad_vol_1);
	}
	module3.measure->vol_1 = Get_Voltage(&module3, adc_buf, filter_v3);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module4.resoures->ad_vol_1);
	}
	module4.measure->vol_1 = Get_Voltage(&module4, adc_buf, filter_v4);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module5.resoures->ad_vol_1);
	}
	module5.measure->vol_1 = Get_Voltage(&module5, adc_buf, filter_v5);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module6.resoures->ad_vol_1);
	}
	module6.measure->vol_1 = Get_Voltage(&module6, adc_buf, filter_v6);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module7.resoures->ad_vol_1);
	}
	module7.measure->vol_1 = Get_Voltage(&module7, adc_buf, filter_v7);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module8.resoures->ad_vol_1);
	}
	module8.measure->vol_1 = Get_Voltage(&module8, adc_buf, filter_v8);
}
static void I_Sample(void)
{
	u8 i;
	
	AD_1_L;
	AD_2_H;
	osDelay(1);
	//电流1	
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module1.resoures->ad_cur); 
	}
	module1.measure->cur = Get_Current(&module1, adc_buf, filter_i1);
	Set_Cs(&module1);
	//电流2
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module2.resoures->ad_cur); 
	}
	module2.measure->cur = Get_Current(&module2, adc_buf, filter_i2);
	Set_Cs(&module2);
	//电流3
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module3.resoures->ad_cur); 
	}
	module3.measure->cur = Get_Current(&module3, adc_buf, filter_i3);
	Set_Cs(&module3);
	//电流4	
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module4.resoures->ad_cur); 
	}
	module4.measure->cur = Get_Current(&module4, adc_buf, filter_i4);
	Set_Cs(&module4);
	//电流5
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module5.resoures->ad_cur); 
	}
	module5.measure->cur = Get_Current(&module5, adc_buf, filter_i5);
	Set_Cs(&module5);
	//电流6
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module6.resoures->ad_cur); 
	}
	module6.measure->cur = Get_Current(&module6, adc_buf, filter_i6);
	Set_Cs(&module6);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module7.resoures->ad_cur); 
	}
	module7.measure->cur = Get_Current(&module7, adc_buf, filter_i7);
	Set_Cs(&module7);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module8.resoures->ad_cur); 
	}
	module8.measure->cur = Get_Current(&module8, adc_buf, filter_i8);
	Set_Cs(&module8);
}
static void V2_Sample(void)
{
	u8 i;
	
	AD_1_H;
	AD_2_L;
	osDelay(1);
	//辅助电压
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module1.resoures->ad_vol_2);
	}
	module1.measure->vol_2 = Get_Assist_Voltage(&module1, adc_buf, filter_v12);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module2.resoures->ad_vol_2);
	}
	module2.measure->vol_2 = Get_Assist_Voltage(&module2, adc_buf, filter_v22);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module3.resoures->ad_vol_2);
	}
	module3.measure->vol_2 = Get_Assist_Voltage(&module3, adc_buf, filter_v32);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
	adc_buf[i] = Ltc1867_Read(module4.resoures->ad_vol_2);
	}
	module4.measure->vol_2 = Get_Assist_Voltage(&module4, adc_buf, filter_v42);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module5.resoures->ad_vol_2);
	}
	module5.measure->vol_2 = Get_Assist_Voltage(&module5, adc_buf, filter_v52);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module6.resoures->ad_vol_2);
	}
	module6.measure->vol_2 = Get_Assist_Voltage(&module6, adc_buf, filter_v62);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module7.resoures->ad_vol_2);
	}
	module7.measure->vol_2 = Get_Assist_Voltage(&module7, adc_buf, filter_v72);
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(module8.resoures->ad_vol_2);
	}
	module8.measure->vol_2 = Get_Assist_Voltage(&module8, adc_buf, filter_v82);
}
/********************************************************************
*校准模式下采样数据
*参数：
*返回：
*********************************************************************/
static void Adc_Correct(DEVER_MODULE *modulex)
{
	u8 i;

	//电压
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(modulex->resoures->ad_vol_1);
	}
	modulex->measure->vol_1 = Get_Adc_Val(adc_buf, filter_v1);
	//电流
	for(i = 0; i < ADC_BUF_LEN; i++)
	{
		adc_buf[i] = Ltc1867_Read(modulex->resoures->ad_cur); 
	}
	modulex->measure->cur = Get_Adc_Val(adc_buf, filter_i1);
}
/********************************************************************
*获取通道采样值
*参数：
*返回：
*********************************************************************/
static void Get_Ch_Measure(DEVER_CH *chx)
{
	u8 i;
	u32 sum = 0;
	MODULE_MEASURE *modx_measure;
	modx_measure = chx->module1->measure;
	chx->measure->vol_1 = modx_measure->vol_1;
//	chx->measure->vol_2 = modx_measure->vol_2;
//	chx->measure->temperture = modx_measure->temperture;
	for(i = 0; i < CH_MODULE_NUM; i++)
	{
		modx_measure = (*(&chx->module1 + i))->measure;
		sum += modx_measure->cur;
	}
	chx->measure->cur = sum;
}
/********************************************************************
*排序 小到大
*参数：	*buf	数组
				len		长度
*返回：
*********************************************************************/
static void Sort(u16 *buf, u8 len)
{
	u8 i,j;
	u16 temp;

	for (j = 0; j < len - 1; j++) //排序  小到大
	{
		for (i = j + 1; i < len; i++)
		{
			if (buf[j] > buf[i])
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
}

/********************************************************************
*中值滤波
*参数：	*adc_buf	数组		
*返回：	去掉2*N个值后的平均值
*********************************************************************/
static u16 Ad_Filter(u16 * adc_buf)
{
	u8 count;
	u16 value_buf[ADC_BUF_LEN];
	u32  sum = 0;
	for(count = 0; count < ADC_BUF_LEN; count++)  //读取一通道数据
	{
		value_buf[count] = adc_buf[count];
	}
	Sort(value_buf, ADC_BUF_LEN);  //排序  小到大

#define N 2

	for (count = N; count < ADC_BUF_LEN - N; count++)
	{
		sum += value_buf[count];
	}

	return (u16)(sum / (ADC_BUF_LEN - N * 2));
}

/********************************************************************
*滑动平均滤波
*参数：	*filter	数组
		new		新值
*返回：	去掉2*M个值后的平均值
*********************************************************************/
static u16 Filter_Glide_Ave(u16 *filter, u16 new)
{
	u8 i;
	u32 sum = 0;
	u16 temp_buf[ADC_FILTER_CNT];

	for(i = 0; i < ADC_FILTER_CNT - 1; i++)  //去掉最旧
	{
		filter[i] = filter[i + 1];
	}
		filter[ADC_FILTER_CNT - 1] = new;  //最新值

	memcpy(temp_buf, filter, ADC_FILTER_CNT * 2);
	
	Sort(temp_buf, ADC_FILTER_CNT);  //排序  小到大

#define M 2

	for(i = M; i < ADC_FILTER_CNT - M; i++)
	{
		sum += temp_buf[i];
	}
	return (u16)(sum / (ADC_FILTER_CNT - M * 2));
}
/********************************************************************
*设置ADC电压校准系数
*参数：
*返回：
*********************************************************************/
static void Set_Adv_Correct(DEVER_MODULE *modulex, u16 val)
{
	//TODO
	MODULE_INFO *info;
	CORRECT_DEF *ad_correct;
	
	info = modulex->info;
	ad_correct = modulex->ad_correct;
	
	val = (u8)(val / AD_POINT_VALUE);
	val = val > (POINT_NUM - 1) ? (POINT_NUM - 1) : val;
	
	switch (info->run_mode)
	{
		case CC_MODE:
		case CV_MODE:
		case CVC_MODE:
			modulex_correct.k_adv1 = ad_correct[val].k_cv1;
			modulex_correct.b_adv1 = ad_correct[val].b_cv1;
			break;
		case DCC_MODE:
		case DCP_MODE:
		case DCV_MODE:
		case STEWING:
			modulex_correct.k_adv1 = ad_correct[val].k_dcv1;
			modulex_correct.b_adv1 = ad_correct[val].b_dcv1;
			break;
	}
	if(modulex_correct.k_adv1 < 1E-5)
	{
		modulex_correct.k_adv1 = mod_correct_def.k_adv1;
		modulex_correct.b_adv1 = mod_correct_def.b_adv1;
	}
}
/********************************************************************
*设置ADc电流校准系数
*参数：
*返回：
*********************************************************************/
static void Set_Adi_Correct(DEVER_MODULE *modulex, u16 val)
{
	//TODO
	MODULE_INFO *info;
	CORRECT_DEF *ad_correct;
	
	info = modulex->info;
	ad_correct = modulex->ad_correct;
	
	val = (u8)(val / AD_POINT_VALUE);
	val = val > (POINT_NUM - 1) ? (POINT_NUM - 1) : val;
	
	switch (info->run_mode)
	{
		case CC_MODE:
		case CV_MODE:
		case CVC_MODE:
			modulex_correct.k_adi = ad_correct[val].k_ci;
			modulex_correct.b_adi = ad_correct[val].b_ci;
			break;
		case DCC_MODE:
		case DCP_MODE:
		case DCV_MODE:
		case STEWING:
			modulex_correct.k_adi = ad_correct[val].k_dci;
			modulex_correct.b_adi = ad_correct[val].b_dci;
			break;
	}
	if(modulex_correct.k_adi < 1E-5)
	{
		modulex_correct.k_adi = mod_correct_def.k_adi;
		modulex_correct.b_adi = mod_correct_def.b_adi;
	}
}

/********************************************************************
*滤波之后的ADC值
*参数：
*返回：
*********************************************************************/
static u16 Get_Adc_Val(u16 *adc_buf, u16 *filter)
{
	u16 adc_val;

	adc_val = Ad_Filter(adc_buf);
	adc_val = Filter_Glide_Ave(filter, adc_val);
	
	return adc_val;
}
/********************************************************************
*采集的电压值
*参数：
*返回：	
*********************************************************************/
static u32 Get_Voltage(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter)
{
	u16 adc_val;
	u32 v;
	float temp;
	
	adc_val = Get_Adc_Val(adc_buf, filter);
	if(adc_val == 0)
	{
		return adc_val;
	}
	Set_Adv_Correct(modulex, adc_val);
	temp = ((float)adc_val) * modulex_correct.k_adv1 + modulex_correct.b_adv1;
	if(temp < 1E-6)
	{
		temp = 0;
	}
	v = (u32)temp;
	
	return v;
}
/********************************************************************
*采集的辅助电压值
*参数：	
*返回：	
*********************************************************************/
static u32 Get_Assist_Voltage(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter)
{
	u16 adc_val;
	u32 v;
	float temp;
	
	adc_val = Get_Adc_Val(adc_buf, filter);
	if(adc_val == 0)
	{
		return adc_val;
	}
	Set_Adv_Correct(modulex, adc_val);
	temp = ((float)adc_val) * modulex_correct.k_adv2 + modulex_correct.b_adv2;
	if(temp < 1E-6)
	{
		temp = 0;
	}
	v = (u32)temp;
	
	return v;
}
/********************************************************************
*采集的电流值
*参数：	
*返回：	
*********************************************************************/
static u32 Get_Current(DEVER_MODULE *modulex, u16 *adc_buf, u16 *filter)
{
	u16 adc_val;
	u32 i;
	float temp;
	
	adc_val = Get_Adc_Val(adc_buf, filter);
	if(adc_val == 0)
	{
		return adc_val;
	}
	Set_Adi_Correct(modulex, adc_val);
	temp = ((float)adc_val) * modulex_correct.k_adi + modulex_correct.b_adi;
	if(temp < 1E-6)
	{
		temp = 0;
	}
	i = (u32)temp;
	
	return i;
}
/********************************************************************
*采集的温度值
*参数：	
*返回：	
*********************************************************************/
//static u32 Get_Temperature(MODULE_CORRECT *correct, u16 adc_val)
//{
//	//todo
//}
/********************************************************************
*设置同步整流信号
*参数：
*返回：
*********************************************************************/
static void Set_Cs(DEVER_MODULE *modulex)
{
	if(modulex->info->now_state == RUN)
	{
		if(modulex->measure->cur > 3000)
		{
			CS_Control(modulex->resoures->cs, SET);
		}
		else
		{
			CS_Control(modulex->resoures->cs, RESET);
		}
	}
}
/********************************************************************
*恒功率时的功率控制
*参数：	*chx	通道对象
				*info		通道参数
				
*返回：
*********************************************************************/
//static void Power_Control(DEVER_CH *chx, u32 vol)
//{
//	u32 cur;
//	if(chx->info->run_mode == DCP_MODE && chx->info->now_state == RUN)
//	{
//	cur = chx->info->set_power / vol;
//	cur = cur > chx->protect->cur_max ? chx->protect->cur_max : cur;
////	chx->config(vol, cur);
//	}
//}
/********************************************************************
*检测采集回来的值是否正常
*参数：	
*返回：
*********************************************************************/
