/***********************************************************
 * �ļ���       ��lptmr.c
 * ˵��         ���͹��Ķ�ʱģʽ
 * ����         ��Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#include "common.h"
#include "KL2x_lptmr.h"

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�lptmr_counter_init
*  ����˵��������������
*  ����˵����LPT0_ALTn  PIN��ѡ��
*            count      �������
*            LPT_CFG    ������ʽ
*  �������أ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void lptmr_counter_init(LPT0_ALTn altn, uint16 count, uint8 PrescaleValue, LPT_CFG cfg)
{

    if(PrescaleValue > 0x0f) PrescaleValue = 0x0f;

    //��������ܽ�
    if(altn == LPT0_ALT1)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //�� PORTA ʱ��
        PORTA_PCR19 = PORT_PCR_MUX(0x5);    //��PTA19��ʹ�� ALT5
    }
    else if(altn == LPT0_ALT2)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //ʹ�� PORTC ʱ��
        PORTC_PCR5 = PORT_PCR_MUX(0x3);     //��PTC5��ʹ�� ALT3
    }
    // ����ģ��ʱ��
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //ʹ��LPTģ��ʱ��

    // ��״̬�Ĵ���
    LPTMR0_CSR = 0x00;                      //�ȹ���LPT��������������ʱ�ӷ�Ƶ��

    //�����ۼӼ���ֵ
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =  LPTMR_CMR_COMPARE(count);                       //���ñȽ�ֵ
    // ʱ��ѡ�� 
    LPTMR_PSR_REG(LPTMR0_BASE_PTR)  = ( 0 
                                        | LPTMR_PSR_PCS(0x0) 
                                        | LPTMR_PSR_PBYP_MASK  
                                        | LPTMR_PSR_PRESCALE(PrescaleValue)        //ʹ�� LPO clock �� bypass glitch filter
                                      );                                              // ���������������˲�����2^n��ʱ�������ز�ʶ��
                                       

    // �ܽ����á�ʹ���ж�
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  = (0  
                                       | LPTMR_CSR_TPS(altn) 
                                       | LPTMR_CSR_TMS_MASK  
                                       | ( cfg == LPT_Falling ? LPTMR_CSR_TPP_MASK : 0)  
                                     //  | LPTMR_CSR_TEN_MASK  
                                       | LPTMR_CSR_TIE_MASK  
                                      );
    // ѡ������ܽ�  ѡ��������� �½��� ������ ʹ��LPT
    // TFC = 0��������ֵ���ڱȽ�ֵʱ������ֵ��λ
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ;       //ʹ��LPT
    enable_irq(LPTMR_irq_no);	 // �����ŵ�IRQ�ж�
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�time_delay_ms
*  ����˵������ʱ������ʹ�ö����Ķ�ʱ����ʱ��׼ȷ
*  ����˵����ms   ��ʱʱ�䣬��λΪms
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ������Դ�
*************************************************************************/
void time_delay_ms(uint32 ms)
{
    /* Make sure the clock to the LPTMR is enabled */
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

//    /* Set the compare value to the number of ms to delay */
//    LPTMR0_CMR = ms;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0_PSR = LPTMR_PSR_PCS(0x1) |LPTMR_PSR_PBYP_MASK; //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =  LPTMR_CMR_COMPARE(ms); // ���ñȽ�ֵ
    /* Start the timer */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); // ��ʱ���Ƚϱ�־��CNR = CMR����

    /* Clear Timer Compare Flag */
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; // �رն�ʱ��ʹ��

    return;
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�lptmr_pulse_get
*  ����˵��������������
*  ����˵����
*  �������أ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/

uint16 lptmr_pulse_get(void)
{
    uint16  LPTCOUNT = 0  ;
    LPTMR0_CNR = 12 ; // On each read of the CNR,software must first write to the CNR with any value. 
    LPTCOUNT = LPTMR0_CNR ;
    lptmr_counter_clean();
    return (uint16)LPTCOUNT;
}



