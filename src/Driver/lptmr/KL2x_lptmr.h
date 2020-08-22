/***********************************************************
 * 文件名       ：lptmr.h
 * 说明         ：低功耗定时模式
 * 作者         ：野火嵌入式开发工作室/landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef __KL2X_LPTMR_H__
#define __KL2X_LPTMR_H__

/********************************************************************/


typedef enum LPT0_ALTn
{
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;


#define  lptmr_counter_clean()     LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;\
                                    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK  ;          //计数器计数清0

/*          用于延时         */

void time_delay_ms(uint32 ms);      //延时

/*       用于脉冲计数        */

//void lptmr_counter_init(LPT0_ALTn, uint16_t count,  LPT_CFG);                           //计数器初始化设置
void lptmr_counter_init(LPT0_ALTn altn, uint16 count, uint8 PrescaleValue, LPT_CFG cfg);
uint16 lptmr_pulse_get(void) ;




/********************************************************************/

#endif /* __LPTMR_H__ */
