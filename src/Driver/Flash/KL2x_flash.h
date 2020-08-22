/***********************************************************
 * 文件名       ：K6x_flash.h
 * 说明         ：flash写入擦除
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef _K6X_FLASH_H
#define _K6X_FLASH_H

//kl25包含128K的程序Flash

#include "common.h"

//static uint32 flash_cmd_launch(void);// 声明函数
__RAMFUN void hw_flash_init(void) ;
__RAMFUN uint8 flash_erase_sector(uint16 sectorNo) ;
__RAMFUN uint8 flash_write(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[]) ;




#endif //_HW_FLASH_H__ 
