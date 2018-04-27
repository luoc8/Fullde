#ifndef _COMMON_H
#define _COMMON_H
#include "stm32f10x.h"
#define BIGENDIAN 1
typedef union
{
	float flaot_data;
	u8 bit_data[4];
}UNION_FLOAT;

typedef union
{
	uint64_t u64_data;
	u8 bit_data[8];
}UNION_U64;

typedef union
{
	u32 u32_data;
	u8 bit_data[4];
}UNION_U32;

typedef union
{
	u16 u16_data;
	u8 bit_data[2];
}UNION_U16;

u16 Get_U16(u8 *buf);
u32 Get_U32(u8 *buf);
float Get_Float(u8 *buf);
uint64_t Get_U64(u8 *buf);
void Float_To_Byte(float float_data, u8 *buf);
void U64_To_Byte(uint64_t u64_data, u8 *buf);
void U32_To_Byte(u32 u32_data, u8 *buf);
void U16_To_Byte(u16 u16_data, u8 *buf);

#endif
