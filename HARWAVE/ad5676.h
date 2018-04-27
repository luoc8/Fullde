#ifndef _AD5676_H
#define _AD5676_H
#include "stm32f10x.h"
#define DCEN_ON 1    //菊花链使能
#define DAC_NUM	2		
#define SELECT_DA_1		1
#define SELECT_DA_2		2
//#define SELECT_DA_3		3
/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
#define SYNC_H       GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define SYNC_L       GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define AD5676_CMD(x)                  ((0x000F & (x)) << 20)
#define AD5676_ADDR(x)                 ((0x000F & (x)) << 16)
#define AD5676_DATA_BITS(x)            ((0xFFFF & (x)) <<  0)

#define AD5676_CMD_NOP					0 // 无操作
#define AD5676_CMD_WR_IN_REG			1 // 写入输入寄存器n
#define AD5676_CMD_UPDATE_DAC_REG       2 // 以输入寄存器n的值更新DAC寄存器n
#define AD5676_CMD_WR_DAC_REG   		3 // 写入并更新DAC通道n
#define AD5676_CMD_WR_PWR_DAC			4 // DAC关断/上电
#define AD5676_CMD_HW_LDAC_MASK			5 // 硬件LDAC屏蔽寄存器
#define AD5676_CMD_SW_RESET			    6 // 软件复位（上电复位）
#define AD5676_CMD_INT_REF_SETUP		7 // 内部基准电压源设置寄存器	
#define AD5676_CMD_SET_DCEN				8 // 设置DCEN寄存器（菊花链使能）
#define AD5676_CMD_SET_READBACK 		9 // 设置回读寄存器（回读使能）
#define AD5676_CMD_UPDATE_IN_REG_ALL	10 // 用输入数据同时更新所有通道的输入寄存器
#define AD5676_CMD_UPDATE_DAC_REG_ALL	11 // 用输入数据同时更新所有通道的DAC寄存器和输入寄存器

/**********************DAC通道地址********************************************/
#define AD5676_DAC_0            0
#define AD5676_DAC_1            1
#define AD5676_DAC_2            2
#define AD5676_DAC_3            3
#define AD5676_DAC_4            4
#define AD5676_DAC_5            5
#define AD5676_DAC_6            6
#define AD5676_DAC_7            7
/*********************菊花链使能（DCEN）寄存器********************************/
#define AD5676_DCEN_OFF           0 //独立模式
#define AD5676_DCEN_ON           1 //DCEN模式
/*************************** SPI specific macros *****************************/

/* Power Setup Register definition */
#define AD5676_PWR_PD7(x)      (((x) & 0x3) << 14)
#define AD5676_PWR_PD6(x)      (((x) & 0x3) << 12)
#define AD5676_PWR_PD5(x)      (((x) & 0x3) << 10)
#define AD5676_PWR_PD4(x)      (((x) & 0x3) << 8)
#define AD5676_PWR_PD3(x)      (((x) & 0x3) << 6)
#define AD5676_PWR_PD2(x)      (((x) & 0x3) << 4)
#define AD5676_PWR_PD1(x)      (((x) & 0x3) << 2)
#define AD5676_PWR_PD0(x)      (((x) & 0x3) << 0)

/* AD569X_PWR_PDn(x) options(n = A, B, C, D) */
#define AD5676_PD_NORMAL       0 // 正常
#define AD5676_PD_1K           1 // 1K接地
#define AD5676_PD_3STATE       3 // 三态

/*基准电压源设置寄存器*/
#define AD5676_INT_REF_ON      0 //基准电压源开启（默认）
#define AD5676_INT_REF_OFF     1 //基准电压源关闭
void AD5676_Init(void);
void AD5676_WriteFunction(u8 selectDac,u8 writeCommand, 
                          u8 channel, 
                          u16 data);
void AD5676_InternalVoltageReference(u8 vRefMode,u8 selectDac);
u16 AD5676_ReadBack(u8 dacChannelAddr, u8 selectDac);
void AD5676_DCEN_Set(u8 dcenMode);
void AD5676_Soft_Reset(u8 selectDac);
void AD5676_Clean_Register(void);
#endif
