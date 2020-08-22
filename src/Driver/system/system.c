
/***********************************************************
 * 文件名       ：system.c
 * 说明         ：系统频率初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://bbs.landzo.com
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include "system.h"

MCGPLL  Gmcgpll ;
SIMDIV  Gsimdiv ;


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

//uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

     
/*-------------------------------------------------------------------------*
//函数名: syscloption                                                   *
//功  能:  根据选择系统时钟选择乘除因子                                                   *
//参  数: SYSPLL  选择时钟							  	   *
//返  回: 无                                                               *
//说  明: K60DZ  K64F12 的计算公式 SYSPLL =   OSCCLK *(pllVDIV+24)  /(pllPRDIV+1) 
*         K60FX        的计算公式 SYSPLL =   OSCCLK *(pllVDIV+16)  /(pllPRDIV+1)
//-------------------------------------------------------------------------*/
 void syscloption(system_option SYSPLL){
   uint32 systemclock ;
   /**************
   计算pll分频因子,
   因为K60FX与另外芯片不一样，
   **************/
   #if (defined(MKL25Z4)||defined(MKL26Z4))
         #if( OSCCLK == 50)
           if(SYSPLL == 48 ){
             Gmcgpll.pllPRDIV = 24 ;
             Gmcgpll.pllVDIV  = 0 ;
           }           
           else if(SYSPLL == 50 ){
             Gmcgpll.pllPRDIV = 24 ;
             Gmcgpll.pllVDIV  = 1 ;
           }
           else if(SYSPLL <= 130){
             Gmcgpll.pllPRDIV = 19 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK- 24) ;
           }
           else if(SYSPLL <= 210){
             Gmcgpll.pllPRDIV = 13 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }
           else {
             Gmcgpll.pllPRDIV = 13 ;
             Gmcgpll.pllVDIV = 16 ;
           }

         #elif( OSCCLK == 12)
            if(SYSPLL == 50 ){
             Gmcgpll.pllPRDIV = 5 ;
             Gmcgpll.pllVDIV  = 1 ;
           }
           else if(SYSPLL <= 130){
             Gmcgpll.pllPRDIV = 3 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }else if(SYSPLL <= 220){
             Gmcgpll.pllPRDIV = 2 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }else {
             Gmcgpll.pllPRDIV = 2 ;
             Gmcgpll.pllVDIV =  24 ;
           }
         #endif
       if(Gmcgpll.pllVDIV > 0x1F) Gmcgpll.pllVDIV = 0x1F ;       
        systemclock = OSCCLK *(Gmcgpll.pllVDIV+24)/(Gmcgpll.pllPRDIV+1) - 2; 
  #endif
    /**************
   计算系统时钟
   **************/
        
   Gsimdiv.coreDIV = 0 ;
   Gsimdiv.busDIV = systemclock / BUSCLOCKMAX ;
//    Gsimdiv.FlexBusDIV = systemclock / FLEXCLOCKMAX ;
//     Gsimdiv.FlashDIV = systemclock/FLASHCLOCKMAX ;
   if(Gsimdiv.busDIV > 0x8) Gsimdiv.busDIV = 0X08 ;
//       if(Gsimdiv.FlexBusDIV > 0xF) Gsimdiv.FlexBusDIV = 0X0F ;
//       if(Gsimdiv.FlashDIV > 0xF) Gsimdiv.FlashDIV = 0X0F ;
  
 }
/*-------------------------------------------------------------------------*
//函数名: system_init                                                  *
//功  能: 系统初始化                                                   *
//参  数: SYSPLL  系统选择时钟							  	   *
//返  回: sysclolck   设置的系统时钟                                                              *
//说  明: 芯片使用PLL来产生芯片所需的各种时钟，芯片复位后，MCG默认进入FEI模式，
//         需按照FEI->FEB->PBE->PEE的顺序进行配置。                                                     *
//-------------------------------------------------------------------------*/
 uint32 system_init(system_option SYSPLL){
   uint32 systemclock ;
   
   syscloption(SYSPLL) ;
   
   //默认开始状态为FEI模式，切换到FBE模式
    // Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
     MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK| MCG_C2_EREFS0_MASK;          
    //选择外部晶振（50MHz）作为时钟源，设置FLL分频器，清IREFS启动外部晶振
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(6);
    while (!(MCG_S & MCG_S_OSCINIT0_MASK)) {};  //等待晶振稳定
    //等待参考时钟状态位清零
    while (MCG_S & MCG_S_IREFST_MASK){}; 
    //等待时钟状态位显示时钟源来自外部参考时钟
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 

    //已进入FBE模式，切换到PBE模式
    //设置PLL分频，为外部时钟源的分频，得到分频时钟
    MCG_C5 = MCG_C5_PRDIV0(Gmcgpll.pllPRDIV);
    //确保MCG_C6处于复位状态，禁止LOLIE、PLL、和时钟控制器，清PLL VCO分频器
    MCG_C6 = 0x0;
    //选择PLL VCO分频器，系统时钟分频器取决于时钟选项
//???    set_sys_dividers(Gsimdiv.coreDIV,Gsimdiv.busDIV,Gsimdiv.FlexBusDIV,Gsimdiv.FlashDIV) ;  
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(Gsimdiv.coreDIV) | SIM_CLKDIV1_OUTDIV4(Gsimdiv.busDIV);

    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(Gmcgpll.pllVDIV) ;
    while (!(MCG_S & MCG_S_PLLST_MASK)){}; // 等待PLL稳定
    while (!(MCG_S & MCG_S_LOCK0_MASK)){};  // 等待PLL锁定
    //已进入PBE模式，切换到PEE模式
    //通过清零CLKS位来进入PEE模式
    // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 &= ~MCG_C1_CLKS_MASK; 
        // Wait for clock status bits to update
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};
   
#if (defined(MKL25Z4)||defined(MKL26Z4)) 
     systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 24)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;
#elif defined(MK60F15)
     systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 16)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;      
#endif
    systemclock *=1000 ;
   return systemclock ;
 }






