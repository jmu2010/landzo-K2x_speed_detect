/***********************************************************
 * �ļ���       ��KL25_gpio.c
 * ˵��         ��IO�˿�Ӧ�ú���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/
/*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "KL2x_gpio.h"



//���˿ڻ���ַ���볣��������PORTX[0]~PORTX[4]��
volatile struct PORT_MemMap *PORTX[5]={PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};
//GPIO�ڻ���ַ���볣��������GPIOx[0]~GPIOx[4]��
volatile struct GPIO_MemMap *GPIOX[5]={PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR};
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_enable_port
*  ����˵������������GPIO�˿ڵ�ʱ�ӡ� ���߲��������á�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
void gpio_enable_port (void)
{
 	  SIM_SCGC5 |=   SIM_SCGC5_PORTA_MASK \
	               | SIM_SCGC5_PORTB_MASK \
	               | SIM_SCGC5_PORTC_MASK \
	               | SIM_SCGC5_PORTD_MASK \
	               | SIM_SCGC5_PORTE_MASK;
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�gpio_Interrupt_init
*  ����˵������ʼ��GPIO�ж�Դ
*  ����˵����PTxn        �˿ں�
*            GPIO_CFG    �˿�״̬
*            mode        �ж�״̬ 
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/

void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode)
{
  if((cfg == GPI)||(cfg == GPI_DOWN)||(cfg == GPI_UP))
  {
    Port_init(ptxn,(cfg | ALT1 | mode));
    GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //���ö˿ڷ���Ϊ���� 
#if defined(MKL25Z4)
    if(PTX(ptxn)==0) enable_irq(PortA_irq_no);
    else if(PTX(ptxn)==3) enable_irq(PortD_irq_no);
#elif defined(MKL26Z4)
    if(PTX(ptxn)==0) enable_irq(PortA_irq_no);
    else if((PTX(ptxn)==2)||(PTX(ptxn)==3)) enable_irq(PortC_PortD_irq_no);
#endif
  }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������/������ӿƼ����޹�˾
*
*  �������ƣ�gpio_init
*  ����˵������ʼ��gpio
*  ����˵����PTxn        �˿ں�
*            GPIO_CFG    �˿�״̬
*            uint8       �������״̬ 1 ��0
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 sata)
{ 
//�˿ڷ���������뻹�����
    if( (cfg  == GPI) || (cfg == GPI_UP) || (cfg == GPI_DOWN) )
    {
       Port_init(ptxn,cfg|ALT1);
       GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //���ö˿ڷ���Ϊ���� 
    }
    else
    {  
        Port_init(ptxn,ALT1) ;  
      //���ö˿ڷ���Ϊ���
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));        // GPIO PDDR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿ڷ������
        //�˿��������/        
        if(sata == 0){     
            GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��0������Ӧ�ܽ�����Ϊ�˿�����͵�ƽ
         }else{
            GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿�����ߵ�ƽ
        }
    }
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�gpio_set
*  ����˵������������״̬
*  ����˵����PTxn        �˿ں�
*            uint8       �������״̬ 1 ��0
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_set (PTxn ptxn,uint8 sata){
  if(sata) GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��1������Ӧ�ܽ�����Ϊ�˿�����ߵ�ƽ
   else    GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR �ܽź� ��0������Ӧ�ܽ�����Ϊ�˿�����͵�ƽ 
}
/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�gpio_turn
*  ����˵�������ŷ�ת
*  ����˵����PTxn        �˿ں�
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
void gpio_turn(PTxn ptxn){
  GPIO_PTOR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));   // GPIO PTOR �ܽź�  
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�gpio_get
*  ����˵�������ŷ�ת
*  ����˵����PTxn        �˿ں�
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
uint8 gpio_get (PTxn ptxn){
  return ((GPIO_PDIR_REG(GPIOX[ptxn>> 5])>>(ptxn&0x1f)) &0x01);   // GPIO PDIR �ܽź�  
}
               
