/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：arm_cm4.h
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/

#ifndef _COMMON_H_    //防止重复定义（Common_H  开头)
#define _COMMON_H_

//1．芯片寄存器映射文件及处理器内核属性文件
#if defined(MKL25Z4)
#include "MKL25Z4.h"      // 包含芯片头文件
#elif defined(MKL26Z4)
#include "MKL26Z4.h"      // 包含芯片头文件
#else
  #error NO Kl2x defined
#endif


//2．定义开关总中断

#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级



//3．位操作宏函数（置位、清位、获得寄存器一位的状态）
#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    //置寄存器的一位
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  //清寄存器的一位
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  //获得寄存器一位的状态

//4．重定义基本数据类型（类型别名宏定义）
typedef unsigned char        uint_8;   // 无符号8位数，字节
typedef unsigned short int   uint_16;  // 无符号16位数，字
typedef unsigned long int    uint_32;  // 无符号32位数，长字
typedef char                 int_8;    // 有符号8位数
typedef short int            int_16;   // 有符号16位数
typedef int                  int_32;   // 有符号32位数

#define u8   uint_8
#define u16  uint_16
#define u32  uint_32

//不优化类型
typedef volatile uint_8      vuint_8;  // 不优化无符号8位数，字节
typedef volatile uint_16     vuint_16; // 不优化无符号16位数，字
typedef volatile uint_32     vuint_32; // 不优化无符号32位数，长字
typedef volatile int_8       vint_8;   // 不优化有符号8位数
typedef volatile int_16      vint_16;  // 不优化有符号16位数
typedef volatile int_32      vint_32;  // 不优化有符号32位数

typedef	union
{
	uint_32	DW;
	uint_16	W[2];
	uint_8	B[4];
	struct
	{
		uint_32 b0:1; uint_32 b1:1; uint_32 b2:1; uint_32 b3:1; uint_32 b4:1; uint_32 b5:1; uint_32 b6:1; uint_32 b7:1; 
		uint_32 b8:1; uint_32 b9:1; uint_32 b10:1;uint_32 b11:1;uint_32 b12:1;uint_32 b13:1;uint_32 b14:1;uint_32 b15:1;
		uint_32 b16:1;uint_32 b17:1;uint_32 b18:1;uint_32 b19:1;uint_32 b20:1;uint_32 b21:1;uint_32 b22:1;uint_32 b23:1;
		uint_32 b24:1;uint_32 b25:1;uint_32 b26:1;uint_32 b27:1;uint_32 b28:1;uint_32 b29:1;uint_32 b30:1;uint_32 b31:1;
	};
}Bittyp;		//sizeof(Dtype)	为 4 


/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
	#define 	__RAMFUN	__ramfunc	//IAR 用 __ramfunc 来声明
#else
	#define 	__RAMFUN
#endif


//喂狗
#define WDI_FEED() {SIM_SRVCOP = 0x55;	SIM_SRVCOP = 0xAA;}



// 7、包含相关头文件
#include "assert.h"
#include "io.h"
#include "memtest.h"
#include "queue.h"
#include "stdlib.h"
#include "uif.h"
#include "typedef.h"
//#include "tower.h"
#include "arm_cm0.h"
#include "vectors.h"
#include "startup.h"

/***********************
系统配置的时钟定义
***********************/
#include "systemclock.h"   
#include "gpio_cfg.h"
#include "port.h"
#include "pin_cfg.h"


#define TRUE                             (1)
#define FALSE                            (0)
#define INC_IRQ                         (-16)
#define HIGH                             (1)
#define LOW                              (0)

extern uint32   core_clk_khz ;    //系统时钟
extern uint32   bus_clk_khz ;      //bus时钟

#endif //防止重复定义（Common_H  结尾)
