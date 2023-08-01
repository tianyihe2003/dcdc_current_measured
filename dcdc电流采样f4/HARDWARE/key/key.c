#include "stm32f4xx.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	//PC4,C5,C9,C1O,C11,C12	���ó���������
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_2|GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //PC4,C5,C9,C1O,C11,C12	���ó������������ó����룬Ĭ������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��PC4,C5,C9,C1O,C11,C12	���ó���������
  
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;

//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(5);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
		else if(KEY5==0)return 5;
		else if(KEY6==0)return 6;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)key_up=1; 	    
 	return 0;// �ް�������
}

void LED2_INIT(void )
{

 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	//PC4,C5,C9,C1O,C11,C12	���ó���������
	GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT ; //PC4,C5,C9,C1O,C11,C12	���ó������������ó����룬Ĭ������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PC4,C5,C9,C1O,C11,C12	���ó���������
 #if 1
GPIO_SetBits(GPIOB,GPIO_Pin_0 );	
	#else
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 );	
	#endif
}



