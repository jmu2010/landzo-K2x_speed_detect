/***********************************************************
 * �ļ���       ��KL2X_SPI.h
 * ˵��         ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://bbs.landzo.com
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/


#ifndef _KL2X_SPI_H_
#define _KL2X_SPI_H_
#include "include.h"

#ifndef NULL
  #define NULL 0u
#endif
extern volatile struct SPI_MemMap *SPIx[2];

//�������ӻ�ģʽ
typedef enum SPI_CFG
{
  MASTER,    
  SLAVE      
}SPI_CFG;

//����SPIģ���
typedef enum SPIn
{        
    SPI0,    
    SPI1,           
}SPIn;

#define SPI_TX_WAIT(SPIn)     while(!(SPI_S_REG(SPIx[spin]) & SPI_S_SPTEF_MASK));     //�ȴ����� ���
#define SPI_RX_WAIT(SPIn)     while(!(SPI_S_REG(SPIx[SPIn]) & SPI_S_SPRF_MASK));     //�ȴ����� FIFOΪ�ǿ�


void spi_init(SPIn spin,SPI_CFG mode, uint32  baudKH) ;
uint8 spi_wr_byte(SPIn spin,uint8 data);
void spi_wr_stream(SPIn spin,uint8 sent_buff[],uint32 len);
uint8 spi_receivebyte(SPIn spin);
uint8 spi_receiveN(SPIn spin,uint_8 *data,uint_32 len);
void spi_irq_EN(SPIn);
void spi_irq_DIS(SPIn);

#endif  // _SPI_H_