/***********************************************************
 * �ļ���       ��KL2x_TPM.h
 * ˵��         ��TPM��ʱ����Ŀǰ�� PWM�����벶׽����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef _KL2X_TPM_H_
#define _KL2X_TPM_H_

extern volatile struct TPM_MemMap *TPMx[3];

typedef enum TPMn
{
    TPM0,
    TPM1,
    TPM2
} TPMn;

typedef enum CHn
{
           //   --FTM0--  --FTM1--  --FTM2--
    CH0,   //     PTC1      PTA12      PTA1
    CH1,   //     PTC2      PTA13      PTA2
    CH2,   //     PTC3       ��         ��
    CH3,   //     PTC4       ��         ��
    CH4,   //     PTD4       ��         ��
    CH5,   //     PTD5       ��         ��
    // ����ʾ������
} CHn;



/*********************** PWM **************************/

#define TPM_PRECISON 100u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/FTM_PRECISON

void  TPM_PWM_init(TPMn, CHn, uint32 freq, uint32 duty);  //��ʼ��FTM��PWM���ܲ�����Ƶ�ʡ�ռ�ձȡ�����ͨ�����ռ�ձȡ�ͬһ��FTM����ͨ����PWMƵ����һ���ģ���3��FTM

void  TPM_PWM_Duty(TPMn, CHn, uint32 duty);    //����ͨ��ռ�ձ�,ռ�ձ�Ϊ ��duty * ���ȣ� % ����� FTM_PRECISON ����Ϊ 1000 ��duty = 100 ����ռ�ձ� 100*0.1%=10%
void  TPM_PWM_freq(TPMn, uint32 freq);              //����FTM��Ƶ��

void  TPM_QUAD_init(TPMn tpmn,CHn ch);
void  TPM_SpeedMeasure_init(void);


/*********************** ���벶׽ **************************/

typedef enum Input_cfg
{
    Rising,               //�����ز�׽
    Falling,              //�½��ز�׽
    Rising_or_Falling     //�����ز�׽
} Input_cfg;


void TPM_Input_init(TPMn, CHn, Input_cfg);

#define TPM_IRQ_EN(TPMn,CHn)        TPM_CnSC_REG(TPMx[TPMn],CHn) |= TPM_CnSC_CHIE_MASK       //���� TPMn_CHn �ж�
#define TPM_IRQ_DIS(TPMn,CHn)       TPM_CnSC_REG(TPMx[TPMn],CHn) &= ~TPM_CnSC_CHIE_MASK      //�ر� TPMn_CHn �ж�








#endif  



