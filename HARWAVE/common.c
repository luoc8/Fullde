#include "common.h"
/* 大端 ***************************************************************/

#if BIGENDIAN
	/********************************************************************
	*从数组中获取float
	*参数：	*buf	数组
	*返回：	float
	*********************************************************************/
	float Get_Float(u8 *buf)
	{
		UNION_FLOAT temp;
		
		temp.bit_data[3] = *buf; 
		temp.bit_data[2] = *(buf + 1); 
		temp.bit_data[1] = *(buf + 2); 
		temp.bit_data[0] = *(buf + 3); 
		
		return temp.flaot_data;
	}
	/********************************************************************
  *获取64位无符号长整形
  *参数：
  *返回：
  *********************************************************************/
	uint64_t Get_u64(u8 *buf)
	{
		UNION_U64 temp;
		
		temp.bit_data[7] = *buf; 
		temp.bit_data[6] = *(buf + 1); 
		temp.bit_data[5] = *(buf + 2); 
		temp.bit_data[4] = *(buf + 3);
		temp.bit_data[3] = *(buf + 4); 
		temp.bit_data[2] = *(buf + 5); 
		temp.bit_data[1] = *(buf + 6); 
		temp.bit_data[0] = *(buf + 7); 		
		
		return temp.u64_data;
	}
	/********************************************************************
	*从数组中获取u32
	*参数：	*buf	数组
	*返回：	u32
	*********************************************************************/
	u32 Get_U32(u8 *buf)
	{
		UNION_U32 temp;
		
		temp.bit_data[3] = *buf; 
		temp.bit_data[2] = *(buf + 1); 
		temp.bit_data[1] = *(buf + 2); 
		temp.bit_data[0] = *(buf + 3);
		return temp.u32_data;
//		return (((*buf)<<24)|((*(buf+1))<<16)|((*(buf+2))<<8)|(*(buf+3)));
	}
	/********************************************************************
	*从数组中获取u16
	*参数：	*buf	数组
	*返回：	u16
	*********************************************************************/
	u16 Get_U16(u8 *buf)
	{
		UNION_U16 temp;
		
		temp.bit_data[1] = *buf;
		temp.bit_data[0] = *(buf + 1);
		return temp.u16_data;
//		return ((*buf)<<8)|(*(buf+1));
	}
	/********************************************************************
  *float转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void Float_To_Byte(float float_data, u8 *buf)
	{
		UNION_FLOAT temp;
		
		temp.flaot_data = float_data;
		buf[0] = temp.bit_data[3];
		buf[1] = temp.bit_data[2];
		buf[2] = temp.bit_data[1];
		buf[3] = temp.bit_data[0];
	}
	/********************************************************************
  *u64转化成字节
  *参数：
  *返回：
  *********************************************************************/
	void U64_To_Byte(uint64_t u64_data, u8 *buf)
	{
		UNION_U64 temp;
		
		temp.u64_data = u64_data;
		buf[0] = temp.bit_data[7];
		buf[1] = temp.bit_data[6];
		buf[2] = temp.bit_data[5];
		buf[3] = temp.bit_data[4];
		buf[4] = temp.bit_data[3];
		buf[5] = temp.bit_data[2];
		buf[6] = temp.bit_data[1];
		buf[7] = temp.bit_data[0];
	}
	/********************************************************************
  *u32转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void U32_To_Byte(u32 u32_data, u8 *buf)
	{
		UNION_U32 temp;
		
		temp.u32_data = u32_data;
		buf[0] = temp.bit_data[3];
		buf[1] = temp.bit_data[2];
		buf[2] = temp.bit_data[1];
		buf[3] = temp.bit_data[0];
	}
	/********************************************************************
  *u16转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void U16_To_Byte(u16 u16_data, u8 *buf)
	{
		UNION_U16 temp;
		
		temp.u16_data = u16_data;
		buf[0] = temp.bit_data[1];
		buf[1] = temp.bit_data[0];
	}
/* 小端模式 ***********************************************************/

#else
	/********************************************************************
	*从数组中获取float
	*参数：	*buf	数组
	*返回：	float
	*********************************************************************/
	float Get_Float(u8 *buf)
	{
		UNION_FLOAT temp;
		
		temp.bit_data[0] = *buf; 
		temp.bit_data[1] = *(buf + 1); 
		temp.bit_data[2] = *(buf + 2); 
		temp.bit_data[3] = *(buf + 3); 
		
		return temp.flaot_data;
	}
	/********************************************************************
  *获取u64
  *参数：
  *返回：
  *********************************************************************/
	uint64_t Get_u64(u8 *buf)
	{
		UNION_U64 temp;
		
		temp.bit_data[0] = *buf; 
		temp.bit_data[1] = *(buf + 1); 
		temp.bit_data[2] = *(buf + 2); 
		temp.bit_data[3] = *(buf + 3);
		temp.bit_data[4] = *(buf + 4); 
		temp.bit_data[5] = *(buf + 5); 
		temp.bit_data[6] = *(buf + 6); 
		temp.bit_data[7] = *(buf + 7); 
		
		return temp.u64_data;
	}
	/********************************************************************
	*从数组中获取u32
	*参数：	*buf	数组
	*返回：	u32
	*********************************************************************/
	u32 Get_U32(u8 *buf)
	{
		UNION_U32 temp;
		
		temp.bit_data[0] = *buf; 
		temp.bit_data[1] = *(buf + 1); 
		temp.bit_data[2] = *(buf + 2); 
		temp.bit_data[3] = *(buf + 3);
		return temp.u32_data;
	}
	/********************************************************************
	*从数组中获取u16
	*参数：	*buf	数组
	*返回：	u16
	*********************************************************************/
	u16 Get_U16(u8 *buf)
	{
		UNION_U16 temp;
		
		temp.bit_data[0] = *buf;
		temp.bit_data[1] = *(buf + 1);
		return temp.u16_data;
	}
	/********************************************************************
  *float转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void Float_To_Byte(float float_data, u8 *buf)
	{
		UNION_FLOAT temp;
		
		temp.flaot_data = float_data;
		buf[0] = temp.bit_data[0];
		buf[1] = temp.bit_data[1];
		buf[2] = temp.bit_data[2];
		buf[3] = temp.bit_data[3];
	}
	/********************************************************************
  *u64转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void U64_To_Byte(uint64_t u64_data, u8 *buf)
	{
		UNION_U64 temp;
		
		temp.u64_data = u64_data;
		buf[0] = temp.bit_data[0];
		buf[1] = temp.bit_data[1];
		buf[2] = temp.bit_data[2];
		buf[3] = temp.bit_data[3];
		buf[4] = temp.bit_data[4];
		buf[5] = temp.bit_data[5];
		buf[6] = temp.bit_data[6];
		buf[7] = temp.bit_data[7];
	}
	/********************************************************************
  *u32转化成字节
  *参数：
  *返回：
  *********************************************************************/
	void U32_To_Byte(u32 u32_data, u8 *buf)
	{
		UNION_U32 temp;
		
		temp.u32_data = u32_data;
		buf[0] = temp.bit_data[0];
		buf[1] = temp.bit_data[1];
		buf[2] = temp.bit_data[2];
		buf[3] = temp.bit_data[3];
	}
	/********************************************************************
  *u16转换成字节
  *参数：
  *返回：
  *********************************************************************/
	void U16_To_Byte(u16 u16_data, u8 *buf)
	{
		UNION_U16 temp;
		
		temp.u16_data = u16_data;
		buf[0] = temp.bit_data[0];
		buf[1] = temp.bit_data[1];
	}
	
#endif



