/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��isr.h
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��landzo���ӿ�����
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1
#include  "common.h"


void HardFault_Handler(void);      //����Ӳ���Ϸã������ܷɣ�
void UART0_IRQHandler(void);      // USART0�жϴ�����
void PIT_IRQHandler(void);            //PIT0 ��ʱ�жϷ�����
void PORTA_IRQHandler(void);           //PORTA�жϷ�����
void PORTD_IRQHandler(void);           //PORTD�жϷ�����
void FTM0_IRQHandler(void);            //FTM0���벶׽�ж�
void FTM1_IRQHandler(void);            //FTM0���벶׽�ж�
void LPT_Handler(void);           //LPT�����ж�
void DMA_CH0_Handler(void);
void DMA_CH4_Handler(void);              //DMAͨ��4���жϷ�����
__RAMFUN void setIRQadrr(IRQInterruptIndex IRQnum,void IRQAdrr(void) ) ;

#endif  //__ISR_H

/* End of "isr.h" */
