/***********************************************************
 * �ļ���       ��systemclock.h
 * ˵��         ��ϵͳʱ��Դ����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 * ��̳         ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_                        /**< Symbol preventing repeated inclusion */






///*!< Macro to enable all interrupts. */
//#define EnableInterrupts asm(" CPSIE i");
//
///*!< Macro to disable all interrupts. */
//#define DisableInterrupts asm(" CPSID i");
/***************
�ⲿ���� ��λ M
***************/

#define OSCCLK    50 

/**************
systemƵ�ʶ���
***************/

#define  SYSTEMCLOCK   PLL48


#endif  
