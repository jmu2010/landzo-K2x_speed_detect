/***********************************************************
 * �ļ���       ��lptmr.h
 * ˵��         ���͹��Ķ�ʱģʽ
 * ����         ��Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __KL2X_LPTMR_H__
#define __KL2X_LPTMR_H__

/********************************************************************/


typedef enum LPT0_ALTn
{
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //�����ش���
    LPT_Falling = 1     //�½��ش���
} LPT_CFG;


#define  lptmr_counter_clean()     LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;\
                                    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK  ;          //������������0

/*          ������ʱ         */

void time_delay_ms(uint32 ms);      //��ʱ

/*       �����������        */

//void lptmr_counter_init(LPT0_ALTn, uint16_t count,  LPT_CFG);                           //��������ʼ������
void lptmr_counter_init(LPT0_ALTn altn, uint16 count, uint8 PrescaleValue, LPT_CFG cfg);
uint16 lptmr_pulse_get(void) ;




/********************************************************************/

#endif /* __LPTMR_H__ */
