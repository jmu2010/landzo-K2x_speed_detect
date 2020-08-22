
/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��adc.c
 * ����         ��adc��������
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	

/***********************************************************
 * �ļ���       ��adc.c
 * ˵��         ��adc��ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/
#include "common.h"
#include "KL2x_adc.h"
#include "gpio_cfg.h"
#include "port.h"


ADC_MemMapPtr ADCx[1] = {ADC0_BASE_PTR}; //��������ָ�����鱣�� ADCx �ĵ�ַ
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�adc_DMA_init
*  ����˵����AD��ʼ����ʹ��ʱ��
*  ����˵����ADCn        ģ��ţ� ADC0��
*  �������أ���ao
*  ��    ע���ο����ݴ�ѧ������
*************************************************************************/
void adc_DMA_init(ADC_Ch ch)
{
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_init(ch)  ;
    //��ʼ��ADCĬ������
    ADC_CFG1_REG(ADCx[adcn])  = ADLPC_LOW
                                 | ADC_CFG1_ADIV(ADIV_4)
                                 | ADLSMP_SHORT
                                 | ADC_CFG1_MODE(0)                   //8λ
                                 | ADC_CFG1_ADICLK(ADICLK_BUS);
    ADC_CFG2_REG(ADCx[adcn])  = (ch>>5) // MUXSEL_ADCB    //MUXSEL_ADCA
                                 | ADACKEN_ENABLED
                                 | ADHSC_HISPEED
                                 | ADC_CFG2_ADLSTS(ADLSTS_2) ;
    
    
   
    ADC_SC2_REG(ADCx[adcn]) = ADTRG_SW                       //Ӳ������
                                | ACFE_DISABLED 
                                | ACFGT_GREATER 
                                | ACREN_DISABLED 
                                | DMAEN_DISABLED               //ʹ��DMA DMAEN_ENABLED
                                | ADC_SC2_REFSEL(REFSEL_EXT);
    
    ADC_SC3_REG(ADCx[adcn])  = CAL_OFF 
                                 | ADCO_CONTINUOUS 
                                 | AVGE_DISABLED 
                                 | ADC_SC3_AVGS(AVGS_4);
  
    ADC_SC1_REG(ADCx[adcn],ch>>5) = AIEN_ON | DIFF_SINGLE |      //
                                 ADC_SC1_ADCH( ch );
 
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�adc_init
*  ����˵������ʼ��PINΪADģʽ
*  ����˵����ADCn      ADģ�飨AD0)
*            ADC_Ch    ADͨ����1 2 3 ......)
*  �������أ���
*  �޸�ʱ�䣺2012-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void adc_init(ADC_Ch ch)
{

  SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //����ADC0ʱ��
  SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
  SIM_SOPT7 = SIM_SOPT7_ADC0TRGSEL(0);

  switch(ch)
  {
    case ADC0_SE4a:  //ADC0_SE4a-- PTE21
      Port_alt(PTE21,ALT0);
      break;
    case ADC0_SE7a:  //ADC0_SE7a -- PTE23
      Port_alt(PTE23,ALT0);
      break;
    case ADC0_SE5b:  //ADC0_SE5b -- PTD1
      Port_alt(PTD1,ALT0);
      break;
    case ADC0_SE6b:   //ADC0_SE6b -- PTD5
      Port_alt(PTD5,ALT0);
      break;
    case ADC0_SE8:  //ADC0_SE8 -- PTB0
      Port_alt(PTB0,ALT0);
      break;
    case ADC0_SE9:  //ADC0_SE9 -- PTB1
      Port_alt(PTB1,ALT0);
      break;
    case ADC0_SE11:  //ADC0_SE11 -- PTC2
      Port_alt(PTC2,ALT0);
      break;
    case ADC0_SE12:  //ADC0_SE12 -- PTB2
      Port_alt(PTB2,ALT0);
      break;
    case ADC0_SE13:  //ADC0_SE13 -- PTB3
      Port_alt(PTB3,ALT0);
      break;
    case ADC0_SE14:  //ADC0_SE14 -- PTC0
      Port_alt(PTC0,ALT0);
      break;
    case ADC0_SE15:   //ADC0_SE15 -- PTC1
      Port_alt(PTC1,ALT0);
      break;
    case ADC0_SE23:   //ADC0_SE23 -- PTE30
      Port_alt(PTE30,ALT0);
      break;
    default:
          break;
  }
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�ad_once
*  ����˵�����ɼ�һ��һ·ģ������ADֵ
*  ����˵����ADCn        ģ��ţ� ADC0��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*  �������أ��޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���ο����ݴ�ѧ������
*************************************************************************/
uint16_t ad_once(ADC_Ch ch, ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
    uint16_t result = 0;
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_start(ch, bit);	  //����ADCת��

    while (( ADC_SC1_REG(ADCx[adcn], 0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
    result = ADC_R_REG(ADCx[adcn], 0);
    ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�ad_ave
*  ����˵������β�����ȡƽ��ֵ
*  ����˵����ADCx        ģ��ţ� ADC0��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*            N           ��ֵ�˲�����(��Χ:0~255)
*  �������أ�16λ�޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
uint16_t ad_ave(ADC_Ch ch, ADC_nbit bit, uint8_t N) //��ֵ�˲�
{
    uint32_t tmp = 0;
    uint8_t  i;

    for(i = 0; i < N; i++)
        tmp += ad_once(ch, bit);
    tmp = tmp / N;
    return (uint16_t)tmp;
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�ad_SUM
*  ����˵������β�����ȡƽ��ֵ
*  ����˵����ADCx        ģ��ţ� ADC0��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*            N           ��ֵ�˲�����(��Χ:0~255)
*  �������أ�16λ�޷��Ž��ֵ
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
uint16_t ad_sum(ADC_Ch ch, ADC_nbit bit, uint8_t N) //��������
{
    uint32_t tmp = 0;
    uint8_t  i;
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_start(ch, bit);	  //����ADCת��
    
    for(i = 0; i < N; i++)
    {
      while (( ADC_SC1_REG(ADCx[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
      tmp += ADC_R_REG(ADCx[adcn], 0);
      ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    }
    return (uint16_t)tmp;
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�adc_start
*  ����˵��������adc���������Bͨ�������������������������
*  ����˵����ADCx        ģ��ţ� ADC0��
*            ADC_Channel ͨ����
*            ADC_nbit    ���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
*  �������أ���
*  �޸�ʱ�䣺2012-2-10
*  ��    ע���޸����ݴ�ѧ������
*************************************************************************/
void adc_start(ADC_Ch ch, ADC_nbit bit)
{
  ADCn adcn = (ADCn)(ch >> 5) ;
  //��ʼ��ADCĬ������
  ADC_CFG1_REG(ADCx[adcn])  = ADLPC_NORMAL
                               | ADC_CFG1_ADIV(ADIV_4)
                               | ADLSMP_LONG
                               | ADC_CFG1_MODE(bit)
                               | ADC_CFG1_ADICLK(ADICLK_BUS);
  ADC_CFG2_REG(ADCx[adcn])  =  ((ch>>5)  << ADC_CFG2_MUXSEL_SHIFT)   //MUXSEL_ADCB    //MUXSEL_ADCA
                               | ADACKEN_DISABLED
                               | ADHSC_HISPEED
                               | ADC_CFG2_ADLSTS(ADLSTS_20) ;
  ADC_SC1_REG(ADCx[adcn],0) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH( ch );
   
}





