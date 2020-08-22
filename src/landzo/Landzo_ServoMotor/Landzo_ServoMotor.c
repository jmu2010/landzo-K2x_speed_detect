#include  "Landzo_servomotor.h"


/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_S3010_init
*  ����˵����S3010�ŷ�����ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_S3010_init(void)
{
   TPM_PWM_init(TPM1, CH0 , 50,23);                           //���ռ�ձ����ó�ʼ��   MOD =19531 ;
   TPM_PWM_init(TPM1, CH1 , 50,23);                           //���ռ�ձ����ó�ʼ��   MOD =19531 ; 
   TPM_CnV_REG(TPMx[TPM1], CH0) = MIDSTRING1;         //������� �ö����������λ���ͱ������������̴�ֱ��
   TPM_CnV_REG(TPMx[TPM1], CH1) = MIDSTRING1;         //������� �ö����������λ���ͱ������������̴�ֱ
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_SD5_init
*  ����˵����SD5�ŷ�����ʼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_SD5_init(void)
{
   TPM_PWM_init(TPM1, CH1 , 180,23);                           //���ռ�ձ����ó�ʼ��   MOD =19531 ;
   TPM_PWM_init(TPM1, CH0 , 180,23);                           //���ռ�ձ����ó�ʼ��   MOD =19531 ; 
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_S3010_centre
*  ����˵����S3010�ŷ������г���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_S3010_centre(void)
{
  uint16_t TurnPWM;
  TurnPWM = MIDSTRING1 ;
  TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ; 
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_SD5_centre
*  ����˵����SD5�ŷ������г���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_SD5_centre(void)
{
  uint16_t TurnPWM;
  TurnPWM = MIDSTRING2 ;
  TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ; 
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_S3010_test
*  ����˵����S3010�ŷ������Գ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_S3010_test(void)
{
  static int8_t tep=0;
  static uint8_t Turnflag=0;
  uint16_t TurnPWM;
  if(tep==10)  Turnflag=1;
  else if(tep==-10) Turnflag=0;
  
  if(Turnflag==0)
  {   
    tep ++ ;       
    TurnPWM = MIDSTRING1 + tep*30 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ; 
  } 
  else if(Turnflag==1)
  {
    tep--;
    TurnPWM = MIDSTRING1 + tep*30 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ;           
  }
  else
  {
    TurnPWM = MIDSTRING1 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ;    
  }
  
}
/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Servo_motor_SD5_test
*  ����˵����SD5�ŷ������Գ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-12
*  ��    ע��
*************************************************************************/
void Servo_motor_SD5_test(void)
{
  static int8_t tep=0;
  static uint8_t Turnflag=0;
  uint16_t TurnPWM;
  if(tep==10)  Turnflag=1;
  else if(tep==-10) Turnflag=0;
  
  if(Turnflag==0)
  {   
    tep ++ ;       
    TurnPWM = MIDSTRING2 + tep*100 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ; 
  } 
  else if(Turnflag==1)
  {
    tep--;
    TurnPWM = MIDSTRING2 + tep*100 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ;       
  }
  else
  {
    TurnPWM = MIDSTRING2 ;
    TPM_CnV_REG(TPMx[TPM1], CH1) = TurnPWM ; 
  TPM_CnV_REG(TPMx[TPM1], CH0) = TurnPWM ;    
  }
  
}
