#include  "exti.h"
#include "key.h"
#include "delay.h"
#include "pid.h"
#include "lcd.h"
#include "io.h"
#include "time.h"
float Kp=50,Ki=5,Kd=0;

#if 0
void EXTIX_Init(void)
{
 
	
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    //GPIOC.4 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOC.5	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOC.9  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   //GPIOC.10	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOC.11	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		 //GPIOC.12	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12); 
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
 
}
#else
void EXTIX_Init(void)
{
	//��һ������ʼ��GPIO
	KEY_Init();	 //	�����˿ڳ�ʼ��
	
	//�ڶ�������ʼ��EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	//���ж�Դ��KEY��������
	//ʹ��PE2��3��4��5������Ϊkey
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0--��ӦK1����
	
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���   
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	//������������NVIC
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
}

#endif

//�ⲿ�ж�4������� 
void EXTI4_IRQHandler(void)     //PC4
{
	delay_ms(40);
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
		{
			
     Kp = Kp + 0.5f;
		// LCD_ShowFloatNum3(18,12, Kp,5,3,12);
			LCD_ShowFloatNum3(20,80,Kp,4,RED,WHITE,16);
		}
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE8�ϵ��жϱ�־λ  
} 

//�ⲿ�ж�9_5������� 
void EXTI9_5_IRQHandler(void)
{
	delay_ms(20);
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
		{
		
	   Kp = Kp - 0.5f;
		LCD_ShowFloatNum3(20,80,Kp,4,RED,WHITE,16);
		}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
		{
			
	   Ki = Ki + 0.5f;
		LCD_ShowFloatNum3(20,80,Ki,4,RED,WHITE,16);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE8�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line9);  //���LINE8�ϵ��жϱ�־λ  	
} 

//�ⲿ�ж�1������� 
void EXTI15_10_IRQHandler(void)
{
	delay_ms(20);
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
		{
	   Ki = Ki - 0.5f;
		LCD_ShowFloatNum3(20,80,Ki,4,RED,WHITE,16);			
		}
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
		{
		 Kd = Kd +0.5f;
		LCD_ShowFloatNum3(20,80,Kd,4,RED,WHITE,16);			
		}
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
		{
	
		 Kd = Kd - 0.5f;
		LCD_ShowFloatNum3(20,80,Kd,4,RED,WHITE,16);		}
	EXTI_ClearITPendingBit(EXTI_Line10);  //���LINE8�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line11);  //���LINE8�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line12);  //���LINE8�ϵ��жϱ�־λ  
} 






