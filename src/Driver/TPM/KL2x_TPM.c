/***********************************************************
 * �ļ���       ��FTM.c
 * ˵��         ��FTM��ʱ����Ŀǰ�� PWM�����벶׽����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#include  "include.h"
#include  "KL2x_TPM.h"

volatile struct TPM_MemMap *TPMx[3] = {TPM0_BASE_PTR, TPM1_BASE_PTR, TPM2_BASE_PTR}; //��������ָ�����鱣�� FTMn �ĵ�ַ




/*************************************************************************
*                            Ұ��Ƕ��ʽ����������
*
*  �������ƣ�TPM_PWM_init
*  ����˵������ʼ��TPM��PWM���ܲ�����Ƶ�ʡ�ռ�ձȡ�����ͨ�����ռ�ձȡ�
*  ����˵����TPMn        ģ��ţ�TPM0��  TPM1��  TPM2��
*            CHn         ͨ���ţ�CH0~CH7��
*            freq        Ƶ�ʣ���λΪHz��
*            duty        ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��ͬһ��FTM����ͨ����PWMƵ����һ���ģ���3��FTM�����������3����ͬƵ��PWM
*************************************************************************/

void TPM_PWM_init(TPMn tpmn, CHn ch, uint32 freq, uint32 duty)
{
    uint16 mod;
    uint8 sc_ps;
    uint16 cv;   
    uint32 clk_hz = (bus_clk_khz * 1000) >> 1;       //busƵ��/2 
    /*       �����Ƶ����       */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    if(freq < 1000)sc_ps++;    
    mod = (clk_hz >> sc_ps) / freq;                        //Ƶ����������,clk_hz = 25000000
    /* 
    uart_putchar(UART0,mod>>24); 
    uart_putchar(UART0,mod>>16); 
    uart_putchar(UART0,mod>>8); 
    uart_putchar(UART0,mod>>0); */
    cv = (duty * (mod - 0 + 1)) / TPM_PRECISON;            //ռ�ձ���������
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;                           //ʹ��FTM0ʱ��
        switch(ch)
        {
        case CH0:
            if((TPM0_CH0 == PTC1) || (TPM0_CH0 == PTD0))
            {
                Port_alt(TPM0_CH0,ALT4);
            }
            else if(TPM0_CH0 == PTE24)
            {
                Port_alt(TPM0_CH0,ALT3);
            }
            break;

        case CH1:
            if((TPM0_CH1 == PTC2) || (TPM0_CH0 == PTD1))
            {
                Port_alt(TPM0_CH1,ALT4);
            }
            else if((TPM0_CH1 == PTA4) || (TPM0_CH1 == PTE25))
            {
                Port_alt(TPM0_CH1,ALT3);
            }
            break;

        case CH2:
            if((TPM0_CH2 == PTC3) || (TPM0_CH2 == PTD2))
            {
                Port_alt(TPM0_CH2,ALT4);
            }
            else if((TPM0_CH2 == PTA5) || (TPM0_CH2 == PTE29))
            {
                Port_alt(TPM0_CH2,ALT3);
            }
            break;

        case CH3:
            if((TPM0_CH3 == PTC4) || (TPM0_CH3 == PTD3))
            {
                Port_alt(TPM0_CH3,ALT4);
            }
            else if(TPM0_CH3 == PTE30)
            {
                Port_alt(TPM0_CH3,ALT3);
            }
            break;

        case CH4:
            if(TPM0_CH4 == PTD4)
            {
                Port_alt(TPM0_CH4,ALT4);
            }
            else if((TPM0_CH4 == PTC8) || (TPM0_CH4 == PTE31))
            {
                Port_alt(TPM0_CH4,ALT3);
            }
            break;

        case CH5:
            if(TPM0_CH5 == PTD5)
            {
                Port_alt(TPM0_CH5,ALT4);
            }
            else if(TPM0_CH5 == PTC9)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            break;

        default:
            return;
        }
        break;

    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;                           //ʹ��FTM1ʱ��
        switch(ch)
        {
        case CH0:
          Port_alt(TPM1_CH0,ALT3) ;
          break;
        case CH1:
          Port_alt(TPM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case CH0:
          Port_alt(TPM2_CH0,ALT3) ;
          break;

        case CH1:
          Port_alt(TPM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
    default:
        break;
    }
    /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
    //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ
    TPM_CnSC_REG(TPMx[tpmn], ch) &= ~TPM_CnSC_ELSA_MASK;
    TPM_CnSC_REG(TPMx[tpmn], ch)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       ���ض���PWM
    // ELSnB:ELSnA = 10     �ȸߺ��
    // ELSnB:ELSnA = 11     �ȵͺ��

    /******************** ����ʱ�Ӻͷ�Ƶ ********************/
    //SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        // 00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    TPM_SC_REG(TPMx[tpmn])    = ( 0
                                  //| TPM_SC_CPWMS_MASK         //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                                  | TPM_SC_PS(sc_ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                                  | TPM_SC_CMOD(1)
                                );
    TPM_MOD_REG(TPMx[tpmn])   = mod;                        //ģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
    TPM_CnV_REG(TPMx[tpmn], ch) = cv;
    TPM_CNT_REG(TPMx[tpmn])   = 0;                          //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ�����������0��
}


/*************************************************************************
*                            Ұ��Ƕ��ʽ����������
*
*  �������ƣ�TPM_PWM_Duty
*  ����˵��������ͨ��ռ�ձ�
*  ����˵����TPMn        ģ��ţ�TPM0��  TPM1��  TPM2��
*            CHn         ͨ����
*            duty        ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��
*************************************************************************/

void TPM_PWM_Duty(TPMn tpmn, CHn ch, uint32 duty)
{
    uint32 cv;
    uint32 mod = 0;
//  ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = TPM_MOD_REG(TPMx[tpmn]);        //��ȡ MOD ��ֵ
    cv = (duty * (mod - 0 + 1)) / TPM_PRECISON;

    // ����TPMͨ��ֵ
    TPM_CnV_REG(TPMx[tpmn], ch) = cv;
}


/*************************************************************************
*                            Ұ��Ƕ��ʽ����������
*
*  �������ƣ�TPM_PWM_freq
*  ����˵��������FTM��Ƶ��
*  ����˵����TPMn        ģ��ţ�TPM0��  TPM1��  TPM2��
*            freq        Ƶ�ʣ���λΪHz��
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*  ��    ע��ͬһ��FTM����ͨ����PWMƵ����һ���ģ���3��FTM�����������3����ͬƵ��PWM
*************************************************************************/
void TPM_PWM_freq(TPMn tpmn, uint32 freq)             //����TPM��Ƶ��
{
    uint32 clk_hz = (core_clk_khz * 1000) >> 1;        //busƵ��/2
    uint32 mod;
    uint8 sc_ps;

    /*       ����Ƶ������        */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    mod = (clk_hz >> sc_ps) / freq;


    /******************** ����ʱ�Ӻͷ�Ƶ ********************/
    TPM_SC_REG(TPMx[tpmn])    = TPM_SC_PS(sc_ps) | TPM_SC_CMOD(1);  //û��ʹ������ж�
    TPM_MOD_REG(TPMx[tpmn])   = mod;                                                    //Modulo valueģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
    TPM_CNT_REG(TPMx[tpmn])   = 0;                                                      //��������ֻ�е�16λ����
    
}






/*************************************************************************
*                            Ұ��Ƕ��ʽ����������
*
*  �������ƣ�TPM_Input_init
*  ����˵�������벶׽��ʼ������
*  ����˵����TPMn        ģ��ţ�TPM0��  TPM1��  TPM2��
*            CHn         ͨ����
*            Input_cfg   ���벶׽���ã�Rising��Falling��Rising_or_Falling�������ز�׽���½��ز�׽�������ز�׽
*  �������أ���
*  �޸�ʱ�䣺2012-1-26
*  ��    ע��CH0~CH3����ʹ�ù�������δ����⹦��
*************************************************************************/
void TPM_Input_init(TPMn tpmn, CHn ch, Input_cfg cfg)
{
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;                           //ʹ��FTM0ʱ��
        switch(ch)
        {
        case CH0:
            if((TPM0_CH0 == PTC1) || (TPM0_CH0 == PTD0))
            {
                Port_alt(TPM0_CH0,ALT4) ;
            }
            else if(TPM0_CH0 == PTE24)
            {
                Port_alt(TPM0_CH0,ALT3) ;
            }
            break;

        case CH1:
            if((TPM0_CH1 == PTC2) || (TPM0_CH0 == PTD1))
            {
                Port_alt(TPM0_CH1,ALT4) ;
            }
            else if((TPM0_CH1 == PTA4) || (TPM0_CH1 == PTE25))
            {
                Port_alt(TPM0_CH1,ALT3) ;
            }
            break;

        case CH2:
            if((TPM0_CH2 == PTC3) || (TPM0_CH2 == PTD2))
            {
                Port_alt(TPM0_CH2,ALT4) ;
            }
            else if((TPM0_CH2 == PTA5) || (TPM0_CH2 == PTE29))
            {
                Port_alt(TPM0_CH2,ALT3) ;
            }
            break;

        case CH3:
            if((TPM0_CH3 == PTC4) || (TPM0_CH3 == PTD3))
            {
                Port_alt(TPM0_CH3,ALT4) ;
            }
            else if(TPM0_CH3 == PTE30)
            {
                Port_alt(TPM0_CH3,ALT3) ;
            }
            break;

        case CH4:
            if(TPM0_CH4 == PTD4)
            {
                Port_alt(TPM0_CH4,ALT4) ;
            }
            else if((TPM0_CH4 == PTC8) || (TPM0_CH4 == PTE31))
            {
                Port_alt(TPM0_CH4,ALT3) ;
            }
            break;

        case CH5:
            if(TPM0_CH5 == PTD5)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            else if(TPM0_CH5 == PTC9)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            break;

        default:
            return;
        }
        break;

    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;                           //ʹ��FTM1ʱ��
        switch(ch)
        {
        case CH0:
          Port_alt(TPM1_CH0,ALT3) ;
          break;
        case CH1:
          Port_alt(TPM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case CH0:
          Port_alt(TPM2_CH0,ALT3) ;
          break;

        case CH1:
          Port_alt(TPM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
    default:
        break;
    }

    switch(cfg)
    {
    case Rising:    //�����ش���
        TPM_CnSC_REG(TPMx[tpmn], ch) |=  ( TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK );                   //��1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_ELSB_MASK  | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //��0
        break;

    case Falling:   //�½��ش���
        TPM_CnSC_REG(TPMx[tpmn], ch) |= (TPM_CnSC_ELSB_MASK  | TPM_CnSC_CHIE_MASK );                    //��1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //��0
        break;

    case Rising_or_Falling: //�����ء��½��ض�����
        TPM_CnSC_REG(TPMx[tpmn], ch) |=  ( TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK ); //��1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_MSB_MASK  | TPM_CnSC_MSA_MASK); //��0
        break;
    }

    TPM_SC_REG(TPMx[tpmn]) = TPM_SC_CMOD(0x1);       //System clock
    TPM_MOD_REG(TPMx[tpmn])   = 0xFFFF;
    TPM_CNT_REG(TPMx[tpmn])   = 0;                   //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ�����������0��

    TPM_STATUS_REG(TPMx[tpmn]) = 0x00;               //���жϱ�־λ

    //�������벶׽�ж�
    enable_irq(TPM0_irq_no + tpmn);
}



