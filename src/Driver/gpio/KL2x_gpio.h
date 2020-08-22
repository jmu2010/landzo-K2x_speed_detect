/***********************************************************
 * 文件名       ：K6x_gpio.h
 * 说明         ：IO端口应用
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/
#ifndef __K6x_GPIO_H__
#define __K6x_GPIO_H__

#include "gpio_cfg.h"
#include  "port.h"
/*
 * 定义管脚方向
 */
typedef enum GPIO_CFG
{
    //这里的值不能改！！！
    GPI         = 0,                                //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                                //定义管脚输出方向
    
    GPI_DOWN    = 0x02,                       //输入下拉              PORTx_PCRn需要PE=1，PS=0
    GPI_UP      = 0x03,                       //输入上拉              PORTx_PCRn需要PE=1，PS=1
    GPI_PF      = 0x10,                       //输入，带无源滤波器,滤波范围：10 MHz ~ 30 MHz 。不支持高速接口（>=2MHz）  0b10000           Passive Filter Enable
    GPI_DOWN_PF = GPI_DOWN | GPI_PF ,         //输入下拉，带无源滤波器
    GPI_UP_PF   = GPI_UP   | GPI_PF ,         //输入上拉，带无源滤波器

    GPO_HDS     = 0x41,                        //输出高驱动能力   0b100 0001    High drive strength
    GPO_SSR     = 0x05,                        //输出慢变化率          0b101    Slow slew rate
    GPO_HDS_SSR = GPO_HDS | GPO_SSR,           //输出高驱动能力、慢变化率   
       
} GPIO_CFG;

#define DISABLED      IRQ_DISABLED    //中断禁止
#define DMARING       DMA_RISING     //DMA上升沿触发
#define DMAFALLING    DMA_FALLING     //DMA下降沿触发
#define DMAEITHER     DMA_EITHER     //DMA跳变沿触发
#define ZERO          IRQ_ZERO        //低电平触发
#define RING          IRQ_RISING      //上升沿触发
#define FALLING       IRQ_FALLING    //下降沿触发
#define EITHER        IRQ_EITHER    //跳变沿触发
#define ONE           IRQ_ONE         //高电平触发

void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 data) ;
void gpio_set (PTxn ptxn,uint8 sata) ;
void gpio_turn(PTxn ptxn) ;
uint8 gpio_get (PTxn ptxn) ;
void gpio_enable_port (void) ;
void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode)  ;

#endif      
