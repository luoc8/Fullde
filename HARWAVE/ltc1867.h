﻿/*!
LTC1867: 16-bit 8-channel 200ksps ADC

@verbatim

The LTC1863/LTC1867 are pin-compatible, 8-channel 12-/16-bit A/D converters 
with serial I/O, and an internal reference. The ADCs typically draw only 1.3mA 
from a single 5V supply. The 8-channel input multiplexer can be configured for 
either single-ended or differential inputs and unipolar or bipolar conversions 
(or combinations thereof). The automatic nap and sleep modes benefit power 
sensitive applications. 

The LTC1867's DC performance is outstanding with a +/-2LSB INL specification and 
no missing codes over temperature. The signal-to-noise ratio (SNR) for the 
LTC1867 is typically 89dB, with the internal reference. 

SPI DATA FORMAT (MSB First):

            Byte #1                           Byte #2
Data Out :  D15 D14 D13 D12 D11 D10 D9  D8    D7  D6  D5  D4  D3  D2  D1  D0
Data In  :  SD  OS  S1  S0  COM UNI SLP X     X   X   X   X   X   X   X   X

SD   : Single/Differential Bit
OS   : ODD/Sign Bit
Sx   : Address Select Bit
COM  : CH7/COM Configuration Bit
UNI  : Unipolar/Bipolar Bit
SLP  : Sleep Mode Bit
Dx   : Data Bits
X    : Don't care


Example Code:

Read Channel 0 in Single-Ended Unipolar mode when input is with respect to GND

    adc_command = LTC1867_CH0 | LTC1867_UNIPOLAR_MODE;  // Build ADC command for channel 0
    LTC1867_read(LTC1867_CS, adc_command, &adc_code);   // Throws out last reading
    LTC1867_read(LTC1867_CS, adc_command, &adc_code);   // Obtains the current reading and stores to adc_code variable

    // Convert adc_code to voltage
    adc_voltage = LTC1867_unipolar_code_to_voltage(adc_code, LTC1867_lsb, LTC1867_offset_unipolar_code);

@endverbatim

http://www.linear.com/product/LTC1867

http://www.linear.com/product/LTC1867#demoboards

REVISION HISTORY
$Revision: 2026 $
$Date: 2013-10-14 13:52:48 -0700 (Mon, 14 Oct 2013) $

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

/*! @file
    @ingroup LTC1867
    Header for LTC1867: 16-bit 8-channel 200ksps ADC
*/

#ifndef __LTC1867_H
#define __LTC1867_H 			   
#include "stm32f10x.h" 


//! Define the SPI CS pin
//#ifndef LTC1867_CS
#define LTC1867_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)//PA4 LTC1867片选信号
#define LTC1867_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_4)
//#endif

//! @name LTC1867 Single-Ended Channel Addresses, COM=GND
//! @{
// Single-Ended Channel Address When CH7/COM Pin Is used As CH7
//  adc_command = LTC1867_P0_N1 | LTC1867_UNIPOLAR_MODE;     // Differential Unipolar Mode with CH0 as positive and CH1 as negative.
//直接做了运算
//#define LTC1867_CH0             0x80
//#define LTC1867_CH1             0xC0
//#define LTC1867_CH2             0x90
//#define LTC1867_CH3             0xD0
//#define LTC1867_CH4             0xA0
//#define LTC1867_CH5             0xE0
//#define LTC1867_CH6             0xB0
//#define LTC1867_CH7             0xF0
#define LTC1867_CH0             0x84
#define LTC1867_CH1             0xC4
#define LTC1867_CH2             0x94
#define LTC1867_CH3             0xD4
#define LTC1867_CH4             0xA4
#define LTC1867_CH5             0xE4
#define LTC1867_CH6             0xB4
#define LTC1867_CH7             0xF4
//!@}

//! @name LTC1867 Differential Channel Addresses
//! @{
// Differential Channel Address When CH7/COM Pin Is Used As CH7
#define LTC1867_P0_N1           0x00
#define LTC1867_P1_N0           0x40

#define LTC1867_P2_N3           0x10
#define LTC1867_P3_N2           0x50

#define LTC1867_P4_N5           0x20
#define LTC1867_P5_N4           0x60

#define LTC1867_P6_N7           0x30
#define LTC1867_P7_N6           0x70
//!@}

//! @name LTC1867 Single-Ended Channel Addresses, COM=CH7
//! @{
// Channel Address When CH7/COM Pin Is Used As Common
#define LTC1867_CH0_7COM        0x88
#define LTC1867_CH1_7COM        0xC8
#define LTC1867_CH2_7COM        0x98
#define LTC1867_CH3_7COM        0xD8
#define LTC1867_CH4_7COM        0xA8
#define LTC1867_CH5_7COM        0xE8
#define LTC1867_CH6_7COM        0xB8
//!@}

//! @name LTC1867 Sleep / Unipolar/Bipolar config bits
//! @{
// Sleep Mode Command
#define LTC1867_SLEEP_MODE      0x02
#define LTC1867_EXIT_SLEEP_MODE 0x00
#define LTC1867_UNIPOLAR_MODE   0x04  // Bitwise OR with channel commands for unipolar mode
#define LTC1867_BIPOLAR_MODE    0x00
//!@}

/*
  Example command
  adc_command = LTC1867_P0_N1;                             // Differential Bipolar Mode with CH0 as positive and CH1 as negative.
  adc_command = LTC1867_P0_N1 | LTC1867_UNIPOLAR_MODE;     // Differential Unipolar Mode with CH0 as positive and CH1 as negative.
 */
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
uint16_t SPI1_ReadWriteByte(uint16_t TxData);
void Ltc1867_Init(void);
uint16_t Ltc1867_Read(uint16_t adcCommand);
void Ltc1867_Mode(uint16_t adcCommand);
//float LTC1867_bipolar_code_to_voltage(uint16_t adc_code, float LTC1867_lsb, int32_t LTC1867_offset_bipolar_code);
//void LTC1867_cal_voltage(uint16_t zero_unipolar_code, uint16_t zero_bipolar_code, 
//	uint16_t fs_code, float zero_voltage, float fs_voltage, float *LTC1867_lsb, 
//		int32_t *LTC1867_offset_unipolar_code, int32_t *LTC1867_offset_bipolar_code);
#endif



