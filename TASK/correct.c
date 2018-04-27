#include "sys_def.h"
#include "flash_if.h"

CORRECT_DEF mod_adcorrect[DEVICE_MODULE_NUM][POINT_NUM];
						
CORRECT_DEF mod_dacorrect[DEVICE_MODULE_NUM][POINT_NUM];

/********************************************************************
*从flash中读取参数
*参数：
*返回：
*********************************************************************/
static u8 Re_x_Correct(u32 address, CORRECT_DEF *correct, u32 len)
{
	u16 flag = 0x0000;
	u16 *correct_buf = (u16 *)correct;
	u32 i;
	//读取的大小判读
	if(len > SIZE_CORRECT_MEM)
	{
		return 0;
	}
	//读标志位
	flag = *(vu16*)address;
	//半字读
	len = len / 2;
	if(flag == 0xaaaa)
	{
		address += 4;
		for(i = 0; i < len ; i ++)
		{
			correct_buf[i] = *(vu16*)address;
			address += 2;
		}
		return 1;
	}
	return 0;
}
/********************************************************************
*将参数写入flash
*参数：
*返回：
*********************************************************************/
u8 Wr_x_Correct(u32 address, CORRECT_DEF *correct, u32 len)
{
	u16 flag = 0xaaaa;
	u16 *correct_buf;
	u32 i;
	
	correct_buf = (u16 *)correct;
	//数据是否超出了一页大小
	if(len  > FLASH_PAGE_SIZE - 4)
	{
		return FB_PARAMETER_ERR;
	}
	//擦除整页
	FLASH_Unlock();
	FLASH_ErasePage(address);
	//写入标志位
	FLASH_ProgramHalfWord(address, flag);
	address += 4;
	//半字写
	len = len / 2;
	for(i = 0; i < len; i++)
	{
		FLASH_ProgramHalfWord(address,correct_buf[i]);
		address += 2;
	}
	FLASH_Lock();
	return FB_SUCCESS;
}
/********************************************************************
*读取检准参数 
*参数：
*返回：
*********************************************************************/
void read_correct(void)
{
	u32 len;
	
	len = SIZE_CORRECT_MEM;
	Re_x_Correct(ADC_CORRECT_ADDRESS,&mod_adcorrect[0][0],len);
	Re_x_Correct(DAC_CORRECT_ADDRESS,&mod_dacorrect[0][0],len);
}
