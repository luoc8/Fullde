#include "LTC1867.h"
#include "delay.h"
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/6/7复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收16位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设	
}
//SPI1 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1, ENABLE); 
}
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint16_t SPI1_ReadWriteByte(uint16_t TxData)
{						 	
	u16 retry = 0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}
//初始化LTC1867
void Ltc1867_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // PA4 推挽SPI1 NSS 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	LTC1867_CS_H;				//ADC不选中
	SPI1_Init();		   	//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
}
//单通道读取
//adc读取的数据
//adcCommand命令
uint16_t Ltc1867_Read(uint16_t adcCommand)
{
	uint16_t adc=0;
	LTC1867_CS_H;
	delay_us(5);
	LTC1867_CS_L;
	adc=SPI1_ReadWriteByte(adcCommand<<8);
//	LTC1867_CS_H;
	return adc;
}
//ADC睡眠模式选择
//LTC1867_SLEEP_MODE      //睡眠模式
//LTC1867_EXIT_SLEEP_MODE //退出睡眠模式
//LTC1867_UNIPOLAR_MODE    //单极性 
//LTC1867_BIPOLAR_MODE    //双极性
void Ltc1867_Mode(uint16_t adcCommand)
{
	LTC1867_CS_L;
	SPI1_ReadWriteByte(adcCommand<<8);
	delay_us(5);
	LTC1867_CS_H;	
}
//static float LTC1867_lsb = 6.25009537E-5;               //!< Ideal LSB voltage for a perfect part
//static int32_t LTC1867_offset_unipolar_code = 0;        //!< Ideal unipolar offset for a perfect part
//static int32_t LTC1867_offset_bipolar_code = 0;         //!< Ideal bipolar offset for a perfect part
//// Calculates the LTC1867 input's bipolar voltage given the two's compliment data and lsb weight
//float LTC1867_bipolar_code_to_voltage(uint16_t adc_code, float LTC1867_lsb, int32_t LTC1867_offset_bipolar_code)
//{
//  float adc_voltage, sign = 1.0;
//  if (adc_code>>15)
//  {
//    adc_code = (adc_code ^ 0xFFFF)+1;                                           //! 1) Convert ADC code from two's complement to binary
//    sign = -1;
//  }
//  adc_voltage=((float)(adc_code+LTC1867_offset_bipolar_code))*LTC1867_lsb*sign; //! 2) Calculate voltage from ADC code, lsb, offset.
//  return(adc_voltage);
//}

//// Calibrate the lsb
//void LTC1867_cal_voltage(uint16_t zero_unipolar_code, uint16_t zero_bipolar_code, 
//	uint16_t fs_code, float zero_voltage, float fs_voltage, float *LTC1867_lsb, 
//		int32_t *LTC1867_offset_unipolar_code, int32_t *LTC1867_offset_bipolar_code)
//{
//  float temp_offset;
//  *LTC1867_lsb = (fs_voltage-zero_voltage)/((float)(fs_code - zero_unipolar_code));                     //! 1) Calculate the LSB
//  
//  temp_offset = (zero_voltage/ *LTC1867_lsb) - zero_unipolar_code;                                      //! 2) Calculate Unipolar offset
//  temp_offset = (temp_offset > (floor(temp_offset) + 0.5)) ? ceil(temp_offset) : floor(temp_offset);    //! 3) Round
//  *LTC1867_offset_unipolar_code = (int32_t)temp_offset;                                                 //! 4) Cast as int32_t
//  
//  temp_offset = (zero_voltage / *LTC1867_lsb) - zero_bipolar_code ;                                     //! 5) Calculate Bipolar offset
//  temp_offset = (temp_offset > (floor(temp_offset) + 0.5)) ? ceil(temp_offset) : floor(temp_offset);    //! 6) Round
//  *LTC1867_offset_bipolar_code = (int32_t)temp_offset;                                                  //! 7) cast as int32_t
//}
