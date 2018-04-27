#include "delay.h"
#include "rtx_os.h"
static u8  fac_us=0;							//us延时倍乘数			   
#define delay_osschedlock()		osKernelLock()
#define delay_osschedunlock()	osKernelUnlock()
void delay_us(u32 nus)//使用定时器4，需要先初始化定时器4
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=TIM2->ARR;					//LOAD的值	
	fac_us=10000/1000;	
	ticks=nus*fac_us; 							//需要的节拍数	  		 
	tcnt=0;
//	delay_osschedlock();						//阻止OS调度，防止打断us延时
	told=TIM2->CNT;        					//刚进入时的计数器值
	while(1)
	{
		tnow=TIM2->CNT;	
		if(tnow!=told)
		{	    
//			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
			if(tnow>told)tcnt+=tnow-told;
			else tcnt+=reload-told+tnow;
			told=tnow;
			if(tcnt>=ticks)break;				
		}  
	}
//	delay_osschedunlock();						//恢复OS调度									    
}
//延时nms
//nms:要延时的ms数
//void delay_ms(u16 nms)
//{	
//	if(delay_osrunning&&delay_osintnesting==0)	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
//	{		 
//		if(nms>=fac_ms)							//延时的时间大于OS的最少时间周期 
//		{ 
//   			delay_ostimedly(nms/fac_ms);		//OS延时
//		}
//		nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
//	}
//	delay_us((u32)(nms*1000));					//普通方式延时  
//}







































