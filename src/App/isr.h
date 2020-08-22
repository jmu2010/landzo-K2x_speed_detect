/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：isr.h
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1
#include  "common.h"


void HardFault_Handler(void);      //发生硬件上访（程序跑飞）
void UART0_IRQHandler(void);      // USART0中断处理函数
void PIT_IRQHandler(void);            //PIT0 定时中断服务函数
void PORTA_IRQHandler(void);           //PORTA中断服务函数
void PORTD_IRQHandler(void);           //PORTD中断服务函数
void FTM0_IRQHandler(void);            //FTM0输入捕捉中断
void FTM1_IRQHandler(void);            //FTM0输入捕捉中断
void LPT_Handler(void);           //LPT脉冲中断
void DMA_CH0_Handler(void);
void DMA_CH4_Handler(void);              //DMA通道4的中断服务函数
__RAMFUN void setIRQadrr(IRQInterruptIndex IRQnum,void IRQAdrr(void) ) ;

#endif  //__ISR_H

/* End of "isr.h" */
