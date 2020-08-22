
#include "common.h"
#include "system.h"
#include "KL2x_gpio.h"
extern void main(void);

uint32  core_clk_khz ;    //ϵͳʱ��
uint32  bus_clk_khz ;      //busʱ��



//���Ź�WDOG(COP)�궨��
#define WDOG_DISABLE() {SIM_COPC = 0x00u;}  //���ÿ��Ź�
//COPʹ��LPOʱ�ӣ�COP��2^10��LPOʱ�����ں���ʱ
#define WDOG_ENABLE() {SIM_COPC |= SIM_COPC_COPT_MASK;} //ʹ�ܿ��Ź�


// ϵͳ�ϵ���������
 void start(void)
{
  WDOG_DISABLE();       //���ÿ��Ź�
  //WDOG_ENABLE();        ʹ�ܿ��Ź�
  common_startup();
  gpio_enable_port();
  core_clk_khz = system_init(SYSTEMCLOCK) ;     //cpu��Ƶ����
  bus_clk_khz = core_clk_khz /(((SIM_CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1) ;
  main();                 //����������
  while(1);               //����ѭ����������������ѭ��������ִ�в�������䣩
}

//-------------------------------------------------------------------------------



