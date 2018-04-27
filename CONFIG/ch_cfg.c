#include "sys_def.h"
/********************************************************************
*通道数根据需求更改
*通道所包含的模块数根据需求更改
********************************************************************/

/* 通道资源 *********************************************************/
CH_RESOURCES ch1_resource =
{
	.led_1						= LED_1,
	.led_2						= LED_2,
	.led_3						= LED_3,
	.led_4						= LED_4,
};

CH_RESOURCES ch2_resource =
{
	.led_1						= LED_1,
	.led_2						= LED_2,
	.led_3						= LED_3,
	.led_4						= LED_4,
};

CH_RESOURCES ch3_resource =
{
	.led_1						= LED_1,
	.led_2						= LED_2,
	.led_3						= LED_3,
	.led_4						= LED_4,
};

CH_RESOURCES ch4_resource =
{
	.led_1						= LED_1,
	.led_2						= LED_2,
	.led_3						= LED_3,
	.led_4						= LED_4,
};

/* 通道运行所需信息 ***********************************************************/
CH_INFO ch1_info,ch2_info,ch3_info,ch4_info;

/* 通道保护参数 *******************************************************/
CH_PROTECT ch1_protect,ch2_protect,ch3_protect,ch4_protect;

/* 通道采样值 *********************************************************/
CH_MEASURE ch1_measure,ch2_measure,ch3_measure,ch4_measure;

/* 通道对象 ***********************************************************/
DEVER_CH ch1_obj =
{
	.module1					= &module1,					
	.module2					= &module2,
	.ch_res						= &ch1_resource,						
	.info						= &ch1_info,
	.protect					= &ch1_protect,
	.measure					= &ch1_measure
};

DEVER_CH ch2_obj =
{
	.module1					= &module3,					
	.module2					= &module4,
	.ch_res						= &ch2_resource,						
	.info						= &ch2_info,
	.protect					= &ch2_protect,
	.measure					= &ch2_measure
};

DEVER_CH ch3_obj =
{
	.module1					= &module5,					
	.module2					= &module6,
	.ch_res						= &ch3_resource,						
	.info						= &ch3_info,
	.protect					= &ch3_protect,
	.measure					= &ch3_measure
};

DEVER_CH ch4_obj =
{
	.module1					= &module7,					
	.module2					= &module8,
	.ch_res						= &ch4_resource,						
	.info						= &ch4_info,
	.protect					= &ch4_protect,
	.measure					= &ch4_measure
};

