#include "adc.h"
#include "math.h"



vu16 AD_Value[N][M]; //�������ADC�Ľ�� Ҳ��DMA��Ŀ���ַ
vu16 After_filter[M]; //�������ƽ��ֵ֮��Ľ��

//��ʼ��ADC
//����ADC1��ͨ��0~7,��Ӧ��ϵ����
//ADC1_CH0-->PA0	
//ADC1_CH1-->PA1	
//ADC1_CH2-->PA2
//ADC1_CH3-->PA3	
//ADC1_CH4-->PA4	
//ADC1_CH5-->PA5	
//ADC1_CH6-->PA6
//ADC1_CH7-->PA7	

void  Adc1_Init(void)
{ 


  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	 DMA_InitTypeDef       DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

	 /* DMA2 Stream0 channel0 ���� **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;       //������2��ͨ��0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;  										//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //���赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = N*M;    //����4λ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //����������Ĵ���
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //�ڴ������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����ʽ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   //�ڴ��ʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //ѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;    //�����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         //����fifo
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;  //����ͻ��ģʽ
/*DMA����Ϊ�˳����������ݵģ����۵���DMA ��ͻ��DMA����ͬ����ÿ�δ���һ����Ԫ���ݣ�����ʹ��32bit����λ��ÿ��ԪΪ32bit������ʹ��8bit����λ��ÿ��ԪΪ8bit�����õ�ʱ������ͻ��DMA��ÿ��ʱ�Ӷ�����һ����Ԫ���ݵ��������䣻������DMA���ǣ���Ҫ����ʱ�ӲŴ���һ�����ݵ�Ԫ��*/        
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ��ģʽ
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);


//Additionalfunctions�����ӣ��������ܣ����ű����ӵ�����ģ��ʹ�ã�ʹ��ʱֱ����ͨ���ü��ɣ�
//����ADC�Ĳ�������ͨ��������Ϊģ�����룬���õ�����GPIO_PinAFConfig�������ã�ֱ��������������ʹ��ANģʽ��
	//����8��ADͨ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//|GPIO_Pin_4|GPIO_Pin_5;//PA4 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  	 
 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//���������׶�֮����ӳ�5��ʱ��
 ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
 //ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
     /* ADC1 ��ʼ�� ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;    //����ɨ��ģʽ����ͨ���ؿ�
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//û���ⲿ���������������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�Ҷ���
    ADC_InitStructure.ADC_NbrOfConversion = M;// yuan 2���ĸ�ͨ��
    ADC_Init(ADC1, &ADC_InitStructure);

    //ת�������ת��ʱ��
		//AD_Value��0-2Ϊ���������ѹ
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);  //ADC10��ӦAD[0]��Ӧp5  	//PC0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_15Cycles);  //ADC11��ӦAD[1]��Ӧp6		//PC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_15Cycles);													//PC2
		//AD_Value��3-5Ϊ�����������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_3Cycles);													//PC3
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_3Cycles);													//PC4
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_3Cycles);													//PC5
//    
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  7, ADC_SampleTime_3Cycles);  //ADC0��ӦAD[6]��Ӧp1 	 	//PA0
//	  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  8, ADC_SampleTime_3Cycles);  //ADC1��ӦAD[7]��Ӧp2		//PA1
 
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
  ADC_SoftwareStartConv(ADC1);
	

}


	
	


//��ȡͨ����ѹֵ
int ADC_Value(u8 Channel)	 //Channelѡ��ADCͨ��
{
	unsigned int sum=0,temp;
	u8  number; 				   
	for ( number=0;number<N;number++)	   	            //ÿͨ����N��		
	{  	
		temp=AD_Value[number][Channel];			//ÿһ����һ�����ݣ��������ͨ��
		sum += temp;			//ͨ��Channel	  	
	}  		   		
	After_filter[Channel]=sum/N;		
	sum=0;	   
	return After_filter[Channel];   //3.3V�ĵ�ƽ�������Ч��ƽ	  
}
//
int Adfilter(u8 Channel)    
{
	unsigned int num=0;
	u8 i;
	for(i=0;i<100;i++)
	{
		  num+= ADC_Value(Channel);
	}
	i=0;
	return (u16)(num/100);
}



/*******************************************************************************************
*��������:int filter(u8 Channel)    //��λֵƽ���˲������ֳƷ��������ƽ���˲�����
*��ڲ���:u8 Channel                //ѡ��ADCͨ��
*����ֵ:  (sum/(N-2))               //�˲�����
*��������:�൱�ڡ���λֵ�˲�����+������ƽ���˲�����
*����˵��:�������� N �����ݣ�ȥ��һ�����ֵ��һ����Сֵ
*         Ȼ����� N-2 �����ݵ�����ƽ��ֵ
*         N ֵ��ѡȡ��3~14
*******************************************************************************************/
uint16_t ADC_Filter(u8 Channel)		//��λֵƽ���˲������ֳƷ��������ƽ���˲�����
{
	uint8_t count,i,j;
	uint32_t sum=0;
	uint16_t temp=0;
    uint16_t tempbuf[N];
    for(i=0;i<N;i++)
    {
        tempbuf[i] = AD_Value[i][Channel];
    }
	for (j=0;j<N;j++)     //ð�ݷ�
	{
		for (i=j;i<N;i++)
		{
			if ( tempbuf[i]>tempbuf[j])
			{
				temp = tempbuf[i];
				tempbuf[i] = tempbuf[j];
				tempbuf[j] = temp;
			}
		}
	}
	for ( count=2;count<N-2;count++) 
		sum = sum + tempbuf[count];
	return (sum/(N-4));
}


/*****************************************************************************
*��������:int Rms_voltage( int *arr,int data_len )
*��ڲ���:int *arr               //��������ָ��
*         int data_len           //���ݳ���
*����ֵ:  rms                    //������ֵ
*��������:ȡ������ֵ
*****************************************************************************/
int Rms_voltage( int *arr,int data_len )
{
	static int sum=0,rms=0;
	int i;
	for(i=0;i<data_len;i++)
	{
		sum+=arr[i]*arr[i];         //����ƽ����
	}
	rms=(int)sqrt(sum/data_len);  //ƽ����ȡƽ�����ٿ���
	sum=0;        
	return rms;
}



/*************************************************************************************************************
*��������:int First_order_filter(u8 Channel,u8 coefficient)   //һ�׵�ͨ�˲������ֳ�һ�׹����˲�����
*��ڲ���:u8 Channel                                    //ѡ��ADCͨ��
*         u8 coefficien                                //�˲����˲�ϵ��   ȡֵ0--100
*����ֵ:  filtered_data                                //�˲�����
*��������:һ�׵�ͨ�˲�
*����˵��:Y(n)=aX(n)+(1-a)Y(n-1)
*				 X(n)Ϊ���β���ֵ
*				 Y(n-1)Ϊ�ϴ��˲����ֵ
*				 Y(n)Ϊ�����˲����ֵ
*        һ�׵�ͨ�˲������ñ��β���ֵ���ϴ��˲����ֵ���м�Ȩ���õ���Ч�˲�ֵ��ʹ������������з�������
*************************************************************************************************************/
int First_order_filter(u8 Channel,u8 coefficient)    //һ�׵�ͨ�˲������ֳ�һ�׹����˲�����
{
	static int filtered_data[16],data_collection[16];
    
  data_collection[Channel]=ADC_Filter(Channel);            //��ȡADC�ɼ�ֵ      
	if(data_collection[Channel] < filtered_data[Channel])
	{
		filtered_data[Channel]=filtered_data[Channel] - (filtered_data[Channel] - data_collection[Channel])*coefficient/100;
	}
	else 
	{
		filtered_data[Channel]=filtered_data[Channel] + (data_collection[Channel] - filtered_data[Channel])*coefficient/100;
	}
	return filtered_data[Channel];
}




