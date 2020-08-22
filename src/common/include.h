/***********************************************************
 * �ļ���       ��include.h
 * ˵��         ������ģ������ļ�
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */

#include  "port.h"
#include  "isr.h"
#include  "KL2x_gpio.h"
#include  "KL2x_delay.h"
#include  "KL2x_flash.h" 
#include  "KL2x_PIT.h"       // �����жϼ�ʱ��
#include  "KL2x_adc.h"       // ADCģ��
#include  "KL2x_uart.h"      // ����
#include  "KL2x_lptmr.h"     // �͹��Ķ�ʱ��(��ʱ)
#include  "KL2x_TPM.h"       // FTMģ��
#include  "KL2x_spi.h" 
#include  "KL2x_i2c.h" 
#include  "KL2x_dma.h"       //DMAģ��



#include "Landzo_Calculation.h" // ����ͷ
#include "Landzo_CCD.h"         // CCD
#include "Landzo_Electromagnetism.h"
#include "Landzo_GyroAccelerate.h"
#include "Landzo_Key.h"
#include "Landzo_MeasureSpeed.h"
#include "Landzo_Motor.h"
#include "Landzo_OLED.h"
#include "Landzo_Predator.h"
#include "Landzo_servomotor.h"  

/*



   
    

#include  "landzo_sd.h"  
*/




#endif  //__INCLUDE_H__
