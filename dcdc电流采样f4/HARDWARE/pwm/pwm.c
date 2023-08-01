#include "pwm.h"
#include "math.h"

 /**************************************************

�������ܣ����ö�ʱ��Ϊpwm�������
���ţ�PA7����CH1N  PA8  CHI
���룺arr���Զ���װ�ؼĴ�����ֵ,psc: ʱ��Ԥ��Ƶϵ��
�������
ֻ�и߼���ʱ�����ܻ������TIM1,TIM8   �� CH1 CH1N
һ��ͨ�������������ӳ�䣬
Ҫ��TIM1ʱ��
***************************************************/

void TIM1_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    //��һ��������ʱ��        
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    

  //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);          //�����ض���
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOE8����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOE8����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_TIM1); //GPIOE8����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);    
	
	//�ڶ���������goio��	
    /********TIM1_CH1 ��������*********/        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;           //GPIOE8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	//AF�Ǹ��õ���˼
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE8
	
    
    //����������ʱ����������        
    TIM_TimeBaseStructure.TIM_Period=arr;             // �Զ���װ�ؼĴ�����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler=psc;            // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�                       
	  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
   //���Ĳ�pwm�������
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;                    //����Ϊpwm2���ģʽ
    TIM_OCInitStructure.TIM_Pulse=(int)(arr*0.5);                                 //����ռ�ձ�ʱ��  ��1900��Ϊ0
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;            //�����������
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //ʹ�ܸ�ͨ�����
    //���漸�������Ǹ߼���ʱ���Ż��õ���ͨ�ö�ʱ����������
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;        //���û������������
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;   //ʹ�ܻ��������
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //���������״̬
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;     //�����󻥲������״̬
    TIM_OC1Init(TIM1,&TIM_OCInitStructure);                       //����ָ��������ʼ��
    
	//���岽��������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//����ģʽ�����ѡ�� 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;    //��������
    TIM_BDTRInitStructure.TIM_DeadTime =15;// 0xf;                    //����ʱ������0.1us  //����ʱ��Ϊ168M TdtgΪ65ns  
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;              //ɲ������ʹ��
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//ɲ�����뼫��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);        


   //��������ʹ�ܶ˵Ĵ�
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
    
		TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
    TIM_Cmd(TIM1,ENABLE);                           //��TIM1

    //��������Ǹ߼���ʱ�����еģ����pwm�����
    TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm���ʹ�ܣ�һ��Ҫ�ǵô�
}

extern double Sine12bit[400];        //�������ɵ����Ҳ���
void SineWave_Data(u16 cycle ,double *D,float Um)
{
 uint16_t i;
    for( i=0;i<cycle;i++)
    {
		  D[i]=(double)(Um*sin((1.0*i/cycle)*2*PI));   //������ɢ����ʽ
    }
}

