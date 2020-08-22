/***********************************************************
 * 文件名       ：K6x_uart.c
 * 说明         ：串口初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include "KL2x_uart.h"


volatile struct UART_MemMap *UARTx[2] = {UART1_BASE_PTR, UART2_BASE_PTR}; //定义三个指针数组保存 UARTx 的地址



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_init
*  功能说明：初始化串口，设置波特率
*  参数说明：UARTn       模块号（UART0~UART2）
*            baud        波特率，如9600、19200、56000、115200等
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方例程上修改
*************************************************************************/
void uart_init (UARTn uratn, uint32_t baud)
{
    register uint16_t sbr;
    uint8_t temp;
    uint32_t sysclk;     //时钟

    if(uratn == UART0)
    {
      if(UART0_RX == PTA1) Port_alt(UART0_RX,ALT2);
      else if((UART0_RX == PTA15)||(UART0_RX == PTB16)||(UART0_RX == PTD6)) Port_alt(UART0_RX,ALT3);
      else if(UART0_RX == PTE21) Port_alt(UART0_RX,ALT4);
      
      if(UART0_TX == PTA2) Port_alt(UART0_TX,ALT2);
      else if((UART0_TX == PTA14)||(UART0_TX == PTB17)||(UART0_TX == PTD7)) Port_alt(UART0_TX,ALT3);
      else if(UART0_TX == PTE20) Port_alt(UART0_TX,ALT4);

      SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UARTn 时钟
      
            //设置的时候，应该禁止发送接受
      UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

      //配置成8位无校验模式
      //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
      UART_C1_REG(UART0_BASE_PTR) = 0;	// 全部直接使用默认设置就行，所以直接清0

      //计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟
      SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);      //UART0选择MCGFLLCLK_khz=48000Khz时钟源
      SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
      sysclk = (core_clk_khz/2) * 1000;            //内核时钟

      //设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器
      sbr = (uint16_t)(sysclk / (baud << 4));

      /* Save off the current value of the UARTx_BDH except for the SBR field */
      temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));

      UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
      UART_BDL_REG(UART0_BASE_PTR) = (uint8_t)(sbr & UART_BDL_SBR_MASK);

      //初始化控制寄存器、清标志位
      UART0_C4 = 0x0F;
      UART0_C1 = 0x00;
      UART0_C3 = 0x00;
      UART0_MA1 = 0x00;
      UART0_MA2 = 0x00;
      UART0_S1 |= 0x1F;
      UART0_S2 |= 0xC0;

      /* 启动发送和接收 */
      UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK);
    }
    else
    {
      switch(uratn)
      {
        case UART1:
              Port_alt(UART1_RX,ALT3);
              Port_alt(UART1_TX,ALT3);
              SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
              break;

        case UART2:
              if((UART2_RX == PTD2) || (UART2_RX == PTD4)) Port_alt(UART2_RX,ALT3);
              else if(UART2_RX == PTE23) Port_alt(UART2_RX,ALT4);

              if((UART2_TX == PTD3) || (UART2_TX == PTD5)) Port_alt(UART2_TX,ALT3);
              else if(UART2_TX == PTE22) Port_alt(UART2_TX,ALT4);
                       
              SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
              break;
        default:
            break;
      }

      //设置的时候，应该禁止发送接受
      UART_C2_REG(UARTx[uratn - 1]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

      //配置成8位无校验模式
      //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
      UART_C1_REG(UARTx[uratn - 1]) = 0;	// 全部直接使用默认设置就行，所以直接清0

      //计算波特率，串口0、1使用内核时钟，其它串口使用外设时钟
      sysclk = bus_clk_khz * 1000;  //外设时钟

      //设置 UART 数据通讯波特率。通过设置 UART 模块的波特率寄存器
      sbr = (uint16_t)(sysclk / (baud << 4));

      /* Save off the current value of the UARTx_BDH except for the SBR field */
      temp = UART_BDH_REG(UARTx[uratn - 1]) & ~(UART_BDH_SBR(0x1F));

      UART_BDH_REG(UARTx[uratn - 1]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
      UART_BDL_REG(UARTx[uratn - 1]) = (uint8_t)(sbr & UART_BDL_SBR_MASK);
      UART_C1_REG(UARTx[uratn - 1]) = 0x00;
      UART_C3_REG(UARTx[uratn - 1]) = 0x00;
      UART_S1_REG(UARTx[uratn - 1]) = 0x1F;
      UART_S2_REG(UARTx[uratn - 1]) = 0x00;
      /* 启动发送和接收 */
      UART_C2_REG(UARTx[uratn - 1]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK);
    }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_getchar
*  功能说明：无限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
char uart_getchar (UARTn uratn)
{
  if(uratn == UART0)
  {
    /* Wait until character has been received */
    while (!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK));
    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UART0_BASE_PTR);
  }
  else
  {
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn - 1]) & UART_S1_RDRF_MASK));
    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn - 1]);
  }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_pendchar
*  功能说明：有限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
char uart_rechar (UARTn uratn, char *ch)
{
    uint32_t i = 0;
  if(uratn == UART0)
  {
    while(++i < 0xffffff)                                         //时间限制
    {
        if(UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK)         //查询是否接受到数据
        {
            *ch  =   UART_D_REG(UART0_BASE_PTR);                    //接受到8位的数据
            return  1;                                            //返回 1 表示接收成功
        }
    }

    *ch = 0;                                                     //接收不到，应该清空了接收区
    return 0;                                                    //返回0表示接收失败
  }
  else
  {
    while(++i < 0xffffff)                                         //时间限制
    {
        if(UART_S1_REG(UARTx[uratn - 1]) & UART_S1_RDRF_MASK)         //查询是否接受到数据
        {
            *ch  =   UART_D_REG(UARTx[uratn - 1]);                    //接受到8位的数据
            return  1;                                            //返回 1 表示接收成功
        }
    }

    *ch = 0;                                                     //接收不到，应该清空了接收区
    return 0;                                                    //返回0表示接收失败
  }    
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_putchar
*  功能说明：串口发送一个字节
*  参数说明：UARTn       模块号（UART0~UART2）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //等待发送缓冲区空
  if(uratn == UART0)
  {
    while(!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_TDRE_MASK));
    //发送数据
    UART_D_REG(UART0_BASE_PTR) = (uint8_t)ch;
  }
  else
  {
    while(!(UART_S1_REG(UARTx[uratn - 1]) & UART_S1_TDRE_MASK));
    //发送数据
    UART_D_REG(UARTx[uratn - 1]) = (uint8_t)ch;
  }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_query
*  功能说明：查询是否接受到一个字节
*  参数说明：UARTn       模块号（UART0~UART2）
*  函数返回：1           接收到一个字节了
*            0           没有接收到
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
int uart_query (UARTn uratn)
{
  if(uratn == UART0)
    return (UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK);
  else
    return (UART_S1_REG(UARTx[uratn - 1]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendN
*  功能说明：串行 发送指定len个字节长度字符串 （包括 NULL 也会发送）
*  参数说明：UARTn       模块号（UART0~UART2）
*            buff        发送缓冲区
*            len         发送长度
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_sendN (UARTn uratn, uint8_t *buff, uint16_t len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        uart_putchar(uratn, buff[i]);
    }
}
/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendN
*  功能说明：串行 字符串 （包括 NULL 也会发送）
*  参数说明：UARTn       模块号（UART0~UART5）
*            str        发送字符串     
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void uart_sendStr(UARTn uratn, const uint8_t *str)
{
  while(*str)
  {
    uart_putchar(uratn, *str++);
  }
}
/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendN
*  功能说明：开串口接收中断
*  参数说明：UARTn       模块号（UART0~UART2）
*            
*            
*  函数返回：无
*  修改时间：2015-7-24
*  备    注：
*************************************************************************/
void UART_IRQ_EN(UARTn uratn)
{
  if(uratn == UART0) UART_C2_REG(UART0_BASE_PTR)|= UART_C2_RIE_MASK; 
  else UART_C2_REG(UARTx[uratn - 1])|= UART_C2_RIE_MASK;
  enable_irq(uratn + UART0SE_irq_no);
  
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：uart_sendN
*  功能说明：关串口接收中断
*  参数说明：UARTn       模块号（UART0~UART2）
*            
*            
*  函数返回：无
*  修改时间：2015-7-24
*  备    注：
*************************************************************************/
void UART_IRQ_DIS(UARTn uratn)
{
  if(uratn == UART0) UART_C2_REG(UART0_BASE_PTR) &=~UART_C2_RIE_MASK; 
  else UART_C2_REG(UARTx[uratn - 1]) &=~ UART_C2_RIE_MASK;
  enable_irq(uratn + UART0SE_irq_no);
}
