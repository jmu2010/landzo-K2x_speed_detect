/***********************************************************
 * �ļ���       ��K6x_gpio.c
 * ˵��         ��K60 ���Ź���������ͷ�ļ�
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __k6x_WDOG_H__
#define __k6x_WDOG_H__

#include "common.h"


void wdog_init(void);
void wdog_disable(void);
void wdog_enable(void);
void wdog_feed(void);
void wdog_enable_int(void);
void wdog_disable_int(void);

#endif 
