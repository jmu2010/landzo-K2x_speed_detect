#ifndef _LANDZO_ULTRASONIC_H
#define _LANDZO_ULTRASONIC_H

void   Ultrasonic_Init();          //��������ʼ��
void   Ultrasonic_Delay(uint32 n);    //��������ʱus������
void   Ultrasonic_Trig();          //���������ͺ���
float  Ultrasonic_Compute(uint32 us);       //������������뺯��
#endif


