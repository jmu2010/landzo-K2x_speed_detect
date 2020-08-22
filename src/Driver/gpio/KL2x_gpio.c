/***********************************************************
 * 文件名       ：KL25_gpio.c
 * 说明         ：IO端口应用函数
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/
/*
 * 包含头文件
 */
#include "common.h"
#include "KL2x_gpio.h"



//各端口基地址放入常数数据组PORTX[0]~PORTX[4]中
volatile struct PORT_MemMap *PORTX[5]={PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};
//GPIO口基地址放入常数数据组GPIOx[0]~GPIOx[4]中
volatile struct GPIO_MemMap *GPIOX[5]={PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR};
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_enable_port
*  功能说明：加载所有GPIO端口的时钟。 供高层驱动调用。
*  参数说明：
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：苏州大学的例程
*************************************************************************/
void gpio_enable_port (void)
{
 	  SIM_SCGC5 |=   SIM_SCGC5_PORTA_MASK \
	               | SIM_SCGC5_PORTB_MASK \
	               | SIM_SCGC5_PORTC_MASK \
	               | SIM_SCGC5_PORTD_MASK \
	               | SIM_SCGC5_PORTE_MASK;
}
/*************************************************************************
*                             蓝宙电子科技有限公司
*
*  函数名称：gpio_Interrupt_init
*  功能说明：初始化GPIO中断源
*  参数说明：PTxn        端口号
*            GPIO_CFG    端口状态
*            mode        中断状态 
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/

void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, uint32 mode)
{
  if((cfg == GPI)||(cfg == GPI_DOWN)||(cfg == GPI_UP))
  {
    Port_init(ptxn,(cfg | ALT1 | mode));
    GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //设置端口方向为输入 
#if defined(MKL25Z4)
    if(PTX(ptxn)==0) enable_irq(PortA_irq_no);
    else if(PTX(ptxn)==3) enable_irq(PortD_irq_no);
#elif defined(MKL26Z4)
    if(PTX(ptxn)==0) enable_irq(PortA_irq_no);
    else if((PTX(ptxn)==2)||(PTX(ptxn)==3)) enable_irq(PortC_PortD_irq_no);
#endif
  }
}

/*************************************************************************
*                             野火嵌入式开发工作室/蓝宙电子科技有限公司
*
*  函数名称：gpio_init
*  功能说明：初始化gpio
*  参数说明：PTxn        端口号
*            GPIO_CFG    端口状态
*            uint8       引脚输出状态 1 ，0
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_init (PTxn ptxn, GPIO_CFG cfg, uint8 sata)
{ 
//端口方向控制输入还是输出
    if( (cfg  == GPI) || (cfg == GPI_UP) || (cfg == GPI_DOWN) )
    {
       Port_init(ptxn,cfg|ALT1);
       GPIO_PDDR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));  //设置端口方向为输入 
    }
    else
    {  
        Port_init(ptxn,ALT1) ;  
      //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));        // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
        //端口输出数据/        
        if(sata == 0){     
            GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
         }else{
            GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
        }
    }
}



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_set
*  功能说明：设置引脚状态
*  参数说明：PTxn        端口号
*            uint8       引脚输出状态 1 ，0
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_set (PTxn ptxn,uint8 sata){
  if(sata) GPIO_PDOR_REG(GPIOX[ptxn>> 5])  |= (1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
   else    GPIO_PDOR_REG(GPIOX[ptxn>> 5]) &= ~(1 << (ptxn&0x1f));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平 
}
/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_turn
*  功能说明：引脚反转
*  参数说明：PTxn        端口号
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void gpio_turn(PTxn ptxn){
  GPIO_PTOR_REG(GPIOX[ptxn>> 5]) |= (1 << (ptxn&0x1f));   // GPIO PTOR 管脚号  
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_get
*  功能说明：引脚反转
*  参数说明：PTxn        端口号
*  函数返回：无
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
uint8 gpio_get (PTxn ptxn){
  return ((GPIO_PDIR_REG(GPIOX[ptxn>> 5])>>(ptxn&0x1f)) &0x01);   // GPIO PDIR 管脚号  
}
               
