/***********************************************************
 * 文件名       ：isr.c
 * 说明         ：中断功能模块
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

//#include "isr.h"
#include  "include.h"

uint8 TIME0flag_5ms   ;
uint8 TIME0flag_10ms  ;
uint8 TIME0flag_20ms  ;
uint8 TIME0flag_50ms  ;
uint8 TIME0flag_80ms  ;
uint8 TIME0flag_100ms ;
uint8 TIME0flag_1s;
uint8 TIME1flag_20ms  ;
uint8  TimeCount = 0 ;

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：setIRQadrr
*  功能说明：设置中断地址函数
*  参数说明：IRQInterruptIndex 中断向量表，在芯片的.H文件，比如K60 在"MK60DZ10.h"
*            void IRQAdrr(void)  中断函数
*  函数返回：无
*  修改时间：2015-5-15    已测试
*  备    注：
*************************************************************************/

__RAMFUN void setIRQadrr(IRQInterruptIndex IRQnum,void IRQAdrr(void) ){
  extern uint32 __VECTOR_TABLE[];
  extern uint32 __VECTOR_RAM[];
  vuint32  *Vector_TABl ,Vector_TAB;
  vuint16 temp0 ,temp1;
  vuint32 buf[128];
  Vector_TAB = SCB_VTOR ;
  Vector_TABl = (vuint32  *)Vector_TAB ;
  /**************
  中断向量表在ram区域，可以进行直接擦除
  **************/
  if(Vector_TAB == (uint32)&__VECTOR_RAM){   // 0xE000E000u
    Vector_TABl[IRQnum] = (uint32)(IRQAdrr) ;
   }
  else{
    /*****************************
    向量表在flash中
    *****************************/
    for(temp0 = 0 ; temp0 < 128 ; temp0++){
      buf[temp0] = Vector_TABl[temp0] ;
    }
    buf[IRQnum]  = (uint32)(IRQAdrr) ;  
    hw_flash_init() ;
    flash_erase_sector(0) ;
    flash_write(0,0,1024,(uint8 *)buf) ;
  }
 }



/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：USART0_IRQHandler
*  功能说明：串口0 中断 接收 服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2014-8-28    已测试
*  备    注：
*************************************************************************/


void UART0_IRQHandler(void)
{
  uint8 ch;
  DisableInterrupts;		    //关总中断
  //接收一个字节数据并回发
  ch = uart_getchar (UART0);      //接收到一个数据
//  uart_sendStr  (UART0, "\n你发送的数据为：");
  uart_putchar (UART0, ch);       //就发送出去
  
  EnableInterrupts;		    //开总中断
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：USART1_IRQHandler
*  功能说明：串口1 中断 接收 服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*  备    注：
*************************************************************************/


void USART1_IRQHandler(void)
{
/*  uint8 ch;
  
  DisableInterrupts;		    //关总中断
  
  //接收一个字节数据并回发
  ch = uart_getchar (UART1);      //接收到一个数据
  uart_sendStr  (UART1, "\n你发送的数据为：");
  uart_putchar (UART1, ch);       //就发送出去
  
  EnableInterrupts;		    //开总中断 */
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PIT0_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：
*************************************************************************/



void PIT_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);       //清中断标志位
  
  TimeCount ++ ;
  
  if(TimeCount%1 == 0 )
  {
    TIME0flag_5ms = 1;
  } 
  if(TimeCount%2 == 0 )
  {
    TIME0flag_10ms = 1;    
  }   
  if(TimeCount%4 == 0 )
  {
    TIME0flag_20ms = 1;
  }
  if(TimeCount%10 == 0 )
  {
    TIME0flag_50ms = 1;
  }
  if(TimeCount%16 == 0 )
  {
    TIME0flag_80ms = 1;
  }  
  if(TimeCount%20 == 0 )
  {
    TIME0flag_100ms = 1;
  }  
  if(TimeCount%200 == 0 )
  {
    TIME0flag_1s = 1;
    TimeCount = 0 ;
  }
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：HardFault_Handler
*  功能说明：硬件上访中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-4    已测试
*  备    注：可以用LED闪烁来指示发生了硬件上访
*************************************************************************/
void HardFault_Handler(void)
{
  while (1)
  {
    //printf("\n****硬件上访错误!!!*****\r\n\n");
  }
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTD_IRQHandler
*  功能说明：PORTD端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要自己初始化来清除
*************************************************************************/


void PORTD_IRQHandler(void)
{
  uint8_t  n = 0;    //引脚号
//  n = 5;
  if(PORTD_ISFR & (1 << n))         //PTB0触发中断
  {
    PORTD_ISFR  |= (1 << n);        //写1清中断标志位
    
  } 
 
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTA_IRQHandler
*  功能说明：PORTA端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
void PORTA_IRQHandler(void)
{
  uint8_t  n = 0;    //引脚号
  n=5;
  if(PORTA_ISFR & (1 << n))         //PTA5触发中断
  {
    PORTA_ISFR  |= (1 << n);        //写1清中断标志位
    PTB0_OUT = ~PTB0_OUT;
  }
  
  
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：FTM0_IRQHandler
*  功能说明：FTM0输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/
void FTM0_IRQHandler()
{
  
  
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：FTM1_IRQHandler
*  功能说明：FTM1输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/
//static int32 CNT = 0; // (base)->STATUS
void FTM1_IRQHandler()
{
  uint8_t s = TPM_STATUS_REG(TPMx[TPM1]);             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
  uint8_t n;
  TPM_STATUS_REG(TPMx[TPM1]) = 0x00;               //清中断标志位
 
  n = 1;
  if( s & (1 << n))
  {
   // TPM_CnSC_REG(TPM1_BASE_PTR, n) &= ~TPM_CnSC_CHIE_MASK; // 禁止输入捕捉中断
    //     用户任务       //    
    //不建议在这里开启输入捕捉中断
   // TPM_CnSC_REG(TPM1_BASE_PTR,n) |= TPM_CnSC_CHIE_MASK;   // 开启输入捕捉中断
  }
}


/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：是采集摄像头数据，本数据位摄像头AD数据，可以采集到300个点。
设置标志位能够及时搬移。
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
void DMA_CH4_Handler(void)
{
/*  DMA_IRQ_CLEAN(DMA_CH4) ;
  DMA_IRQ_DIS(DMA_CH4);
  DMA_DIS(DMA_CH4); */
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：是采集摄像头数据，本数据位摄像头AD数据，可以采集到300个点。
设置标志位能够及时搬移。
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
void DMA_CH0_Handler(void)
{
//  printf("DMA_CH0_Handler\n");
/*  DMA_IRQ_CLEAN(DMA_CH0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
  DMA_EN(DMA_CH0);  */                                  //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
}



volatile uint8_t LPT_INT_count = 0;
void  LPT_Handler(void)
{
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   //清除LPTMR比较标志
  LPT_INT_count++;                    //中断溢出加1
}