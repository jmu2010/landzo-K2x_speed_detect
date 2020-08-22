/***********************************************************
 * �ļ���       ��K6x_uart.c
 * ˵��         �����ڳ�ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#include "common.h"
#include "KL2x_uart.h"


volatile struct UART_MemMap *UARTx[2] = {UART1_BASE_PTR, UART2_BASE_PTR}; //��������ָ�����鱣�� UARTx �ĵ�ַ



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_init
*  ����˵������ʼ�����ڣ����ò�����
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*            baud        �����ʣ���9600��19200��56000��115200��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ��������޸�
*************************************************************************/
void uart_init (UARTn uratn, uint32_t baud)
{
    register uint16_t sbr;
    uint8_t temp;
    uint32_t sysclk;     //ʱ��

    if(uratn == UART0)
    {
      if(UART0_RX == PTA1) Port_alt(UART0_RX,ALT2);
      else if((UART0_RX == PTA15)||(UART0_RX == PTB16)||(UART0_RX == PTD6)) Port_alt(UART0_RX,ALT3);
      else if(UART0_RX == PTE21) Port_alt(UART0_RX,ALT4);
      
      if(UART0_TX == PTA2) Port_alt(UART0_TX,ALT2);
      else if((UART0_TX == PTA14)||(UART0_TX == PTB17)||(UART0_TX == PTD7)) Port_alt(UART0_TX,ALT3);
      else if(UART0_TX == PTE20) Port_alt(UART0_TX,ALT4);

      SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UARTn ʱ��
      
            //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
      UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

      //���ó�8λ��У��ģʽ
      //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
      UART_C1_REG(UART0_BASE_PTR) = 0;	// ȫ��ֱ��ʹ��Ĭ�����þ��У�����ֱ����0

      //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ������ʱ��
      SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);      //UART0ѡ��MCGFLLCLK_khz=48000Khzʱ��Դ
      SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
      sysclk = (core_clk_khz/2) * 1000;            //�ں�ʱ��

      //���� UART ����ͨѶ�����ʡ�ͨ������ UART ģ��Ĳ����ʼĴ���
      sbr = (uint16_t)(sysclk / (baud << 4));

      /* Save off the current value of the UARTx_BDH except for the SBR field */
      temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));

      UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
      UART_BDL_REG(UART0_BASE_PTR) = (uint8_t)(sbr & UART_BDL_SBR_MASK);

      //��ʼ�����ƼĴ��������־λ
      UART0_C4 = 0x0F;
      UART0_C1 = 0x00;
      UART0_C3 = 0x00;
      UART0_MA1 = 0x00;
      UART0_MA2 = 0x00;
      UART0_S1 |= 0x1F;
      UART0_S2 |= 0xC0;

      /* �������ͺͽ��� */
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

      //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
      UART_C2_REG(UARTx[uratn - 1]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

      //���ó�8λ��У��ģʽ
      //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
      UART_C1_REG(UARTx[uratn - 1]) = 0;	// ȫ��ֱ��ʹ��Ĭ�����þ��У�����ֱ����0

      //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ������ʱ��
      sysclk = bus_clk_khz * 1000;  //����ʱ��

      //���� UART ����ͨѶ�����ʡ�ͨ������ UART ģ��Ĳ����ʼĴ���
      sbr = (uint16_t)(sysclk / (baud << 4));

      /* Save off the current value of the UARTx_BDH except for the SBR field */
      temp = UART_BDH_REG(UARTx[uratn - 1]) & ~(UART_BDH_SBR(0x1F));

      UART_BDH_REG(UARTx[uratn - 1]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
      UART_BDL_REG(UARTx[uratn - 1]) = (uint8_t)(sbr & UART_BDL_SBR_MASK);
      UART_C1_REG(UARTx[uratn - 1]) = 0x00;
      UART_C3_REG(UARTx[uratn - 1]) = 0x00;
      UART_S1_REG(UARTx[uratn - 1]) = 0x1F;
      UART_S2_REG(UARTx[uratn - 1]) = 0x00;
      /* �������ͺͽ��� */
      UART_C2_REG(UARTx[uratn - 1]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_getchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
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
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_rechar (UARTn uratn, char *ch)
{
    uint32_t i = 0;
  if(uratn == UART0)
  {
    while(++i < 0xffffff)                                         //ʱ������
    {
        if(UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK)         //��ѯ�Ƿ���ܵ�����
        {
            *ch  =   UART_D_REG(UART0_BASE_PTR);                    //���ܵ�8λ������
            return  1;                                            //���� 1 ��ʾ���ճɹ�
        }
    }

    *ch = 0;                                                     //���ղ�����Ӧ������˽�����
    return 0;                                                    //����0��ʾ����ʧ��
  }
  else
  {
    while(++i < 0xffffff)                                         //ʱ������
    {
        if(UART_S1_REG(UARTx[uratn - 1]) & UART_S1_RDRF_MASK)         //��ѯ�Ƿ���ܵ�����
        {
            *ch  =   UART_D_REG(UARTx[uratn - 1]);                    //���ܵ�8λ������
            return  1;                                            //���� 1 ��ʾ���ճɹ�
        }
    }

    *ch = 0;                                                     //���ղ�����Ӧ������˽�����
    return 0;                                                    //����0��ʾ����ʧ��
  }    
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_putchar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //�ȴ����ͻ�������
  if(uratn == UART0)
  {
    while(!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_TDRE_MASK));
    //��������
    UART_D_REG(UART0_BASE_PTR) = (uint8_t)ch;
  }
  else
  {
    while(!(UART_S1_REG(UARTx[uratn - 1]) & UART_S1_TDRE_MASK));
    //��������
    UART_D_REG(UARTx[uratn - 1]) = (uint8_t)ch;
  }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_query
*  ����˵������ѯ�Ƿ���ܵ�һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*  �������أ�1           ���յ�һ���ֽ���
*            0           û�н��յ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
int uart_query (UARTn uratn)
{
  if(uratn == UART0)
    return (UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK);
  else
    return (UART_S1_REG(UARTx[uratn - 1]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�������� ����ָ��len���ֽڳ����ַ��� ������ NULL Ҳ�ᷢ�ͣ�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*            buff        ���ͻ�����
*            len         ���ͳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
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
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�������� �ַ��� ������ NULL Ҳ�ᷢ�ͣ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            str        �����ַ���     
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendStr(UARTn uratn, const uint8_t *str)
{
  while(*str)
  {
    uart_putchar(uratn, *str++);
  }
}
/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵���������ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*            
*            
*  �������أ���
*  �޸�ʱ�䣺2015-7-24
*  ��    ע��
*************************************************************************/
void UART_IRQ_EN(UARTn uratn)
{
  if(uratn == UART0) UART_C2_REG(UART0_BASE_PTR)|= UART_C2_RIE_MASK; 
  else UART_C2_REG(UARTx[uratn - 1])|= UART_C2_RIE_MASK;
  enable_irq(uratn + UART0SE_irq_no);
  
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�����ش��ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*            
*            
*  �������أ���
*  �޸�ʱ�䣺2015-7-24
*  ��    ע��
*************************************************************************/
void UART_IRQ_DIS(UARTn uratn)
{
  if(uratn == UART0) UART_C2_REG(UART0_BASE_PTR) &=~UART_C2_RIE_MASK; 
  else UART_C2_REG(UARTx[uratn - 1]) &=~ UART_C2_RIE_MASK;
  enable_irq(uratn + UART0SE_irq_no);
}
