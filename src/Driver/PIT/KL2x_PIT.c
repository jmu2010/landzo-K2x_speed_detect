/***********************************************************
 * �ļ���       ��PTI.c
 * ˵��         ��PTI��ʼ��
 * ����         ��Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#include "KL2x_PIT.h"     //�����жϼ�ʱ��

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�pit_init
*  ����˵����PITn��ʱ�ж�
*  ����˵����PITn        ģ��ţ�PIT0~PIT3��
             cnt         ��ʱʱ��(��λΪbusʱ������)
*  �������أ���
*  �޸�ʱ�䣺2012-1-24
*  ��    ע��
*************************************************************************/
void pit_init(PITn pitn, uint32_t cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    /* ����ʱ��*/
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;                            //ʹ��PITʱ��

    /* PITģ����� PIT Module Control Register (PIT_MCR) */
    PIT_MCR = 0 ;      //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    /* ��ʱ������ֵ���� Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt;                                          //��������ж�ʱ��
    /* ��ʱ�����ƼĴ��� Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) = ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //ʹ�� PITn��ʱ��,����PITn�ж�
    //��ʱʱ�䵽�˺�TIF �� 1 ��д1��ʱ��ͻ���0
    PIT_Flag_Clear(pitn);   //���жϱ�־λ
    enable_irq(PIT_irq_no);   //���������ŵ�IRQ�ж�
    
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�pit_init
*  ����˵����PITn��ʱ�ж�
*  ����˵����PITn        ģ��ţ�PIT0~PIT3��
             cnt         ��ʱʱ��(��λΪbusʱ������)
*  �������أ���
*  �޸�ʱ�䣺2012-1-24
*  ��    ע��
*************************************************************************/
void pit_dma_init(PITn pitn, uint32_t cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    /* ����ʱ��*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //ʹ��PITʱ��

    /* PITģ����� PIT Module Control Register (PIT_MCR) */
    //PIT_MCR         &=~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );       //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
    PIT_MCR = 0;

    /* ��ʱ������ֵ���� Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt;                                          //��������ж�ʱ��

    //��ʱʱ�䵽�˺�TIF �� 1 ��д1��ʱ��ͻ���0
    PIT_Flag_Clear(pitn);                                             //���жϱ�־λ

    /* ��ʱ�����ƼĴ��� Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= (0
                        | PIT_TCTRL_TEN_MASK  //ʹ�� PITn��ʱ��
                        | PIT_TCTRL_TIE_MASK  //��PITn�ж�
                       );

    enable_irq(PIT_irq_no);			                                //���������ŵ�IRQ�ж�
}