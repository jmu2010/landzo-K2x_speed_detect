/***********************************************************
 * 文件名       ：pin_cfg.h
 * 说明         ：端口配置文件
 * 作者         ：：野火嵌入式开发工作室/landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/


#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#include "gpio_cfg.h"


/**********************************  UART   ***************************************/

//      模块通道    端口          可选范围                          建议
#define UART0_RX    PTB16        //PTA1、PTA15、PTD6、PTE21        
#define UART0_TX    PTB17        //PTA2、PTA14、PTD7、PTE20           

#define UART1_RX    PTE1        //PTC3、
#define UART1_TX    PTE0        //PTC4、

#define UART2_RX    PTD2        //PTD4、PTE23
#define UART2_TX    PTD3        //PTD5、PTE22

/**********************************  TPM    ***************************************/

//      模块通道    端口          可选范围              建议
#define TPM0_CH0    PTC1         //PTE24、PTD0、PTA3      PTA3不要用（与Jtag、SWD冲突）
#define TPM0_CH1    PTC2         //PTE25、PTA4、PTD1
#define TPM0_CH2    PTC3         //PTE29、PTA5、PTD2
#define TPM0_CH3    PTC4         //PTE30、PTD3
#define TPM0_CH4    PTC4         //PTE31、PTD8
#define TPM0_CH5    PTC5         //PTD9、PTA0            PTA0不要用（与Jtag、SWD冲突）
          

//      模块通道    端口          可选范围              
#define TPM1_CH0    PTA12        //PTE20、PTB0
#define TPM1_CH1    PTA13        //PTE21、PTB1
//      模块通道    端口          可选范围              
#define TPM2_CH0    PTA1        //PTE22、PTB2
#define TPM2_CH1    PTA2        //PTE23、PTB3

/**********************************  I2C   ***************************************/

//      模块通道    端口          可选范围              建议
#define I2C0_SCL    PTC8        // PTB0、PTB2、PTC8、PTE24
#define I2C0_SDA    PTC9        // PTB1、PTB3、PTC9、PTE25

#define I2C1_SCL    PTC10       // PTA3、PTC10、PTC1、PTE1
#define I2C1_SDA    PTC11       // PTA4、PTC11、PTC2、PTE0

/**********************************  CAN    ***************************************/

//      模块通道    端口          可选范围              建议
#define CAN0_TX    PTA12        // PTA12、PTB18、
#define CAN0_RX    PTA13        // PTA13、PTB19、

#if (defined(MK60DZ10)||defined(MK60F15))

#define CAN1_TX    PTE24        // PTE24、PTC17、
#define CAN1_RX    PTE25        // PTE25、PTC16、

#endif
/**********************************  SPI   ***************************************/

//      模块通道    端口          可选范围              建议
#define SPI0_PCS0   PTD0        // PTA14
#define SPI0_SCK    PTD1        // PTA15
#define SPI0_SIN    PTD2        // PTA16
#define SPI0_SOUT   PTD3        // PTA17


#define SPI1_PCS0   PTE4        // PTD4、PTB10
#define SPI1_SCK    PTE2        // PTD5、PTB11
#define SPI1_SIN    PTE3        // PTD6、PTB16
#define SPI1_SOUT   PTD7        // PTB17


#endif  //_FIRE_DRIVERS_CFG_H_


