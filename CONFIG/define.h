#ifndef _DEFINE_H
#define _DEFINE_H



#define DEVICE_TYPE					1
#define DEVICE_SOFT_VER				1
#define DEVICE_CH_POWER				(uint16_t)1
#define DEVICE_POWER				(uint16_t)10
//150A 一个通道对应6个模块
#define DEVICE_CH_NUM  				(4)						            //通道数 2
#define CH_MODULE_NUM				(2)						            //单通道模块数
#define DEVICE_MODULE_NUM  		    (DEVICE_CH_NUM * CH_MODULE_NUM)		//整个设备多少模块
#define DEVICE_CH_V_MAX				(5000u)				                //通道电压最大值
#define DEVICE_CH_I_MAX				(60000u)			                //通道电流最大值
#define DEVICE_MODULE_I_MAX		    (DEVICE_CH_I_MAX / CH_MODULE_NUM)   //单模块最大值

#define POINT_NUM					(5)						            //分5段区间进行采样
#define SIZE_CORRECT_DEF			(sizeof(CORRECT_DEF))
#define SIZE_CORRECT_MEM			(SIZE_CORRECT_DEF * DEVICE_MODULE_NUM * POINT_NUM)//校准系数大小
#define AD_POINT_VALUE				(65535 / POINT_NUM)
#define DA_V_POINT_VALUE			(5000 / POINT_NUM)
#define DA_I_POINT_VALUE			(30000 / POINT_NUM)

#define RECEIVE_BUF_LEN     	    32						//接收数组长度
#define SEND_BUF_LEN        	    32						//发送缓存长度
#define CH_1             			1						//通道1
#define CH_2             			2						//通道2
#define CH_3						3						//通道3
#define CH_4						4						//通道4
#define MEM_BLK_NUM         	    10						//储存块数
#define MEM_BLK_SIZE        	    32						//储存块大小 = CanRxMsg大小为20 + os_message_t大小为12

#endif
