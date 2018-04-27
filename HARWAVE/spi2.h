#ifndef _SPI2_H
#define _SPI2_H
#include "stm32f10x.h"
//void SPI2_Write(u8 *buf,u8 len);
//void SPI2_Read(u8 *buf,u8 len);
void SPI2_Init(void);
u8 SPI2_ReadWriteByte(u8 TxData);
#endif
