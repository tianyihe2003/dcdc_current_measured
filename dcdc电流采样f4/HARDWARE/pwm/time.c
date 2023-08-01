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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
  
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// ������ʱ�������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2, ENABLE);	
							 
}



void TIM2_IRQHandler(void)   //TIM2�ж�
{  
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{  
		
		
			
		static int T=0;
		const int up=1;
			//if(T==0 || T==199)
					{						
						#if 0
						DC_I0ltmeter=0.00435494*k-13.0514;        //����    
						#endif
						//DC_I0ltmeter=0.00435494*k-13.1514;        //����    
					//if(DC_I0ltmeter<0) DC_I0ltmeter=-DC_I0ltmeter;
						
						
						//DC_v0ltmeter=0.008373873394077*First_order_filter(1,30)+0.170973159674584+0.70;	            //��ѹ	    	
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
						printf("��������ֵ%d\n",k);
						printf("��������ֵ%f\n\n\n",DC_I0ltmeter);
			#endif
				
				T++;
				T%=400;
			delay_ms(1000);
				
}				 
     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ   
			
		
}



  
