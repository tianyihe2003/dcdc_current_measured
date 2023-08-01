#include "exti.h"
#include "pid.h"
#include "usart.h"



/*************************************************************************
�������ܣ�����PID������
��ڲ���:PID��ţ�����ֵ��Ŀ��ֵ
����  ֵ��������
��������ʽ��ɢPID��ʽ 
dac+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
Result�����������
Dac+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/


int Incremental_PID_PWM(int Number,float Measured,float Target,int period)
{ 	float Kp,Ki,Kd;
	switch(3)
		{ case 1: Kp=20,Ki=0,Kd=0;	break;
			case 2:Kp=50,Ki=1.5,Kd=0;	break;
			case 3:Kp=30,Ki=1,Kd=0;	break;
			case 4:Kp=10,Ki=0,Kd=0;	break;
			case 5:Kp=5,Ki=0,Kd=0;	break;
		}
	 static float Bias,Last_bias[Quantity],Previous_bias[Quantity],Result[Quantity],sum_error=0;
	 Bias = Target - Measured;                     //����ƫ��
		sum_error+=Bias;
//	 Result[Number] += Kp*(Bias-Last_bias[Number])
//	 +Ki*Bias+Kd*(Bias - 2*Last_bias[Number] + Previous_bias[Number]);  //����ʽPID������
		
		Result[Number] += Kp*(Bias) +Ki*sum_error +Kd*(Bias -Previous_bias[Number] );  //����ʽPID������
	
	 Previous_bias[Number]=Last_bias[Number];        //�����ϴ�ƫ��
	 Last_bias[Number]=Bias;	                   //���汾��ƫ�� 
		
//	 if( Result[Number]>= 0.9*period) Result[Number] = (int)(0.9*period);
//	 else if( Result[Number]<= 0.1*period)  Result[Number] = 0.1*period;
		printf("target  %f\n",		Target);
		printf("measured  %f\n",	Measured);
		printf(" bias  %f \n",Bias);
		printf("sum_error  %f\n",sum_error);
		printf("last bias  %f\n",Last_bias[Number]);
		printf("previous bias  %f\n",Previous_bias[Number]);
		printf("result %f\n",Result[Number]);
		
	 return Result[Number]; 
    
}




