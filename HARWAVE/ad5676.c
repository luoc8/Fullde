#include "ad5676.h"
#include "spi2.h"
#include "delay.h"

/*******************************************************
*DAC初始化
*参数：
*返回：
********************************************************/
void AD5676_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SYNC_H;	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	
	
//	AD5676_Soft_Reset(SELECT_DA_1);
	
//	AD5676_InternalVoltageReference(AD5676_INT_REF_OFF);
#if DCEN_ON
//	AD5676_Soft_Reset(SELECT_DA_1);
	AD5676_DCEN_Set(AD5676_DCEN_ON);
//	AD5676_Soft_Reset(SELECT_DA_2);
	AD5676_DCEN_Set(AD5676_DCEN_ON);
#endif	
}
/*******************************************************
*写AD5676寄存器
*参数：registerValue 24bit寄存器值
*返回：
********************************************************/
void AD5676_SetInputRegister(u32 registerValue, u8 selectDac)
{
	u8 i;
	u8 registerWord[6] = {0,0,0,0,0,0};
	u32 registerValue1, registerValue2;
	if(selectDac == SELECT_DA_1)
	{
		registerValue1 = 0;
		registerValue2 = registerValue;
	}
	else
	{
		registerValue1 = registerValue;
		registerValue2 = 0;
	}
	registerWord[0] = (u8)(registerValue1>>16);
	registerWord[1] = (u8)(registerValue1>>8);
	registerWord[2] = (u8)registerValue1;
	registerWord[3] = (u8)(registerValue2>>16);
	registerWord[4] = (u8)(registerValue2>>8);
	registerWord[5] = (u8)registerValue2;
	SYNC_L;
	for(i = 0; i < 6; i++)
	{
		SPI2_ReadWriteByte(registerWord[i]);
	}
	SYNC_H;
}
/*******************************************************
*设置基准电压
*参数：
*返回：
********************************************************/
void AD5676_InternalVoltageReference(u8 vRefMode, u8 selectDac)
{
    AD5676_SetInputRegister(AD5676_CMD(AD5676_CMD_INT_REF_SETUP) | 
                            vRefMode, selectDac);
}
/*******************************************************
*写DAC命令
*参数：writeCommand 命令 channel 通道地址 data DAC值
*返回：
********************************************************/
void AD5676_WriteFunction(u8 selectDac,u8 writeCommand, u8 channel, u16 data)
{
    AD5676_SetInputRegister(AD5676_CMD(writeCommand) |
                            AD5676_ADDR(channel) | 
                            ((long)AD5676_DATA_BITS(data)), selectDac);
}
/*******************************************************
*读DAC通道N的值
*参数：dacChannelAddr DAC通道地址
*返回：
********************************************************/
u16 AD5676_ReadBack(unsigned char dacChannelAddr, u8 selectDac)
{
  unsigned long channelValue = 0;
  unsigned char buffer[3]    = {0,0,0};
  u8 i, temp;
//	AD5676_Clean_Register();
	AD5676_WriteFunction(selectDac,AD5676_CMD_SET_READBACK,dacChannelAddr,0x0000);//回读命令
//	delay_us(1);//需要一个延时
	i = 10;
	while(i--);
	SYNC_L;
#if DCEN_ON
	temp = (DAC_NUM - selectDac) * 3;
	for(i = 0; i < temp; i++)
	{
		SPI2_ReadWriteByte(0x00);//无操作
	}
#endif
	for(i = 0; i < 3; i++)
	{
		buffer[i] = SPI2_ReadWriteByte(0x00);//无操作
	}
	SYNC_H;
//    channelValue =(u32)(buffer[0]&0x0f)<<12|buffer[1]<<4|(buffer[2]&0xf0)>>4; //DB23-DB20无效，DB19-DB4为寄存器内容
    channelValue = (buffer[1] << 8) |buffer[2];
    return (u16)channelValue;
}
/********************************************************************
*两块以上DAC芯片以菊花链形式相连
*参数：	dcenMode 独立模式/菊花链模式
*返回：
*********************************************************************/
void AD5676_DCEN_Set(u8 dcenMode)
{
	u8 i;
	u8 buf[3];
	buf[0] = (u8)(AD5676_CMD_SET_DCEN << 4);
	buf[1] = 0;
	buf[2] = dcenMode;
	
	SYNC_L;
	for(i = 0; i < 3; i++)
	{
		SPI2_ReadWriteByte(buf[i]);
	}
	SYNC_H;
}
/********************************************************************
*软件复位
*参数：
*返回：
*********************************************************************/
void AD5676_Soft_Reset(u8 selectDac)
{
	AD5676_SetInputRegister(AD5676_CMD(AD5676_CMD_SW_RESET) | 0x0000, selectDac);
}
void AD5676_Clean_Register()
{
	u8 i;
	SYNC_L;
	for(i = 0; i < 6; i++)
	{
		SPI2_ReadWriteByte(0x00);//无操作
	}
	SYNC_H;
}
