#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h" 
#define N 15 //ÿͨ����15��
#define M 4//6ͨ��
#define ADC1_DR_ADDRESS    ((u32)ADC1+0x4c)
void  Adc1_Init(void);//adc��ʼ������

int ADC_Value(u8 Channel);	
int Adfilter(u8 Channel); 
uint16_t ADC_Filter(u8 Channel);		//��λֵƽ���˲������ֳƷ��������ƽ���˲�����
int Rms_voltage( int *arr,int data_len );
int First_order_filter(u8 Channel,u8 coefficient);   //һ�׵�ͨ�˲���


#endif
