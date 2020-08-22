
/***********************************************************
 * 文件名       ：KL2x_uart.h
 * 说明         ：串口初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef __KL2X_UART_H__
#define __KL2X_UART_H__

#define USART_REC_LEN  200 

typedef enum  UARTn
{
    //初始化默认配置       --TXD--      --RXD--     可以复用其他通道，请在<<pin_cfg.h>>自行修改 uart_init
    UART0,    //           PTD7         PTD6
    UART1,    //           PTC4         PTC3
    UART2,    //           PTD3         PTD2
    
} UARTn;

extern volatile struct UART_MemMap *UARTx[2];
/********************************************************************/



void uart_init (UARTn, uint32_t baud);                 //初始化uartx模块
char uart_getchar   (UARTn);                          //无限等待接受1个字节
void uart_putchar   (UARTn, char ch);                 //发送1个字节
void uart_sendN     (UARTn , uint8_t *buff, uint16_t len); //发送n个字节
void uart_sendStr   (UARTn , const uint8_t *str);          //发送字符串
void uart_irq_EN    (UARTn);                          //开串口接收中断
void uart_irq_DIS   (UARTn);                          //关串口接收中断
void uart_DMA_init (UARTn uratn, uint32_t baud);      //初始化DMAUartx模块
void uart_sendStr(UARTn uratn, const uint8_t *str);
int uart_query (UARTn uratn);
void UART_IRQ_EN(UARTn);  // 开串口接收中断
void UART_IRQ_DIS(UARTn); // 关接收引脚的IRQ中断





/********************************************************************/

#endif /* __UART_H__ */
