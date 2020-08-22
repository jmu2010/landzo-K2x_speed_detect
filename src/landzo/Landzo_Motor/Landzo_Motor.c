#include  "Landzo_motor.h"

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：Motor_one_PWM_Init
*  功能说明：电机驱动初始化程序
*  参数说明：
*  函数返回：无
*  修改时间：2014-9-11
*  备    注：
*************************************************************************/

void Motor_one_PWM_init(void)
{    
  gpio_init (PTC4, GPO,HIGH);                             //电机使能
  TPM_PWM_init(TPM0 , CH1, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH2, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：Motor_double_PWM_Init
*  功能说明：电机驱动初始化程序
*  参数说明：
*  函数返回：无
*  修改时间：2014-9-5
*  备    注：
*************************************************************************/

void Motor_double_PWM_init(void)
{    
  TPM_PWM_init(TPM0 , CH1, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH2, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH4, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
  TPM_PWM_init(TPM0 , CH5, 80000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;TPM_CnV_REG(TPMx[ftmn], ch) = cv;
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：Motor_one_test
*  功能说明：电机驱动初始化程序
*  参数说明：
*  函数返回：无
*  修改时间：2014-9-11
*  备    注：
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
*                           蓝宙电子工作室
*
*  函数名称：Motor_double_test
*  功能说明：电机驱动初始化程序
*  参数说明：
*  函数返回：无
*  修改时间：2014-9-11
*  备    注：
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


