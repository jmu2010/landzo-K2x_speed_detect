/***********************************************************
 * 文件名       ：KL2X_SPI.h
 * 说明         ：
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://bbs.landzo.com
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/


#ifndef _KL2X_SPI_H_
#define _KL2X_SPI_H_
#include "include.h"

#ifndef NULL
  #define NULL 0u
#endif
extern volatile struct SPI_MemMap *SPIx[2];

//定义主从机模式
typedef enum SPI_CFG
{
  MASTER,    
  SLAVE      
}SPI_CFG;

//定义SPI模块号
typedef enum SPIn
{        
    SPI0,    
    SPI1,           
}SPIn;

#define SPI_TX_WAIT(SPIn)     while(!(SPI_S_REG(SPIx[spin]) & SPI_S_SPTEF_MASK));     //等待发送 完成
#define SPI_RX_WAIT(SPIn)     while(!(SPI_S_REG(SPIx[SPIn]) & SPI_S_SPRF_MASK));     //等待发送 FIFO为非空


void spi_init(SPIn spin,SPI_CFG mode, uint32  baudKH) ;
uint8 spi_wr_byte(SPIn spin,uint8 data);
void spi_wr_stream(SPIn spin,uint8 sent_buff[],uint32 len);
uint8 spi_receivebyte(SPIn spin);
uint8 spi_receiveN(SPIn spin,uint_8 *data,uint_32 len);
void spi_irq_EN(SPIn);
void spi_irq_DIS(SPIn);

#endif  // _SPI_H_