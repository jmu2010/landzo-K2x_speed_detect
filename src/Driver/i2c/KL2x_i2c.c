/***********************************************************
 * �ļ���       ��i2c.c
 * ˵��         ��I2C�����������޸��Թٷ�����
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/


#include "KL2x_i2c.h"


unsigned char MasterTransmission;
unsigned char SlaveID;

volatile struct I2C_MemMap *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //��������ָ�����鱣�� I2Cx �ĵ�ַ


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�I2C_init
*  ����˵����I2C��ʼ�������ò����ʺ�����I2C
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/

void I2C_init(I2Cn i2cn)
{
    uint8 mult;
    if(i2cn == I2C0)
    {
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //���� I2C0ʱ��

        /* ���� I2C0���ܵ� GPIO �ӿ� */
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
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //���� I2C1ʱ��

        /* ���� I2C1���ܵ� GPIO �ӿ� */
        if(I2C1_SCL == PTE1)
           Port_alt(I2C1_SCL,ALT6) ;
        else
           Port_alt(I2C1_SCL,ALT2) ;

        if(I2C1_SDA == PTE0)
           Port_alt(I2C1_SDA,ALT6) ;
        else
           Port_alt(I2C1_SDA,ALT2) ;
    }
    /* ����Ƶ�� */
    if(bus_clk_khz <= 24000)mult = 0;         //bus һ��Ƶ
    else  if(bus_clk_khz <=  48000)mult = 1;  //bus ����Ƶ
    else      mult = 2;                       //bus �ķ�Ƶ

    /* ����Ƶ�� */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(mult) | I2C_F_ICR(17) ;  // I2C Frequency Divider register (I2Cx_F)  I2C��Ƶ�Ĵ��� ����� ICR �� SCL_divider ӳ���ϵ
    // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)  
    // SDA hold time = bus period (s) �� mul �� SDA hold value
    // SCL start hold time = bus period (s) �� mul �� SCL start hold value
    // SCL stop hold time = bus period (s) �� mul �� SCL stop hold value

    /* ʹ�� IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�I2C_StartTransmission
*  ����˵�������� I2C ����
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            MSmode      ��дģʽ�� MWSR �� MRSW ��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_StartTransmission (I2Cn i2cn, uint8_t SlaveID, MSmode Mode)
{

    SlaveID = ( SlaveID << 1 ) | Mode ;            //ȷ��д��ַ�Ͷ���ַ

    /* send start signal */
    i2c_Start(i2cn);

    /* send ID with W/R bit */
    i2c_write_byte(i2cn, SlaveID);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�Pause
*  ����˵������ʱ��I2C��д����Ҫ��ʱһ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ�������������ʱ��
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
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�I2C_ReadAddr
*  ����˵������ȡI2C�豸ָ����ַ�Ĵ���������
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*  �������أ��ӻ��Ĵ���������
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
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
    i2c_DisableAck(i2cn); //��Ӧ��

    /* Dummy read ��ٶ�ȡ*/
    result = I2C_D_REG(I2Cx[i2cn]);
    i2c_Wait(i2cn);

    /* Send stop since about to read last byte */
    i2c_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�I2C_WriteData1
*  ����˵����д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*            Data        ����
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_WriteData1(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //��������
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //д��ַ
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Data);                    //д����
    i2c_Wait(i2cn);

    i2c_Stop(i2cn);

    I2C_delay();                                        //��ʱ̫�̵Ļ�������д����
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�I2C_WriteDataN
*  ����˵����д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
*  ����˵����I2Cn        ģ��ţ�I2C0��I2C1��
*            SlaveID     7λ�ӻ���ַ
*            Addr        �ӻ��ļĴ�����ַ
*            Data        ����
*           leng         ���ݳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void I2C_WriteDataN(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t *Data,uint16 leng )
{
  uint16 dataleng ;  
  /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //��������
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //д��ַ
    i2c_Wait(i2cn);
    for(dataleng = 0 ; dataleng < leng ;leng ++){
       i2c_write_byte(i2cn, *Data);                    //д����
        Data ++ ;
        i2c_Wait(i2cn);
      }

    i2c_Stop(i2cn);

    I2C_delay();                                        //��ʱ̫�̵Ļ�������д����
}