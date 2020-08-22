/***********************************************************
 * 文件名       ：PTI.h
 * 说明         ：PTI初始化
 * 作者         ：野火嵌入式开发工作室/landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef _KL2X_PIT_H_
#define _KL2X_PIT_H_
#include "include.h"


typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;


void        pit_init(PITn, uint32_t cnt);                                               //初始化PITn，并设置定时时间(单位为bus时钟周期)
#define     pit_init_ms(PITn,ms)          pit_init(PITn,ms * bus_clk_khz);         //初始化PITn，并设置定时时间(单位为 ms)
#define     pit_init_us(PITn,us)          pit_init(PITn,us * bus_clk_khz/1000);    //初始化PITn，并设置定时时间(单位为 us)
#define     pit_init_ns(PITn,ns)          pit_init(PITn,ns * bus_clk_khz/1000000); //初始化PITn，并设置定时时间(单位为 ns)

#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn) |= PIT_TFLG_TIF_MASK    //清中断标志


void        pit_dma_init(PITn pitn, uint32_t cnt);
#define     pit_dma_init_ms(PITn,ms)     pit_dma_init(PITn,ms * Gbuscl_KH)

#endif  //_PIT_H_