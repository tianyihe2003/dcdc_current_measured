#include "time.h"
#include "adc.h"
#include "pid.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "key.h"
#include "io.h"
#include "usart.h"
#include "main.h"

int Out_Pwm;
extern int k;

void TIM2_Int_Init(u16 arr,u16 PSC)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
  
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 开启定时器更新中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// 使能定时器
	TIM_Cmd(TIM2, ENABLE);	
							 
}



void TIM2_IRQHandler(void)   //TIM2中断
{  
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{  
		
		
			
		static int T=0;
		const int up=1;
			//if(T==0 || T==199)
					{						
						#if 0
						DC_I0ltmeter=0.00435494*k-13.0514;        //电流    
						#endif
						//DC_I0ltmeter=0.00435494*k-13.1514;        //电流    
					//if(DC_I0ltmeter<0) DC_I0ltmeter=-DC_I0ltmeter;
						
						
						//DC_v0ltmeter=0.008373873394077*First_order_filter(1,30)+0.170973159674584+0.70;	            //电压	    	
						#if 0
						AC_MAX=Incremental_PID_PWM(0,DC_I0ltmeter,0.4,period);
						
						#else
						AC_MAX=0.001*Incremental_PID_PWM(0,k*10,(3053+22.962424*3)*10 ,period);
						#endif
					}
//LED1=1;	
    			Out_Pwm=(int)((AC_MAX)+period*0.5);     //+1159
					printf("Out_Pwm:%d\n",Out_Pwm);
			//printf("Out_Pwm1:%d\n",Out_Pwm) 	;
				 
				if( Out_Pwm> 0.8*period)          
				 {		 
					 Out_Pwm = (int)(0.7*period);
					
				 }
					else if( Out_Pwm < 0.2*period)		
				 {		
					 Out_Pwm =(int)( 0.3*period);
				}
					
				  TIM_SetCompare1(TIM1,Out_Pwm);
					
			#if 1
						printf("Out_Pwm:%d\n",Out_Pwm) 			;
						printf("AC_MAX:%d\n",AC_MAX) 			;
						printf("PERIOD %d\n",(int)(0.5*period) ) 			;
						printf("电流测量值%d\n",k);
						printf("电流计算值%f\n\n\n",DC_I0ltmeter);
			#endif
				
				T++;
				T%=400;
			delay_ms(1000);
				
}				 
     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源   
			
		
}



  
