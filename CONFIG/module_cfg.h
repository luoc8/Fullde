#ifndef _MODULE_CFG_H
#define _MODULE_CFG_H
#include "stm32f10x.h"
#include "define.h"

#define MODULE_1				1
#define MODULE_2				2
#define MODULE_3				3
#define MODULE_4				4
#define MODULE_5				5
#define MODULE_6				6
#define MODULE_7				7
#define MODULE_8				8
typedef struct
{
	u16 cs;												//同步整流
	u16 mode;											//充/放电模式
	u16 en;												//硬件环路使能
	u8 dac_n;											//第几块DAC
	u8 da_vol;											//输出电压对应的DAC通道
	u8 da_cur;											//输出电压对应的DAC通道
	u8 ad_vol_1;										//电压采样的ADC通道
	u8 ad_vol_2;										//辅助电压采样的ADC通道
	u8 ad_cur;											//电流采样的ADC通道
//	u8 ad_temperture;									//温度采样的ADC通道
}const MODULE_RESOURES;
typedef struct
{	
	u16 vol_1;   										//电压
	u16 vol_2; 											//辅助电压
	u16 cur;											//电流
	u16 temperture;  									//温度
}MODULE_MEASURE;
typedef struct
{
	u32 set_vol;										//设置电压
	u32 set_cur;										//设置电流
	u8 run_mode;										//要运行模式
	u8 now_state;										//状态
}MODULE_INFO;
typedef struct
{
	float k_dav;										//dac电压K值
	float b_dav;										//dac电压b值
	float k_dai;										//dac电流k
	float b_dai;										//dac电流b
	float k_adv1;										//adc电压k
	float b_adv1;										//adc电压b
	float k_adv2;										//adc辅助电压k
	float b_adv2;										//adc辅助电压b
	float k_adi;										//adc电流k
	float b_adi;										//adc电流b
}MODULE_CORRECT;
typedef struct
{
	float k_cv1;										//充电电压k
	float b_cv1;										//充电电压b
//	float k_cv2;
//	float b_cv2;
	float k_ci;											//充电电流k
	float b_ci;											//充电电流b
	float k_dcv1;										//放电电压k
	float b_dcv1;										//放电电压b
//	float k_dcv2;
//	float b_dcv2;
	float k_dci;										//放电电流k
	float b_dci;										//放电电流b
}CORRECT_DEF;
typedef struct
{
	MODULE_RESOURES *resoures;
	MODULE_MEASURE *measure;
	MODULE_INFO *info;
//	MODULE_CORRECT *correct;
	CORRECT_DEF	*ad_correct;
	CORRECT_DEF	*da_correct;
}const DEVER_MODULE;

#endif
