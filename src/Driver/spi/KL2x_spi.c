/***********************************************************
 * 文件名       ：KL2x_spi.c
 * 说明         ：
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/
#include "KL2x_spi.h"

volatile struct SPI_MemMap *SPIx[2]={SPI0_BASE_PTR,SPI1_BASE_PTR}; //定义两个指针数组保存 SPIx 的地址


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_init
*  功能说明：sSPI初始化
*  参数说明：spin：     SPI通道号。
*            SPI_CFG  模式选择
*            baudKH     波特率
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
  //使能SPI模块时钟，配置SPI引脚功能
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
    //主机模式
    SPI0_C1 = 0x00;
    SPI_C1_REG(SPIx[spin]) |= SPI_C1_MSTR_MASK;    // 配置为主机模式            
                                 
    SPI_C1_REG(SPIx[spin]) =( 0
                              |SPI_C1_SPE_MASK                     // 使能SPI模块
                              |SPI_C1_SSOE_MASK
                            //| SPI_C1_CPOL_MASK                   //时钟极性，1表示 SCK 不活跃状态为高电平,   NRF24L01 不活跃为低电平
                            //| SPI_C1_CPHA_MASK                   //数据在SCK上升沿改变（输出），在下降沿被捕捉（输入读取）。如果是0，则反之。  w25x16在上升沿读取数据；NRF24L01在上升沿读取数据                                                                   
                            //| SPI_C1_LSBFE_MASK                  //1为低位在前。
                            );
    SPI0_C2 = 0x00;
    SPI_C2_REG(SPIx[spin]) |= SPI_C2_MODFEN_MASK;                  //与SPI0中C1的SSOE配合，把从机选择配置为自动模式。
    SPI_BR_REG(SPIx[spin]) =( 0
                              |SPI_BR_SPPR(spiSPPR)               //波特率分频器 ，0~3 对应的分频值Prescaler为 2、3、5、7
                              |SPI_BR_SPR(spiSPR)                 //波特率计数器值 ,当BR<=3,分频Scaler 为 2*（BR+1） ，当BR>=3，分频Scaler 为 2^BR  。BR为4bit
                            );
  }     
  else
  {
    //从机模式
    SPI_C1_REG(SPIx[spin]) = (0 
                              | SPI_C1_SPE_MASK      // 使能SPI模块
                              | SPI_C1_SPIE_MASK     // 打开SPI中断 
                             );
    SPI0_C2 = 0x00;
    SPI_BR_REG(SPIx[spin]) =( 0
                              |SPI_BR_SPPR(spiSPPR)  //波特率分频器 ，0~3 对应的分频值Prescaler为 2、3、5、7
                              |SPI_BR_SPR(spiSPR)    //波特率计数器值 ,当BR<=3,分频Scaler 为 2*（BR+1） ，当BR>=3，分频Scaler 为 2^BR  。BR为4bit
                            );
    enable_irq (SPI1_irq_no);  //开启SPI1中断。  
  }
 
  //清标志位
  SPI_S_REG(SPIx[spin]) = (0
                           | SPI_S_SPTEF_MASK         //End of Queue Flag，发送队列空了，发送完毕                 
                           | SPI_S_SPRF_MASK         //Receive FIFO Overflow Flag，接收FIFO溢出标志位。
                           );     
  
}


/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_wr_byte
*  功能说明：SPI发送一字节数据。
*  参数说明：spin：SPI通道号。
*            data：需要发送的数据。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
  for(i = 0;i < 0xFFF0;i ++)		//在一定时间内发送不能完成，则认为发送失败。
  {
    if(SPI_S_REG(SPIx[spin])&SPI_S_SPTEF_MASK)
    {
      return(1);
    }
  }
  return(0);
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_wr_stream
*  功能说明：SPI发送多字节数据。
*  参数说明：spin：SPI通道号。
*            sent_buff[]：需要发送的数据。
*            len：数据长度。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
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
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_receivebyte
*  功能说明：SPI接收1个字节数据。
*  参数说明：spin：SPI通道号。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
uint_8 spi_receivebyte(SPIn spin)
{
    while(!(SPI_S_REG(SPIx[spin])& SPI_S_SPRF_MASK));	//检测SPI1是否收到了数据。
#if defined(MKL25Z4)
    return SPI_D_REG(SPIx[spin]);
#elif defined(MKL26Z4)
    return SPI_DL_REG(SPIx[spin]);
#endif 
    				
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：spi_receiveN
*  功能说明：SPI接收多个字节数据。
*  参数说明：spin：SPI通道号。
*  函数返回：接收到的数据
*  修改时间：2015-5-15   已测试
*  备    注：
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
//函数名称：spi_irq_EN
//函数参数：spin：SPI通道号。
//函数返回：无
//功能概要：使能SPI中断
//=========================================================================
void spi_irq_EN(SPIn spin)
{
  SPI_C1_REG(SPIx[spin]) |=SPI_C1_SPIE_MASK;     //开启SPI接收中断 
  enable_irq(spin + SPI0_irq_no);			         //开接收引脚的IRQ中断
}

//=========================================================================
//函数名称：spi_irq_DIS
//函数参数：spin：SPI通道号。
//函数返回：无
//功能概要：关闭SPI中断
//=========================================================================
void spi_irq_DIS(SPIn spin)
{
  SPI_C1_REG(SPIx[spin]) &=~SPI_C1_SPIE_MASK;    //关闭SPI接收中断  
  disable_irq(spin + SPI0_irq_no);			                //开接收引脚的IRQ中断
}