#ifndef _IO_CFG_H
#define _IO_CFG_H
#include "stm32f10x.h"
/************************* 控制的同步引脚 *****************************/
#define LE_CS_H         	GPIO_SetBits(GPIOA,GPIO_Pin_0)		
#define LE_CS_L     		GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define CS_1            	GPIO_Pin_0
#define CS_2            	GPIO_Pin_1
#define CS_3            	GPIO_Pin_2
#define CS_4            	GPIO_Pin_3
#define CS_5            	GPIO_Pin_4
#define CS_6            	GPIO_Pin_5
#define CS_7            	GPIO_Pin_6
#define CS_8            	GPIO_Pin_7
#define CS_SET(x) 	    	Set_PC0_7_ODR(&pc0_7_odr.cs_odr,x)
#define CS_RESET(x) 		Reset_PC0_7ODR(&pc0_7_odr.cs_odr,x)

/*********************** IC工作的使能控制引脚 **************************/
#define LE_EN_H     		GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LE_EN_L     		GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define EN_1            	GPIO_Pin_0
#define EN_2            	GPIO_Pin_1
#define EN_3            	GPIO_Pin_2
#define EN_4            	GPIO_Pin_3
#define EN_5            	GPIO_Pin_4
#define EN_6            	GPIO_Pin_5
#define EN_7            	GPIO_Pin_6
#define EN_8           		GPIO_Pin_7
#define EN_SET(x) 	    	Set_PC0_7_ODR(&pc0_7_odr.en_odr,x)
#define EN_RESET(x) 		Reset_PC0_7ODR(&pc0_7_odr.en_odr,x)

/************************ LED的控制引脚 ********************************/
#define LE_LED_H    		GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LE_LED_L    		GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LED_1           	GPIO_Pin_0							//一通道充电状态
#define LED_2           	GPIO_Pin_1							//一通道放电状态
#define LED_3           	GPIO_Pin_2							//一通道异常状态
#define LED_4           	GPIO_Pin_3							//一通道测试正常结束状态
#define LED_5           	GPIO_Pin_4							//二通道充电状态
#define LED_6           	GPIO_Pin_5							//二通道放电状态
#define LED_7           	GPIO_Pin_6							//二通道异常状态
#define LED_8           	GPIO_Pin_7							//二通道测试正常结束状态
#define LED_SET(x) 			Set_PC0_7_ODR(&pc0_7_odr.led_odr,x)
#define LED_RESET(x) 		Reset_PC0_7ODR(&pc0_7_odr.led_odr,x)

/********************** 充放电工作模式控制引脚 **************************/
#define LE_MODE_H   		GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define LE_MODE_L   		GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define MODE_1				GPIO_Pin_0
#define MODE_2				GPIO_Pin_1
#define MODE_3				GPIO_Pin_2
#define MODE_4				GPIO_Pin_3
#define MODE_5				GPIO_Pin_4
#define MODE_6				GPIO_Pin_5
#define MODE_7				GPIO_Pin_6
#define MODE_8				GPIO_Pin_7
#define MODE_SET(x)			Set_PC0_7_ODR(&pc0_7_odr.mode_odr,x)
#define MODE_RESET(x)		Reset_PC0_7ODR(&pc0_7_odr.mode_odr,x)

/*********************** 地址获取控制引脚 *******************************/
#define AD_245_H    		GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define AD_245_L    		GPIO_ResetBits(GPIOB,GPIO_Pin_8)

/*********************** AD采集通道的切换 *******************************/
#define AD_1_H				GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define AD_1_L				GPIO_ResetBits(GPIOC,GPIO_Pin_10)

#define AD_2_H				GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define AD_2_L				GPIO_ResetBits(GPIOC,GPIO_Pin_11)
/*************************** 风扇控制 **********************************/
#define FAN_ON				GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define FAN_OFF				GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#endif
