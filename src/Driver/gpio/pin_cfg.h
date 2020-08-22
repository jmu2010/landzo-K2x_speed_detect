/***********************************************************
 * �ļ���       ��pin_cfg.h
 * ˵��         ���˿������ļ�
 * ����         ����Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/


#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#include "gpio_cfg.h"


/**********************************  UART   ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ                          ����
#define UART0_RX    PTB16        //PTA1��PTA15��PTD6��PTE21        
#define UART0_TX    PTB17        //PTA2��PTA14��PTD7��PTE20           

#define UART1_RX    PTE1        //PTC3��
#define UART1_TX    PTE0        //PTC4��

#define UART2_RX    PTD2        //PTD4��PTE23
#define UART2_TX    PTD3        //PTD5��PTE22

/**********************************  TPM    ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define TPM0_CH0    PTC1         //PTE24��PTD0��PTA3      PTA3��Ҫ�ã���Jtag��SWD��ͻ��
#define TPM0_CH1    PTC2         //PTE25��PTA4��PTD1
#define TPM0_CH2    PTC3         //PTE29��PTA5��PTD2
#define TPM0_CH3    PTC4         //PTE30��PTD3
#define TPM0_CH4    PTC4         //PTE31��PTD8
#define TPM0_CH5    PTC5         //PTD9��PTA0            PTA0��Ҫ�ã���Jtag��SWD��ͻ��
          

//      ģ��ͨ��    �˿�          ��ѡ��Χ              
#define TPM1_CH0    PTA12        //PTE20��PTB0
#define TPM1_CH1    PTA13        //PTE21��PTB1
//      ģ��ͨ��    �˿�          ��ѡ��Χ              
#define TPM2_CH0    PTA1        //PTE22��PTB2
#define TPM2_CH1    PTA2        //PTE23��PTB3

/**********************************  I2C   ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define I2C0_SCL    PTC8        // PTB0��PTB2��PTC8��PTE24
#define I2C0_SDA    PTC9        // PTB1��PTB3��PTC9��PTE25

#define I2C1_SCL    PTC10       // PTA3��PTC10��PTC1��PTE1
#define I2C1_SDA    PTC11       // PTA4��PTC11��PTC2��PTE0

/**********************************  CAN    ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define CAN0_TX    PTA12        // PTA12��PTB18��
#define CAN0_RX    PTA13        // PTA13��PTB19��

#if (defined(MK60DZ10)||defined(MK60F15))

#define CAN1_TX    PTE24        // PTE24��PTC17��
#define CAN1_RX    PTE25        // PTE25��PTC16��

#endif
/**********************************  SPI   ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define SPI0_PCS0   PTD0        // PTA14
#define SPI0_SCK    PTD1        // PTA15
#define SPI0_SIN    PTD2        // PTA16
#define SPI0_SOUT   PTD3        // PTA17


#define SPI1_PCS0   PTE4        // PTD4��PTB10
#define SPI1_SCK    PTE2        // PTD5��PTB11
#define SPI1_SIN    PTE3        // PTD6��PTB16
#define SPI1_SOUT   PTD7        // PTB17


#endif  //_FIRE_DRIVERS_CFG_H_


