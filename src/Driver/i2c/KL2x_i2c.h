/***********************************************************
 * 文件名       ：i2c.h
 * 说明         ：I2C驱动函数，修改自官方例程
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef _KL2X_I2C_H_
#define _KL2X_I2C_H_

#include  "include.h"

typedef enum I2Cn
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn;



typedef enum MSmode
{
    MWSR =   0x00,  /* Master write  */
    MRSW =   0x01,   /* Master read */
} MSmode;

#define i2c_DisableAck(I2Cn)        I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TXAK_MASK

//
#define i2c_RepeatedStart(I2Cn)     I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_RSTA_MASK


//启动信号
#define i2c_Start(I2Cn)             I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TX_MASK;\
                                    I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_MST_MASK

//暂停信号
#define i2c_Stop(I2Cn)              I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_MST_MASK;\
                                    I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//进入接收模式(应答)
#define i2c_EnterRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK;\
                                    I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TXAK_MASK
//进入接收模式(不应答)
#define i2c_PutinRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//等待 I2C0_S
#define i2c_Wait(I2Cn)              while(( I2C_S_REG(I2Cx[I2Cn]) & I2C_S_IICIF_MASK)==0) {} \
                                    I2C_S_REG(I2Cx[I2Cn]) |= I2C_S_IICIF_MASK;

//写一个字节
#define i2c_write_byte(I2Cn,data)   I2C_D_REG(I2Cx[I2Cn]) = data
void I2C_WriteData1(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data) ;
void I2C_init(I2Cn i2cn);                                         //初始化I2C
void  I2C_WriteAddr(I2Cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data);      //读取地址里的内容
uint8    I2C_ReadAddr(I2Cn, uint8_t SlaveID, uint8_t Addr);                //往地址里写入内容
void  I2C_StartTransmission (I2Cn, uint8_t SlaveID, MSmode);       //启动传输
void I2C_WriteDataN(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t *Data,uint16 leng ) ;


#endif