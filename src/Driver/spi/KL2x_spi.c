/***********************************************************
 * �ļ���       ��KL2x_spi.c
 * ˵��         ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/
#include "KL2x_spi.h"

volatile struct SPI_MemMap *SPIx[2]={SPI0_BASE_PTR,SPI1_BASE_PTR}; //��������ָ�����鱣�� SPIx �ĵ�ַ


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_init
*  ����˵����sSPI��ʼ��
*  ����˵����spin��     SPIͨ���š�
*            SPI_CFG  ģʽѡ��
*            baudKH     ������
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/

void spi_init(SPIn spin,SPI_CFG mode, uint32  baudKH)
{
  uint16 spiSPPR,spiSPR;
  uint32 busclcok_kH ;
  uint32 spicout ;
  int32 temp0,temp1,temp2,tempmin;
  uint32 BRS[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
  uint8  PRS[] = {2,3,5,7};

  busclcok_kH = bus_clk_khz / baudKH; 
  tempmin = 0x7fff ;
  for(temp0 = 0 ; temp0 < 4 ;temp0 ++)
  {
    spicout =  busclcok_kH / PRS[temp0] ;  
    for(temp1 = 0 ;temp1 < 0x10 ;temp1 ++)
    {
      temp2 = (uint16)spicout - BRS[temp1];
      if(temp2 < 0) temp2 = -temp2;  
      if(tempmin > temp2)
      {
        tempmin = temp2;
        spiSPPR = temp0; // SPPR
        spiSPR = temp1;  // SPR
      }
    }
  }
  //ʹ��SPIģ��ʱ�ӣ�����SPI���Ź���
  if(spin == 0)
  {
    SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;
    Port_init(SPI0_SIN,ALT2);                              //SIN
    Port_init(SPI0_SOUT,PORT_PCR_DSE_MASK|ALT2);           //SOUT
    Port_init(SPI0_SCK,PORT_PCR_DSE_MASK|ALT2);            //SCK
    Port_init(SPI0_PCS0,PORT_PCR_DSE_MASK|ALT2);           //PCS0
    
//    PORTD_PCR0 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//PCS0
//    PORTD_PCR1 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//SCK
//    PORTD_PCR2 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK;//SOUT
//    PORTD_PCR3 = 0 | PORT_PCR_MUX(0x2);                    //SIN
  }
  else if(spin == 1)
  {
    SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;
    Port_init(SPI1_SIN,ALT2);                              //SIN
    Port_init(SPI1_SOUT,PORT_PCR_DSE_MASK|ALT2);           //SOUT
    Port_init(SPI1_SCK,PORT_PCR_DSE_MASK|ALT2);            //SCK
    Port_init(SPI1_PCS0,PORT_PCR_DSE_MASK|ALT2);           //PCS0
    
//    PORTE_PCR1 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SOUT
//    PORTE_PCR2 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //SCK
//    PORTE_PCR3 = 0 | PORT_PCR_MUX(0x2);                     //SIN
//    PORTE_PCR4 = 0 | PORT_PCR_MUX(0x2) | PORT_PCR_DSE_MASK; //PCS0
  }
  if(mode == MASTER)
  {
    //����ģʽ
    SPI0_C1 = 0x00;
    SPI_C1_REG(SPIx[spin]) |= SPI_C1_MSTR_MASK;    // ����Ϊ����ģʽ            
                                 
    SPI_C1_REG(SPIx[spin]) =( 0
                              |SPI_C1_SPE_MASK                     // ʹ��SPIģ��
                              |SPI_C1_SSOE_MASK
                            //| SPI_C1_CPOL_MASK                   //ʱ�Ӽ��ԣ�1��ʾ SCK ����Ծ״̬Ϊ�ߵ�ƽ,   NRF24L01 ����ԾΪ�͵�ƽ
                            //| SPI_C1_CPHA_MASK                   //������SCK�����ظı䣨����������½��ر���׽�������ȡ���������0����֮��  w25x16�������ض�ȡ���ݣ�NRF24L01�������ض�ȡ����                                                                   
                            //| SPI_C1_LSBFE_MASK                  //1Ϊ��λ��ǰ��
                            );
    SPI0_C2 = 0x00;
    SPI_C2_REG(SPIx[spin]) |= SPI_C2_MODFEN_MASK;                  //��SPI0��C1��SSOE��ϣ��Ѵӻ�ѡ������Ϊ�Զ�ģʽ��
    SPI_BR_REG(SPIx[spin]) =( 0
                              |SPI_BR_SPPR(spiSPPR)               //�����ʷ�Ƶ�� ��0~3 ��Ӧ�ķ�ƵֵPrescalerΪ 2��3��5��7
                              |SPI_BR_SPR(spiSPR)                 //�����ʼ�����ֵ ,��BR<=3,��ƵScaler Ϊ 2*��BR+1�� ����BR>=3����ƵScaler Ϊ 2^BR  ��BRΪ4bit
                            );
  }     
  else
  {
    //�ӻ�ģʽ
    SPI_C1_REG(SPIx[spin]) = (0 
                              | SPI_C1_SPE_MASK      // ʹ��SPIģ��
                              | SPI_C1_SPIE_MASK     // ��SPI�ж� 
                             );
    SPI0_C2 = 0x00;
    SPI_BR_REG(SPIx[spin]) =( 0
                              |SPI_BR_SPPR(spiSPPR)  //�����ʷ�Ƶ�� ��0~3 ��Ӧ�ķ�ƵֵPrescalerΪ 2��3��5��7
                              |SPI_BR_SPR(spiSPR)    //�����ʼ�����ֵ ,��BR<=3,��ƵScaler Ϊ 2*��BR+1�� ����BR>=3����ƵScaler Ϊ 2^BR  ��BRΪ4bit
                            );
    enable_irq (SPI1_irq_no);  //����SPI1�жϡ�  
  }
 
  //���־λ
  SPI_S_REG(SPIx[spin]) = (0
                           | SPI_S_SPTEF_MASK         //End of Queue Flag�����Ͷ��п��ˣ��������                 
                           | SPI_S_SPRF_MASK         //Receive FIFO Overflow Flag������FIFO�����־λ��
                           );     
  
}


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_wr_byte
*  ����˵����SPI����һ�ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*            data����Ҫ���͵����ݡ�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
uint8 spi_wr_byte(SPIn spin,uint8 data)
{  
  uint_32 i;
  SPI_TX_WAIT(spin);
#if defined(MKL25Z4)
  SPI_D_REG(SPIx[spin]) = data;
#elif defined(MKL26Z4)
  SPI_DL_REG(SPIx[spin]) = data;
#endif
  for(i = 0;i < 0xFFF0;i ++)		//��һ��ʱ���ڷ��Ͳ�����ɣ�����Ϊ����ʧ�ܡ�
  {
    if(SPI_S_REG(SPIx[spin])&SPI_S_SPTEF_MASK)
    {
      return(1);
    }
  }
  return(0);
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_wr_stream
*  ����˵����SPI���Ͷ��ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*            sent_buff[]����Ҫ���͵����ݡ�
*            len�����ݳ��ȡ�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
void spi_wr_stream(SPIn spin,uint8 sent_buff[],uint32 len)
{
  uint32 i;
  for(i = 0;i < len;i ++)
  {
    while(!SPI_S_REG(SPIx[spin])&SPI_S_SPTEF_MASK);
#if defined(MKL25Z4)
    SPI_D_REG(SPIx[spin]) = sent_buff[i];
#elif defined(MKL26Z4)
    SPI_DL_REG(SPIx[spin]) = sent_buff[i];
#endif   
  }
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_receivebyte
*  ����˵����SPI����1���ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
uint_8 spi_receivebyte(SPIn spin)
{
    while(!(SPI_S_REG(SPIx[spin])& SPI_S_SPRF_MASK));	//���SPI1�Ƿ��յ������ݡ�
#if defined(MKL25Z4)
    return SPI_D_REG(SPIx[spin]);
#elif defined(MKL26Z4)
    return SPI_DL_REG(SPIx[spin]);
#endif 
    				
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�spi_receiveN
*  ����˵����SPI���ն���ֽ����ݡ�
*  ����˵����spin��SPIͨ���š�
*  �������أ����յ�������
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע��
*************************************************************************/
uint8 spi_receiveN(SPIn spin,uint_8 *data,uint_32 len)
{
    uint_32 i = 0;
    while(i < len)
    {
      if(SPI_S_REG(SPIx[spin]) & SPI_S_SPRF_MASK)
      {      
      #if defined(MKL25Z4)
          data[i] = SPI_D_REG(SPIx[spin]);
          i++;
      #elif defined(MKL26Z4)
          data[i] = SPI_DL_REG(SPIx[spin]);
          i++;
      #endif     
      }
    }
    return 1;
}
//=========================================================================
//�������ƣ�spi_irq_EN
//����������spin��SPIͨ���š�
//�������أ���
//���ܸ�Ҫ��ʹ��SPI�ж�
//=========================================================================
void spi_irq_EN(SPIn spin)
{
  SPI_C1_REG(SPIx[spin]) |=SPI_C1_SPIE_MASK;     //����SPI�����ж� 
  enable_irq(spin + SPI0_irq_no);			         //���������ŵ�IRQ�ж�
}

//=========================================================================
//�������ƣ�spi_irq_DIS
//����������spin��SPIͨ���š�
//�������أ���
//���ܸ�Ҫ���ر�SPI�ж�
//=========================================================================
void spi_irq_DIS(SPIn spin)
{
  SPI_C1_REG(SPIx[spin]) &=~SPI_C1_SPIE_MASK;    //�ر�SPI�����ж�  
  disable_irq(spin + SPI0_irq_no);			                //���������ŵ�IRQ�ж�
}