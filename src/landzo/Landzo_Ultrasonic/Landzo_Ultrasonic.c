#include "include.h"
#include "KL2x_delay.h"

#define   TX     PTE31_OUT


void Ultrasonic_Init()
{
    gpio_init(PTE31,GPO,0);                     //��ʼ��TX�˿�
    gpio_Interrupt_init(PTA12, GPI_DOWN, RING); //PTA4���� �����ش���
    set_irq_priority((INT_PORTA + INC_IRQ),1); //�����ж����ȼ� 1
    setIRQadrr(INT_PORTA,PORTA_IRQHandler);    //����PORTA�жϵ�ַ���� 
}

void   Ultrasonic_Delay(uint32 n)  //��������ʱus������
{
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<13;j++);   //   13   100M   1us
                         //   28   180M   1us
                         //   32   200M   1us
  }
}

void   Ultrasonic_Trig() //���������ͺ���
{
  TX=1;   //����һ��ģ��
  delay_ms(100); 
  TX=0;
}
float  Ultrasonic_Compute(uint32 us)      //������������뺯��
{
  float Distance;
  
  Distance=(us*340.0)/80000.0;//*amend;
  
  return Distance;
}