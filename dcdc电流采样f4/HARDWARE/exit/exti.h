#ifndef __EXIT_H	
#define __EXIT_H	 
#include "sys.h"
extern  float Kp,Ki,Kd;

void EXTIX_Init(void);           //�ⲿ�жϳ�ʼ��	
void EXTI4_IRQHandler(void);     //�ⲿ�жϷ������
void EXTI9_5_IRQHandler(void);   //�ⲿ�жϷ������
void EXTI15_10_IRQHandler(void); //�ⲿ�жϷ������

#endif



