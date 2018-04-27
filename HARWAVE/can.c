#include "can.h"
#include "delay.h"
#include "sys_def.h"

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//则波特率为:36M/((8+9+1)*4)=500Kbps
//返回值:0,初始化OK;
//其他,初始化失败; 

//CAN接收RX0中断使能
//#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.	
u8 CAN_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp)
{ 
	u32 extid = INIT_ID|DEVICE_ID;

	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  		NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//CAN单元设置
	CAN_InitStructure.CAN_TTCM = DISABLE;			//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM = ENABLE;			//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM = DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = DISABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		 	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = ENABLE;			//优先级由发送请求次序决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	        //模式设置： mode:0,普通模式;1,回环模式; 
	//设置波特率
	CAN_InitStructure.CAN_SJW = tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = tbs1; 				//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~ CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = tbs2;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~ CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        	//分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        		//初始化CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber = 0;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)((extid>>13)&0xffff);	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = (u16)((extid<<3)|CAN_ID_EXT);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)((EXT_MASK_ID>>13)&0xffff);//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)((EXT_MASK_ID<<3)|CAN_ID_EXT);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0

	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	
	CAN_FilterInitStructure.CAN_FilterNumber = 1;	//过滤器1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)((HEARTBEAT_ID>>13)&0xffff);	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = (u16)((HEARTBEAT_ID<<3)|CAN_ID_EXT);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)(((HEARTBEAT_ID|DEVICE_ID)>>13)&0xffff);//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)(((HEARTBEAT_ID|DEVICE_ID)<<3)|CAN_ID_EXT);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器1关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器1

	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_FF0|CAN_IT_FOV0,ENABLE);				//FIFO0消息挂号中断允许.		
	return 0;
} 


//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u32 extid, u8* msg, u8 len)
{	
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
//	TxMessage.StdId=id[0];			// 标准标识符 
	TxMessage.ExtId = extid;			// 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Extended; 	// 标准帧
	TxMessage.RTR = CAN_RTR_Data;		// 数据帧
	TxMessage.DLC = len;				// 要发送的数据长度
	for(i = 0;i < len; i++)
	TxMessage.Data[i] = msg[i];			          
	mbox = CAN_Transmit(CAN1, &TxMessage);
	if(mbox == 2)
	{
		i = 0;
		while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (i < 0xFFF))i++;
		if(i >= 0XFFF)return 1;
	}	
	return 0;	 
}















