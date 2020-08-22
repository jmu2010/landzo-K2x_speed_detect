/***********************************************************
 * 文件名       ：lptmr.c
 * 说明         ：低功耗定时模式
 * 作者         ：野火嵌入式开发工作室/landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#include "common.h"
#include "KL2x_lptmr.h"

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：lptmr_counter_init
*  功能说明：获得脉冲计数
*  参数说明：LPT0_ALTn  PIN脚选择
*            count      脉冲个数
*            LPT_CFG    触发方式
*  函数返回：脉冲数
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/
void lptmr_counter_init(LPT0_ALTn altn, uint16 count, uint8 PrescaleValue, LPT_CFG cfg)
{

    if(PrescaleValue > 0x0f) PrescaleValue = 0x0f;

    //设置输入管脚
    if(altn == LPT0_ALT1)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //打开 PORTA 时钟
        PORTA_PCR19 = PORT_PCR_MUX(0x5);    //在PTA19上使用 ALT5
    }
    else if(altn == LPT0_ALT2)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //使能 PORTC 时钟
        PORTC_PCR5 = PORT_PCR_MUX(0x3);     //在PTC5上使用 ALT3
    }
    // 开启模块时钟
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    // 清状态寄存器
    LPTMR0_CSR = 0x00;                      //先关了LPT，这样才能设置时钟分频等

    //设置累加计数值
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =  LPTMR_CMR_COMPARE(count);                       //设置比较值
    // 时钟选择 
    LPTMR_PSR_REG(LPTMR0_BASE_PTR)  = ( 0 
                                        | LPTMR_PSR_PCS(0x0) 
                                        | LPTMR_PSR_PBYP_MASK  
                                        | LPTMR_PSR_PRESCALE(PrescaleValue)        //使用 LPO clock 且 bypass glitch filter
                                      );                                              // 开启和配置脉冲滤波器：2^n个时钟上升沿才识别
                                       

    // 管脚设置、使能中断
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  = (0  
                                       | LPTMR_CSR_TPS(altn) 
                                       | LPTMR_CSR_TMS_MASK  
                                       | ( cfg == LPT_Falling ? LPTMR_CSR_TPP_MASK : 0)  
                                     //  | LPTMR_CSR_TEN_MASK  
                                       | LPTMR_CSR_TIE_MASK  
                                      );
    // 选择输入管脚  选择脉冲计数 下降沿 上升沿 使能LPT
    // TFC = 0，即计数值等于比较值时，计数值复位
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ;       //使能LPT
    enable_irq(LPTMR_irq_no);	 // 开引脚的IRQ中断
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：time_delay_ms
*  功能说明：延时函数，使用定功耗定时器延时，准确
*  参数说明：ms   延时时间，单位为ms
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程自带
*************************************************************************/
void time_delay_ms(uint32 ms)
{
    /* Make sure the clock to the LPTMR is enabled */
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

//    /* Set the compare value to the number of ms to delay */
//    LPTMR0_CMR = ms;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0_PSR = LPTMR_PSR_PCS(0x1) |LPTMR_PSR_PBYP_MASK; //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =  LPTMR_CMR_COMPARE(ms); // 设置比较值
    /* Start the timer */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); // 定时器比较标志；CNR = CMR跳出

    /* Clear Timer Compare Flag */
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; // 关闭定时器使能

    return;
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：lptmr_pulse_get
*  功能说明：获得脉冲计数
*  参数说明：
*  函数返回：脉冲数
*  修改时间：2015-5-15   已测试
*  备    注：
*************************************************************************/

uint16 lptmr_pulse_get(void)
{
    uint16  LPTCOUNT = 0  ;
    LPTMR0_CNR = 12 ; // On each read of the CNR,software must first write to the CNR with any value. 
    LPTCOUNT = LPTMR0_CNR ;
    lptmr_counter_clean();
    return (uint16)LPTCOUNT;
}



