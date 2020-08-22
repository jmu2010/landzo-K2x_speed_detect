#ifndef _LANDZO_ULTRASONIC_H
#define _LANDZO_ULTRASONIC_H

void   Ultrasonic_Init();          //超声波初始化
void   Ultrasonic_Delay(uint32 n);    //超声波延时us级函数
void   Ultrasonic_Trig();          //超声波发送函数
float  Ultrasonic_Compute(uint32 us);       //超声波计算距离函数
#endif


