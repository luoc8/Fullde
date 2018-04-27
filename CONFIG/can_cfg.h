#ifndef _CAN_CONFIG_H
#define _CAN_CONFIG_H
typedef struct
{
	u32 special_page			:8;					//特殊参数也
	u32 device_id				:8;					//设备id
	u32 parameter_page			:8;					//参数页
	u32 idn						:5;					//指令标识
	u32 null					:3;					//CANid域只有29位
}CAN_EXTID;
typedef union
{
	CAN_EXTID			mix_extid;
	u32						extid;
}UNION_EXTID;

/*************************接收到的帧分类*********************************/
#define IDN_CMD   			0x01				//控制
#define IDN_PROTECT_CFG		0x02				//保护参数
#define IDN_HEARTBEAT 		0x03				//心跳
#define IDN_FAULT 			0x04				//故障处理
#define IDN_CONFIG			0x05				//控制参数
#define IDN_DATA_REQUEST  	0x06				//数据请求

#define IDN_CRT_CMD			0x11				//校准控制指令
#define IDN_CRT_DATA		0x16				//校准采样数据
#define IDN_RE_AD_CFG  		0x17				//读取AD矫正系数
#define IDN_WR_AD_CFG		0x18				//写AD矫正系数
#define IDN_RE_DA_CFG  		0x19				//读取DA矫正系数
#define IDN_WR_DA_CFG		0x1a				//写DA矫正系数

/********************bxCAN过滤器用宏定义*********************************/
#define INIT_ID         	0x00000000u     	//初始化ID
#define EXT_MASK_ID     	0x000000ffu      	//屏蔽位
#define HEARTBEAT_ID    	0x03004f00u     	//心跳包ID


/*************************返回的帧分类***********************************/
#define ID_DATA         	0x0600004fu     	//数据帧
#define ID_FEEDBACK     	0x0000004fu     	//反馈帧
#define ID_FAULT        	0x0400004fu     	//故障帧
#define ID_HEARTBEAT    	0x0300004fu     	//心跳帧


/***************************控制/状态************************************/
#define RUN          		0x01           		//运行
#define PAUSE        		0x02           		//暂停
#define CONTINUE     		0x03           		//继续
#define STOP         		0x04           		//停止
#define CONNECT				0x05
#define DISCONNECT			0x06
#define FAULT        		0x05           		//故障
#define CLEAR_FAULT  		0x07           		//清楚故障

/***************************反馈***************************************/           
#define FB_SUCCESS        	0xff           		//成功
#define FB_PARAMETER_ERR 	0x01           		//参数错误
#define FB_CRC_ERR        	0x02           		//校验失败
#define FB_MODE_ERR      	0x03           		//模式不符
#define FB_W_DAC_ERR      	0x04           		//写DAC失败
#define FB_CONNECT_ERR    	0x05           		//并联失败
#define FB_NO_THIS_CH		0x06				//没有这个通道
#define FB_NO_THIS_MODE		0x07				//没有这种模式
#define	FB_NO_THIS_CMD		0x08				//没有这种命令
#define FB_DATA_LEN_ERR		0x09				//数据长度不对
#define FB_OTHER_ERR      	0xaa           		//其他原因

/************************充放电模式************************************/
#define PROTECT_CFG   		0x00           		//保护参数设置
#define CC_MODE       		0x01          		//恒流充电
#define CV_MODE       		0x02          		//恒压充电
#define CVC_MODE      		0x03          		//恒压恒流充电
#define DCC_MODE      		0x04          		//恒流放电
#define DCP_MODE      		0x05          		//恒功率放电
#define DCV_MODE      		0x06          		//恒压放电
#define STEWING       		0x07          		//搁置

/************************* 校准模式 ***********************************/
#define AD_CRT_MODE			0x08				//AD充电校准
#define AD_DCRT_MODE		0x09				//AD放电校准
#define DA_CRT_MODE			0x0a				//DA充电校准
#define DA_DCRT_MODE		0x0b				//DA放电校准

#endif
