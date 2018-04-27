#include "sys_def.h"
#include "ad5676.h"
#include "ltc1867.h"
/*********************************************************************
*
*
*
*
**********************************************************************/

/* 模块外设资源 **********************************************************/
MODULE_RESOURES mod1_resource =
{
	.cs							= CS_1,
	.mode						= MODE_1,
	.en							= EN_1,
	.dac_n						= SELECT_DA_1,
	.da_vol						= AD5676_DAC_0,
	.da_cur						= AD5676_DAC_1,
	.ad_vol_1					= LTC1867_CH0,
	.ad_vol_2					= LTC1867_CH0,
	.ad_cur						= LTC1867_CH0
//	.ad_temperture		= LTC1867_CH6
};

MODULE_RESOURES mod2_resource =
{
	.cs							= CS_2,
	.mode						= MODE_2,
	.en							= EN_2,
	.dac_n						= SELECT_DA_1,
	.da_vol						= AD5676_DAC_2,
	.da_cur						= AD5676_DAC_3,
	.ad_vol_1					= LTC1867_CH1,
	.ad_vol_2					= LTC1867_CH1,
	.ad_cur						= LTC1867_CH1
//	.ad_temperture		= LTC1867_CH6
};

MODULE_RESOURES mod3_resource =
{
	.cs							= CS_3,
	.mode						= MODE_3,
	.en							= EN_3,
	.dac_n						= SELECT_DA_1,
	.da_vol						= AD5676_DAC_4,
	.da_cur						= AD5676_DAC_5,
	.ad_vol_1					= LTC1867_CH2,
	.ad_vol_2					= LTC1867_CH2,
	.ad_cur						= LTC1867_CH2
//	.ad_temperture		= LTC1867_CH6
};

MODULE_RESOURES mod4_resource =
{
	.cs							= CS_4,
	.mode						= MODE_4,
	.en							= EN_4,
	.dac_n						= SELECT_DA_1,
	.da_vol						= AD5676_DAC_6,
	.da_cur						= AD5676_DAC_7,
	.ad_vol_1					= LTC1867_CH3,
	.ad_vol_2					= LTC1867_CH3,
	.ad_cur						= LTC1867_CH3
//	.ad_temperture		= LTC1867_CH7
};

MODULE_RESOURES mod5_resource =
{
	.cs							= CS_5,
	.mode						= MODE_5,
	.en							= EN_5,
	.dac_n						= SELECT_DA_2,
	.da_vol						= AD5676_DAC_0,
	.da_cur						= AD5676_DAC_1,
	.ad_vol_1					= LTC1867_CH4,
	.ad_vol_2					= LTC1867_CH4,
	.ad_cur						= LTC1867_CH4
//	.ad_temperture		= LTC1867_CH7
};

MODULE_RESOURES mod6_resource =
{
	.cs							= CS_6,
	.mode						= MODE_6,
	.en							= EN_6,
	.dac_n						= SELECT_DA_2,
	.da_vol						= AD5676_DAC_2,
	.da_cur						= AD5676_DAC_3,
	.ad_vol_1					= LTC1867_CH5,
	.ad_vol_2					= LTC1867_CH5,
	.ad_cur						= LTC1867_CH5
//	.ad_temperture		= LTC1867_CH7
};

MODULE_RESOURES mod7_resource =
{
	.cs							= CS_7,
	.mode						= MODE_7,
	.en							= EN_7,
	.dac_n						= SELECT_DA_2,
	.da_vol						= AD5676_DAC_4,
	.da_cur						= AD5676_DAC_5,
	.ad_vol_1					= LTC1867_CH6,
	.ad_vol_2					= LTC1867_CH6,
	.ad_cur						= LTC1867_CH6
//	.ad_temperture		= LTC1867_CH7
};

MODULE_RESOURES mod8_resource =
{
	.cs							= CS_8,
	.mode						= MODE_8,
	.en							= EN_8,
	.dac_n						= SELECT_DA_2,
	.da_vol						= AD5676_DAC_6,
	.da_cur						= AD5676_DAC_7,
	.ad_vol_1					= LTC1867_CH7,
	.ad_vol_2					= LTC1867_CH7,
	.ad_cur						= LTC1867_CH7
//	.ad_temperture		= LTC1867_CH7
};

/* 模块采样值 ***************************************************************/
MODULE_MEASURE mod1_measure,mod2_measure,mod3_measure,mod4_measure,
				mod5_measure,mod6_measure,mod7_measure,mod8_measure;

/* 模块运行所需信息 *********************************************************/
MODULE_INFO mod1_info,mod2_info,mod3_info,mod4_info,mod5_info,mod6_info,mod7_info,mod8_info;

/* 模块校准系数 *************************************************************/
//MODULE_CORRECT mod1_correct,mod2_correct,mod3_correct,mod4_correct,mod5_correct,mod6_correct;
MODULE_CORRECT modulex_correct;

/* 模块默认校准系数 *********************************************************/
MODULE_CORRECT mod_correct_def =
{
	.k_dav						= 1.030900E+1,
	.b_dav						= 3.054601E+2,
	.k_dai						= 1.772602E+0,
	.b_dai						= 4.218851E+2,
	.k_adv1						= 1.595895E-1,
	.b_adv1						= -6.2487373E+0,
	.k_adv2						= 1.595895E-1,
	.b_adv2						= -6.2487373E+0,	
	.k_adi						= 9.303372E-1,
	.b_adi						= -4.214370E+2,
};
/* 模块操作对象 ************************************************************/
DEVER_MODULE module1 = 
{
	.resoures					= &mod1_resource,
	.measure					= &mod1_measure,
	.info						= &mod1_info,
//	.correct					= &mod1_correct,
	.ad_correct					= mod_adcorrect[0],
	.da_correct					= mod_dacorrect[0]
};

DEVER_MODULE module2 = 
{
	.resoures					= &mod2_resource,
	.measure					= &mod2_measure,
	.info						= &mod2_info,
//	.correct					= &mod2_correct,
	.ad_correct					= mod_adcorrect[1],
	.da_correct					= mod_dacorrect[1]
};

DEVER_MODULE module3 = 
{
	.resoures					= &mod3_resource,
	.measure					= &mod3_measure,
	.info						= &mod3_info,
//	.correct					= &mod3_correct,
	.ad_correct					= mod_adcorrect[2],
	.da_correct					= mod_dacorrect[2]
};

DEVER_MODULE module4 = 
{
	.resoures					= &mod4_resource,
	.measure					= &mod4_measure,
	.info						= &mod4_info,
//	.correct					= &mod4_correct,
	.ad_correct					= mod_adcorrect[3],
	.da_correct					= mod_dacorrect[3]
};

DEVER_MODULE module5 = 
{
	.resoures					= &mod5_resource,
	.measure					= &mod5_measure,
	.info						= &mod5_info,
//	.correct					= &mod5_correct,
	.ad_correct					= mod_adcorrect[4],
	.da_correct					= mod_dacorrect[4]
};

DEVER_MODULE module6 = 
{
	.resoures					= &mod6_resource,
	.measure					= &mod6_measure,
	.info						= &mod6_info,
//	.correct					= &mod6_correct,
	.ad_correct					= mod_adcorrect[5],
	.da_correct					= mod_dacorrect[5]
};

DEVER_MODULE module7 = 
{
	.resoures					= &mod7_resource,
	.measure					= &mod7_measure,
	.info						= &mod7_info,
//	.correct					= &mod6_correct,
	.ad_correct					= mod_adcorrect[6],
	.da_correct					= mod_dacorrect[6]
};

DEVER_MODULE module8 = 
{
	.resoures					= &mod8_resource,
	.measure					= &mod8_measure,
	.info						= &mod8_info,
//	.correct					= &mod6_correct,
	.ad_correct					= mod_adcorrect[7],
	.da_correct					= mod_dacorrect[7]
};
