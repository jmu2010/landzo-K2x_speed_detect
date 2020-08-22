/***********************************************************
 * 文件名       ：systemclock.h
 * 说明         ：系统时钟源定义
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 * 论坛         ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_                        /**< Symbol preventing repeated inclusion */






///*!< Macro to enable all interrupts. */
//#define EnableInterrupts asm(" CPSIE i");
//
///*!< Macro to disable all interrupts. */
//#define DisableInterrupts asm(" CPSID i");
/***************
外部晶振 单位 M
***************/

#define OSCCLK    50 

/**************
system频率定义
***************/

#define  SYSTEMCLOCK   PLL48


#endif  
