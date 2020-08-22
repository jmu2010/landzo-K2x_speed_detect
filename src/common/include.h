/***********************************************************
 * 文件名       ：include.h
 * 说明         ：驱动模块包含文件
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "port.h"
#include  "isr.h"
#include  "KL2x_gpio.h"
#include  "KL2x_delay.h"
#include  "KL2x_flash.h" 
#include  "KL2x_PIT.h"       // 周期中断计时器
#include  "KL2x_adc.h"       // ADC模块
#include  "KL2x_uart.h"      // 串口
#include  "KL2x_lptmr.h"     // 低功耗定时器(延时)
#include  "KL2x_TPM.h"       // FTM模块
#include  "KL2x_spi.h" 
#include  "KL2x_i2c.h" 
#include  "KL2x_dma.h"       //DMA模块



#include "Landzo_Calculation.h" // 摄像头
#include "Landzo_CCD.h"         // CCD
#include "Landzo_Electromagnetism.h"
#include "Landzo_GyroAccelerate.h"
#include "Landzo_Key.h"
#include "Landzo_MeasureSpeed.h"
#include "Landzo_Motor.h"
#include "Landzo_OLED.h"
#include "Landzo_Predator.h"
#include "Landzo_servomotor.h"  

/*



   
    

#include  "landzo_sd.h"  
*/




#endif  //__INCLUDE_H__
