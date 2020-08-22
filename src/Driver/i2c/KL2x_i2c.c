/***********************************************************
 * 文件名       ：i2c.c
 * 说明         ：I2C驱动函数，修改自官方例程
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/


#include "KL2x_i2c.h"


unsigned char MasterTransmission;
unsigned char SlaveID;

volatile struct I2C_MemMap *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //定义两个指针数组保存 I2Cx 的地址


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_init
*  功能说明：I2C初始化，设置波特率和启动I2C
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/

void I2C_init(I2Cn i2cn)
{
    uint8 mult;
    if(i2cn == I2C0)
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟

        /* 配置 I2C0功能的 GPIO 接口 */
        if(I2C0_SCL == PTE24)
          Port_alt(I2C0_SCL,ALT5);
        else
          Port_alt(I2C0_SCL,ALT2);
        if(I2C0_SDA == PTE25)
          Port_alt(I2C0_SDA,ALT5);
        else
          Port_alt(I2C0_SDA,ALT2);
    }
    else
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟

        /* 配置 I2C1功能的 GPIO 接口 */
        if(I2C1_SCL == PTE1)
           Port_alt(I2C1_SCL,ALT6) ;
        else
           Port_alt(I2C1_SCL,ALT2) ;

        if(I2C1_SDA == PTE0)
           Port_alt(I2C1_SDA,ALT6) ;
        else
           Port_alt(I2C1_SDA,ALT2) ;
    }
    /* 设置频率 */
    if(bus_clk_khz <= 24000)mult = 0;         //bus 一分频
    else  if(bus_clk_khz <=  48000)mult = 1;  //bus 二分频
    else      mult = 2;                       //bus 四分频

    /* 设置频率 */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(mult) | I2C_F_ICR(17) ;  // I2C Frequency Divider register (I2Cx_F)  I2C分频寄存器 查表获得 ICR 与 SCL_divider 映射关系
    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  
    // SDA hold time = bus period (s) × mul × SDA hold value
    // SCL start hold time = bus period (s) × mul × SCL start hold value
    // SCL stop hold time = bus period (s) × mul × SCL stop hold value

    /* 使能 IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_StartTransmission
*  功能说明：启动 I2C 传输
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            MSmode      读写模式（ MWSR 或 MRSW ）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_StartTransmission (I2Cn i2cn, uint8_t SlaveID, MSmode Mode)
{

    SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址

    /* send start signal */
    i2c_Start(i2cn);

    /* send ID with W/R bit */
    i2c_write_byte(i2cn, SlaveID);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：Pause
*  功能说明：延时，I2C读写后，需要延时一下
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方基础上增大延时数
*************************************************************************/
void I2C_delay(void)
{
    uint16_t n;
    for(n = 1; n < 20000; n++)      
    {
        asm("nop");
    }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_ReadAddr
*  功能说明：读取I2C设备指定地址寄存器的数据
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*  函数返回：从机寄存器的数据
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
uint8_t I2C_ReadAddr(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr)
{
    uint8_t result;

    /* Send Slave Address */
    I2C_StartTransmission (i2cn, SlaveID, MWSR);
    i2c_Wait(i2cn);

    /* Write Register Address */
    i2c_write_byte(i2cn, Addr);
    i2c_Wait(i2cn);

    /* Do a repeated start */
    i2c_RepeatedStart(i2cn);

    /* Send Slave Address */
    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );
    i2c_Wait(i2cn);

    /* Put in Rx Mode */
    i2c_PutinRxMode(i2cn);

    /* Turn off ACK since this is second to last byte being read*/
    i2c_DisableAck(i2cn); //不应答

    /* Dummy read 虚假读取*/
    result = I2C_D_REG(I2Cx[i2cn]);
    i2c_Wait(i2cn);

    /* Send stop since about to read last byte */
    i2c_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_WriteData1
*  功能说明：写入一个字节数据到I2C设备指定寄存器地址
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*            Data        数据
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_WriteData1(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //启动传输
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //写地址
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Data);                    //写数据
    i2c_Wait(i2cn);

    i2c_Stop(i2cn);

    I2C_delay();                                        //延时太短的话，可能写出错
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_WriteDataN
*  功能说明：写入一个字节数据到I2C设备指定寄存器地址
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*            Data        数据
*           leng         数据长度
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_WriteDataN(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t *Data,uint16 leng )
{
  uint16 dataleng ;  
  /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //启动传输
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //写地址
    i2c_Wait(i2cn);
    for(dataleng = 0 ; dataleng < leng ;leng ++){
       i2c_write_byte(i2cn, *Data);                    //写数据
        Data ++ ;
        i2c_Wait(i2cn);
      }

    i2c_Stop(i2cn);

    I2C_delay();                                        //延时太短的话，可能写出错
}