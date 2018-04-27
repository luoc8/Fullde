#ifndef SYS_DEF_H
#define SYS_DEF_H
#include "stm32f10x.h"
#include "os_cfg.h"
#include "define.h"
#include "ch_cfg.h"
#include "module_cfg.h"
#include "can_cfg.h"
#include "io_cfg.h"
#include "string.h"
typedef struct
{
	u8 tim_se_data;										//数据发送时间间隔
	u8 tim_hb_err;										//心跳超时时间间隔
	u8 data_se_times;									//数据发送计时
	u8 hb_err_times;									//心跳时间计时
}TIME_DEF;
typedef struct
{
	u16 crc_16;											//modbus校验码
	u8 len;												//数据长度
	u8 ch_num;											//通道号/校准时对应模块号
	u8 idn;												//帧类型
//	u8 special_page;
	u8 parameter_page;									//参数页
	u8 buf[RECEIVE_BUF_LEN];							//数据
}RECEIVE_DEF;
typedef struct
{
	u32 extid;											//canid
	u8 *buf;											//要发送的数据
	u16 sys_times;										//当前通道时间
	u8 idn;												//指令标识
	u8 ch_num;											//目前的通道号
	u8 state;											//目前的运行状态
	u8 mode;											//目前运行的模式
//	u8 step;											//工步号
	u8 len;												//数据长度
	u8 err_code;										//0xff成功 其他失败	
}SEND_DATA;

typedef struct
{
	u8 cs_odr;											//同步
	u8 en_odr;											//IC使能
	u8 led_odr;											//led
	u8 mode_odr;										//充/放电模式
}PC0_7_ODR;

/************************* 全局变量 **********************************/
extern u8 				DEVICE_ID;							//设备ID
extern PC0_7_ODR 		pc0_7_odr;							//PC0-7输出寄存器
extern TIME_DEF 		times;								//时间相关
extern DEVER_MODULE   	module1,module2,module3,module4,
						module5,module6,module7,module8;	//模块控制对象
extern DEVER_CH 		ch1_obj,ch2_obj,ch3_obj,ch4_obj;	//通道控制对象
extern RECEIVE_DEF		re_data;							//接收的数据
extern SEND_DATA 		se_data;							//要发送的数据

extern MODULE_CORRECT 	modulex_correct;					//当前调用的K、b值
extern MODULE_CORRECT 	mod_correct_def;					//默认校准系数

extern DEVER_MODULE 	*correct_module;					//要校准的模块
extern u8 				correct_en;							//校准使能

extern CORRECT_DEF 		mod_adcorrect[DEVICE_MODULE_NUM][POINT_NUM];	//AD校准参数						
extern CORRECT_DEF 		mod_dacorrect[DEVICE_MODULE_NUM][POINT_NUM];	//DA校准参数

u8 Read_Address(void);
void Pcio_Init(u8 state);
void Set_PC0_7_ODR(u8 *pc0_7_odr, u8 gpio_pin_x);
void Reset_PC0_7ODR(u8 *pc0_7_odr, u8 gpio_pin_x);
void Io_Init(void);
void CS_Control(u16 cs, FlagStatus state);
void read_correct(void);
u8 Wr_x_Correct(u32 address, CORRECT_DEF *correct, u32 len);

#endif

