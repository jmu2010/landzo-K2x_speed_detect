/***********************************************************
 * 文件名       ：KL2x_TPM.h
 * 说明         ：TPM定时器，目前有 PWM、输入捕捉功能
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/

#ifndef _KL2X_TPM_H_
#define _KL2X_TPM_H_

extern volatile struct TPM_MemMap *TPMx[3];

typedef enum TPMn
{
    TPM0,
    TPM1,
    TPM2
} TPMn;

typedef enum CHn
{
           //   --FTM0--  --FTM1--  --FTM2--
    CH0,   //     PTC1      PTA12      PTA1
    CH1,   //     PTC2      PTA13      PTA2
    CH2,   //     PTC3       ×         ×
    CH3,   //     PTC4       ×         ×
    CH4,   //     PTD4       ×         ×
    CH5,   //     PTD5       ×         ×
    // ×表示不存在
} CHn;



/*********************** PWM **************************/

#define TPM_PRECISON 100u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON

void  TPM_PWM_init(TPMn, CHn, uint32 freq, uint32 duty);  //初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个FTM，各通道的PWM频率是一样的，共3个FTM

void  TPM_PWM_Duty(TPMn, CHn, uint32 duty);    //设置通道占空比,占空比为 （duty * 精度） % ，如果 FTM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
void  TPM_PWM_freq(TPMn, uint32 freq);              //设置FTM的频率

void  TPM_QUAD_init(TPMn tpmn,CHn ch);
void  TPM_SpeedMeasure_init(void);


/*********************** 输入捕捉 **************************/

typedef enum Input_cfg
{
    Rising,               //上升沿捕捉
    Falling,              //下降沿捕捉
    Rising_or_Falling     //跳变沿捕捉
} Input_cfg;


void TPM_Input_init(TPMn, CHn, Input_cfg);

#define TPM_IRQ_EN(TPMn,CHn)        TPM_CnSC_REG(TPMx[TPMn],CHn) |= TPM_CnSC_CHIE_MASK       //开启 TPMn_CHn 中断
#define TPM_IRQ_DIS(TPMn,CHn)       TPM_CnSC_REG(TPMx[TPMn],CHn) &= ~TPM_CnSC_CHIE_MASK      //关闭 TPMn_CHn 中断








#endif  



