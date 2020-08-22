
/***********************************************************
 * �ļ���       ��KL2x_uart.h
 * ˵��         �����ڳ�ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

#ifndef __KL2X_UART_H__
#define __KL2X_UART_H__

#define USART_REC_LEN  200 

typedef enum  UARTn
{
    //��ʼ��Ĭ������       --TXD--      --RXD--     ���Ը�������ͨ��������<<pin_cfg.h>>�����޸� uart_init
    UART0,    //           PTD7         PTD6
    UART1,    //           PTC4         PTC3
    UART2,    //           PTD3         PTD2
    
} UARTn;

extern volatile struct UART_MemMap *UARTx[2];
/********************************************************************/



void uart_init (UARTn, uint32_t baud);                 //��ʼ��uartxģ��
char uart_getchar   (UARTn);                          //���޵ȴ�����1���ֽ�
void uart_putchar   (UARTn, char ch);                 //����1���ֽ�
void uart_sendN     (UARTn , uint8_t *buff, uint16_t len); //����n���ֽ�
void uart_sendStr   (UARTn , const uint8_t *str);          //�����ַ���
void uart_irq_EN    (UARTn);                          //�����ڽ����ж�
void uart_irq_DIS   (UARTn);                          //�ش��ڽ����ж�
void uart_DMA_init (UARTn uratn, uint32_t baud);      //��ʼ��DMAUartxģ��
void uart_sendStr(UARTn uratn, const uint8_t *str);
int uart_query (UARTn uratn);
void UART_IRQ_EN(UARTn);  // �����ڽ����ж�
void UART_IRQ_DIS(UARTn); // �ؽ������ŵ�IRQ�ж�





/********************************************************************/

#endif /* __UART_H__ */
