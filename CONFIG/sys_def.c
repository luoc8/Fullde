#include "sys_def.h"
#include "can.h"
#include "delay.h"

u8 DEVICE_ID = 0x01;		//默认设备号
//各锁存器的值保存
PC0_7_ODR pc0_7_odr =
{
	.cs_odr			= 0x00,				
	.en_odr			= 0x00,		
	.led_odr		= 0x00,
	.mode_odr		= 0x00		
};
/*******************************************************
*获取设备ID
*参数：
*返回：id 设备ID
********************************************************/
u8 Read_Address()
{
	u16 id = 0x0000;
	AD_245_H;
	id = GPIO_ReadInputData(GPIOC);
	AD_245_L;
	return(u8)id;
}
/*******************************************************
*读PC0-7输出口转台
*参数：
*返回：
********************************************************/
u8 Read_PC0_7_ODR()
{
	u8 pc0_7_odr = 0x00;
	pc0_7_odr = (u8)GPIO_ReadOutputData(GPIOC);
	return pc0_7_odr;
}
/*******************************************************
*置位特定io口
*参数：	*pc0_7_odr	上一次的引脚状态
		gpio_pin_x	需要置位的引脚
*返回：
********************************************************/
void Set_PC0_7_ODR(u8 *pc0_7_odr, u8 gpio_pin_x)
{
	*pc0_7_odr |= gpio_pin_x;
	GPIOC->ODR = (GPIOC->ODR&0xff00) | *pc0_7_odr;
}
/*******************************************************
*复位特定io口
*参数：	*pc0_7_odr	上一次的引脚状态
		gpio_pin_x	需要复位的引脚
*返回：
********************************************************/
void Reset_PC0_7ODR(u8 *pc0_7_odr, u8 gpio_pin_x)
{
	*pc0_7_odr &= ~gpio_pin_x;
	GPIOC->ODR = (GPIOC->ODR&0xff00) | *pc0_7_odr;
}

/*******************************************************
*初始化PC0-PC7
*参数：	1 输出
		0 输入
*返回：
********************************************************/
void Pcio_Init(u8 state)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
								GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	if(state)
	{
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_ResetBits(GPIOC,0x00ff);
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	}
		
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
/********************************************************************
*初始化通道模块相关IO
*参数：
*返回：
*********************************************************************/
void Io_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_10|GPIO_Pin_11);
	
	//锁存器输出上电后为高电平，需软件置低
	LE_CS_H;
	LE_EN_H;
	LE_LED_H;
	LE_MODE_H;
	
	delay_us(1);
	
	LE_CS_L;
	LE_EN_L;
	LE_LED_L;
	LE_MODE_L;	
}


