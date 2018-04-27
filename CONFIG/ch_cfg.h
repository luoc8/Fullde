#ifndef _CH_CFG_H
#define _CH_CFG_H
#include "module_cfg.h"
typedef struct
{
//	MODULE_RESOURES *mod1_res;
//	MODULE_RESOURES *mod2_res;
//	MODULE_RESOURES *mod3_res;
	u16 led_1;											//led灯
	u16 led_2;
	u16 led_3;
	u16 led_4;
}const CH_RESOURCES;
typedef struct
{
	u32 set_vol;										//设置电压
	u32 set_cur;										//设置电流
	u32 set_power;										//设置功率
//	u32 end_vol;
//	u32 end_cur;
//	u32 end_cap;
//	u16 end_time;
	u16 sys_times;										//通道运行时间
	u16 ch_err;											//通道错误
	u8 ch_en;											//通道使能
	u8 now_state;										//目前运行状态
	u8 run_mode;										//要运行模式
//	u8 cfg_mode;										//配置的模式
//	u8 step;
	u8 cmd;												//控制命令
}CH_INFO;
typedef struct
{
	u32 vol_max;      									//最大电压
	u32 vol_min;										//最小电压		
	u32 temperture_max;  								//最大温度
	u32 cur_max;										//最大电流
}CH_PROTECT;
typedef struct
{
//	uint64_t capacity;
	u32 vol_1;											//电压
	u32 vol_2;											//辅助电压
	u32 cur;											//电流
//	u32 capacity;										//容量
	u32 temperture;										//温度
}CH_MEASURE;
typedef struct
{
	DEVER_MODULE 	*module1;							//通道对应的模块
	DEVER_MODULE 	*module2;
	CH_RESOURCES 	*ch_res;							//通道对应的外设资源
	CH_INFO 		*info;								
	CH_PROTECT 		*protect;
	CH_MEASURE		*measure;
}const DEVER_CH;

#endif

