#include  "Landzo_motor.h"

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Motor_one_PWM_Init
*  ����˵�������������ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-11
*  ��    ע��
*************************************************************************/

void Motor_one_PWM_init(void)
{    
  gpio_init (PTC4, GPO,HIGH);                             //���ʹ��
  TPM_PWM_init(TPM0 , CH1, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH2, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Motor_double_PWM_Init
*  ����˵�������������ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-5
*  ��    ע��
*************************************************************************/

void Motor_double_PWM_init(void)
{    
  TPM_PWM_init(TPM0 , CH1, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH2, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH4, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH5, 80000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Motor_one_test
*  ����˵�������������ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-11
*  ��    ע��
*************************************************************************/

void Motor_one_test(void)
{
  static uint8_t count=0;
  if( count == 3 )
  {
    count = 2 ;
    TPM_PWM_Duty(TPM0 , CH1,40);
    TPM_PWM_Duty(TPM0 , CH2,0);
  }
  else if(count == 2)
  {
    
    count = 1 ;
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,0);
    
  }
  else if(count == 1)
  {
    
    count = 0 ;
    
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,40);
    
  }
  else if(count == 0)
  {    
    count = 3 ;
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,0);
    
  }
}
/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Motor_double_test
*  ����˵�������������ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-11
*  ��    ע��
*************************************************************************/
void Motor_double_test(void)
{
  static uint8_t count=0;
  if( count == 3 )
  {
    count = 2 ;
    TPM_PWM_Duty(TPM0 , CH1,40);
    TPM_PWM_Duty(TPM0 , CH2,0);
    TPM_PWM_Duty(TPM0 , CH4,40);
    TPM_PWM_Duty(TPM0 , CH5,0);
  } 
  else if(count == 2)
  {
    
    count = 1 ;
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,0);
    TPM_PWM_Duty(TPM0 , CH4,0);
    TPM_PWM_Duty(TPM0 , CH5,0);
    
  }
  else if(count == 1)
  {
    
    count = 0 ;
    
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,40);
    TPM_PWM_Duty(TPM0 , CH4,0);
    TPM_PWM_Duty(TPM0 , CH5,40);
    
  } 
  else if(count == 0)
  {
    
    count = 3 ;
    TPM_PWM_Duty(TPM0 , CH1,0);
    TPM_PWM_Duty(TPM0 , CH2,0);
    TPM_PWM_Duty(TPM0 , CH4,0);
    TPM_PWM_Duty(TPM0 , CH5,0);
    
  }
}


