
#include "include.h"
#include "Landzo_Calculation.h"



/*********************************************************** 
函数名称：CCD_init
函数功能：
入口参数：
出口参数：无 
备 注： 
***********************************************************/
//extern uint8  BUFF[500] ;
extern uint8  ADdata[DATALINE][DATACOUNT];

void CCD_INC (void)
{
   gpio_Interrupt_init(PTA17, GPI_UP, FALLING) ;          //场中断
   gpio_Interrupt_init(PTA14, GPI_UP, FALLING) ;          //行中断 
   DMA_PORTx2BUFF_Init (DMA_CH0, (void *)&PTD_BYTE0_INT, (void *)&ADdata[0][0], PTA16,DMA_BYTE1, DATACOUNT); //初始化DMA模块    
    //DMA通道3 ，PTE0-PTE7 数据采集，ADdata数据存储位置，PTA16触发通道，DMA_BYTE1  数据存储长度，DATACOUNT  数据总个数
   set_irq_priority((INT_PORTA + INC_IRQ),0);    //设置优先级
   setIRQadrr(INT_PORTA,PORTA_IRQHandler);  //设置中断地址函数
}



