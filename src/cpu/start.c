
#include "common.h"
#include "system.h"
#include "KL2x_gpio.h"
extern void main(void);

uint32  core_clk_khz ;    //系统时钟
uint32  bus_clk_khz ;      //bus时钟



//看门狗WDOG(COP)宏定义
#define WDOG_DISABLE() {SIM_COPC = 0x00u;}  //禁用看门狗
//COP使用LPO时钟，COP在2^10个LPO时钟周期后延时
#define WDOG_ENABLE() {SIM_COPC |= SIM_COPC_COPT_MASK;} //使能看门狗


// 系统上电启动程序
 void start(void)
{
  WDOG_DISABLE();       //禁用看门狗
  //WDOG_ENABLE();        使能看门狗
  common_startup();
  gpio_enable_port();
  core_clk_khz = system_init(SYSTEMCLOCK) ;     //cpu主频设置
  bus_clk_khz = core_clk_khz /(((SIM_CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1) ;
  main();                 //进入主函数
  while(1);               //永久循环（主函数内永久循环，含有执行不到该语句）
}

//-------------------------------------------------------------------------------



