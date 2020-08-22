/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��arm_cm4.h
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��landzo���ӿ�����
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/

#ifndef _COMMON_H_    //��ֹ�ظ����壨Common_H  ��ͷ)
#define _COMMON_H_

//1��оƬ�Ĵ���ӳ���ļ����������ں������ļ�
#if defined(MKL25Z4)
#include "MKL25Z4.h"      // ����оƬͷ�ļ�
#elif defined(MKL26Z4)
#include "MKL26Z4.h"      // ����оƬͷ�ļ�
#else
  #error NO Kl2x defined
#endif


//2�����忪�����ж�

#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //ʹ��IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //��ֹIRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //�������ȼ�



//3��λ�����꺯������λ����λ����üĴ���һλ��״̬��
#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    //�üĴ�����һλ
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  //��Ĵ�����һλ
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  //��üĴ���һλ��״̬

//4���ض�������������ͣ����ͱ����궨�壩
typedef unsigned char        uint_8;   // �޷���8λ�����ֽ�
typedef unsigned short int   uint_16;  // �޷���16λ������
typedef unsigned long int    uint_32;  // �޷���32λ��������
typedef char                 int_8;    // �з���8λ��
typedef short int            int_16;   // �з���16λ��
typedef int                  int_32;   // �з���32λ��

#define u8   uint_8
#define u16  uint_16
#define u32  uint_32

//���Ż�����
typedef volatile uint_8      vuint_8;  // ���Ż��޷���8λ�����ֽ�
typedef volatile uint_16     vuint_16; // ���Ż��޷���16λ������
typedef volatile uint_32     vuint_32; // ���Ż��޷���32λ��������
typedef volatile int_8       vint_8;   // ���Ż��з���8λ��
typedef volatile int_16      vint_16;  // ���Ż��з���16λ��
typedef volatile int_32      vint_32;  // ���Ż��з���32λ��

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
}Bittyp;		//sizeof(Dtype)	Ϊ 4 


/*
 * �������е�RAM�ĺ���
 */
#if defined(__ICCARM__)
	#define 	__RAMFUN	__ramfunc	//IAR �� __ramfunc ������
#else
	#define 	__RAMFUN
#endif


//ι��
#define WDI_FEED() {SIM_SRVCOP = 0x55;	SIM_SRVCOP = 0xAA;}



// 7���������ͷ�ļ�
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
ϵͳ���õ�ʱ�Ӷ���
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

extern uint32   core_clk_khz ;    //ϵͳʱ��
extern uint32   bus_clk_khz ;      //busʱ��

#endif //��ֹ�ظ����壨Common_H  ��β)
