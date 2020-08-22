/***********************************************************
 * 文件名       ：system.c
 * 说明         ：系统频率初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 * 论坛         ：http://bbs.landzo.com
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef SYSTEM_H_
#define SYSTEM_H_                        /**< Symbol preventing repeated inclusion */



#include <stdint.h>




/* Predefined clock setups
   0 ... Default  part configuration
         Multipurpose Clock Generator (MCG) in FEI mode.
         Reference clock source for MCG module: Slow internal reference clock
         Core clock = 20.97152MHz
         Bus clock  = 20.97152MHz
   1 ... Maximum achievable clock frequency configuration
         Multipurpose Clock Generator (MCG) in PEE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 120MHz
         Bus clock  = 60MHz
   2 ... Chip internaly clocked, ready for Very Low Power Run mode.
         Multipurpose Clock Generator (MCG) in BLPI mode.
         Reference clock source for MCG module: Fast internal reference clock
         Core clock = 4MHz
         Bus clock  = 4MHz
   3 ... Chip externally clocked, ready for Very Low Power Run mode.
         Multipurpose Clock Generator (MCG) in BLPE mode.
         Reference clock source for MCG module: RTC oscillator reference clock
         Core clock = 0.032768MHz
         Bus clock  = 0.032768MHz
   4 ... USB clock setup
         Multipurpose Clock Generator (MCG) in PEE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 120MHz
         Bus clock  = 60MHz
 */

/* Define clock source values */

#define CPU_XTAL_CLK_HZ                50000000u           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_XTAL32k_CLK_HZ             32768u              /* Value of the external 32k crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768u              /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000u            /* Value of the fast internal oscillator clock frequency in Hz  */
#define CPU_INT_IRC_CLK_HZ             48000000u           /* Value of the 48M internal oscillator clock frequency in Hz  */

/* RTC oscillator setting */
/* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0,CLKO=1,OSCE=1,WPS=0,UM=0,SUP=0,WPE=0,SWR=0 */
#define SYSTEM_RTC_CR_VALUE            0x0300U             /* RTC_CR */

/* Low power mode enable */
/* SMC_PMPROT: AVLP=1,ALLS=1,AVLLS=1 */
#define SYSTEM_SMC_PMPROT_VALUE        0x2AU               /* SMC_PMPROT */

  
typedef struct{
    uint8   pllPRDIV;            //外部晶振除数因子
    uint8   pllVDIV;             //外部晶振乘积因子
  }MCGPLL ;

typedef struct{
    uint8   coreDIV;            //Core除数因子
    uint8   busDIV;             //bus除数因子
    uint8   FlexBusDIV;         //FlexBus除数因子
    uint8   FlashDIV;           //Flash除数因子
  }SIMDIV ;

typedef enum system_option
{
    PLL48    = 48,
    PLL50    = 50,
    PLL60    = 60,
    PLL70    = 70,
    PLL80    = 80,
    PLL90    = 90,
    PLL100   = 100,
    PLL110   = 110,
    PLL120   = 120,
    PLL130   = 130,
    PLL140   = 140,
    PLL150   = 150,
    PLL160   = 160,
    PLL170   = 170,
    PLL180   = 180,
    PLL190   = 190,
    PLL200   = 200, 
    PLL210   = 210,
    PLL220   = 220,
    PLL230   = 230,
    PLL240   = 240,
    PLL250   = 250,
    PLL260   = 260,
    PLL270   = 270,
    PLL280   = 280,
    PLL290   = 290,
    PLL300   = 300,
} system_option;

#define BUSCLOCKMAX          25     //bus clock最大频率
//#define FLEXCLOCKMAX         50   //FLEX clock最大频率
#define FLASHCLOCKMAX        25    //FLASH clock最大频率

uint32 system_init(system_option SYSPLL) ;
//__RAMFUN void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4) ;
#endif  
