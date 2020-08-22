/***********************************************************
 * �ļ���       ��PTI.h
 * ˵��         ��PTI��ʼ��
 * ����         ��Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef _KL2X_PIT_H_
#define _KL2X_PIT_H_
#include "include.h"


typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;


void        pit_init(PITn, uint32_t cnt);                                               //��ʼ��PITn�������ö�ʱʱ��(��λΪbusʱ������)
#define     pit_init_ms(PITn,ms)          pit_init(PITn,ms * bus_clk_khz);         //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     pit_init_us(PITn,us)          pit_init(PITn,us * bus_clk_khz/1000);    //��ʼ��PITn�������ö�ʱʱ��(��λΪ us)
#define     pit_init_ns(PITn,ns)          pit_init(PITn,ns * bus_clk_khz/1000000); //��ʼ��PITn�������ö�ʱʱ��(��λΪ ns)

#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn) |= PIT_TFLG_TIF_MASK    //���жϱ�־


void        pit_dma_init(PITn pitn, uint32_t cnt);
#define     pit_dma_init_ms(PITn,ms)     pit_dma_init(PITn,ms * Gbuscl_KH)

#endif  //_PIT_H_