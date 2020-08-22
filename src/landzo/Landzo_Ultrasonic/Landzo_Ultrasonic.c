#include "include.h"
#include "KL2x_delay.h"

#define   TX     PTE31_OUT


void Ultrasonic_Init()
{
    gpio_init(PTE31,GPO,0);                     //初始化TX端口
    gpio_Interrupt_init(PTA12, GPI_DOWN, RING); //PTA4下拉 上升沿触发
    set_irq_priority((INT_PORTA + INC_IRQ),1); //设置中断优先级 1
    setIRQadrr(INT_PORTA,PORTA_IRQHandler);    //设置PORTA中断地址函数 
}

void   Ultrasonic_Delay(uint32 n)  //超声波延时us级函数
{
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<13;j++);   //   13   100M   1us
                         //   28   180M   1us
                         //   32   200M   1us
  }
}

void   Ultrasonic_Trig() //超声波发送函数
{
  TX=1;   //启动一次模块
  delay_ms(100); 
  TX=0;
}
float  Ultrasonic_Compute(uint32 us)      //超声波计算距离函数
{
  float Distance;
  
  Distance=(us*340.0)/80000.0;//*amend;
  
  return Distance;
}