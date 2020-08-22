/***********************************************************
 * 文件名       ：FTM.c
 * 说明         ：FTM定时器，目前有 PWM、输入捕捉功能
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V2.0
 *时间          ：15.5.26
************************************************************/

#include  "include.h"
#include  "KL2x_TPM.h"

volatile struct TPM_MemMap *TPMx[3] = {TPM0_BASE_PTR, TPM1_BASE_PTR, TPM2_BASE_PTR}; //定义三个指针数组保存 FTMn 的地址




/*************************************************************************
*                            野火嵌入式开发工作室
*
*  函数名称：TPM_PWM_init
*  功能说明：初始化TPM的PWM功能并设置频率、占空比。设置通道输出占空比。
*  参数说明：TPMn        模块号（TPM0、  TPM1、  TPM2）
*            CHn         通道号（CH0~CH7）
*            freq        频率（单位为Hz）
*            duty        占空比
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：同一个FTM，各通道的PWM频率是一样的，共3个FTM，即可以输出3个不同频率PWM
*************************************************************************/

void TPM_PWM_init(TPMn tpmn, CHn ch, uint32 freq, uint32 duty)
{
    uint16 mod;
    uint8 sc_ps;
    uint16 cv;   
    uint32 clk_hz = (bus_clk_khz * 1000) >> 1;       //bus频率/2 
    /*       计算分频因子       */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    if(freq < 1000)sc_ps++;    
    mod = (clk_hz >> sc_ps) / freq;                        //频率设置因子,clk_hz = 25000000
    /* 
    uart_putchar(UART0,mod>>24); 
    uart_putchar(UART0,mod>>16); 
    uart_putchar(UART0,mod>>8); 
    uart_putchar(UART0,mod>>0); */
    cv = (duty * (mod - 0 + 1)) / TPM_PRECISON;            //占空比设置因子
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;                           //使能FTM0时钟
        switch(ch)
        {
        case CH0:
            if((TPM0_CH0 == PTC1) || (TPM0_CH0 == PTD0))
            {
                Port_alt(TPM0_CH0,ALT4);
            }
            else if(TPM0_CH0 == PTE24)
            {
                Port_alt(TPM0_CH0,ALT3);
            }
            break;

        case CH1:
            if((TPM0_CH1 == PTC2) || (TPM0_CH0 == PTD1))
            {
                Port_alt(TPM0_CH1,ALT4);
            }
            else if((TPM0_CH1 == PTA4) || (TPM0_CH1 == PTE25))
            {
                Port_alt(TPM0_CH1,ALT3);
            }
            break;

        case CH2:
            if((TPM0_CH2 == PTC3) || (TPM0_CH2 == PTD2))
            {
                Port_alt(TPM0_CH2,ALT4);
            }
            else if((TPM0_CH2 == PTA5) || (TPM0_CH2 == PTE29))
            {
                Port_alt(TPM0_CH2,ALT3);
            }
            break;

        case CH3:
            if((TPM0_CH3 == PTC4) || (TPM0_CH3 == PTD3))
            {
                Port_alt(TPM0_CH3,ALT4);
            }
            else if(TPM0_CH3 == PTE30)
            {
                Port_alt(TPM0_CH3,ALT3);
            }
            break;

        case CH4:
            if(TPM0_CH4 == PTD4)
            {
                Port_alt(TPM0_CH4,ALT4);
            }
            else if((TPM0_CH4 == PTC8) || (TPM0_CH4 == PTE31))
            {
                Port_alt(TPM0_CH4,ALT3);
            }
            break;

        case CH5:
            if(TPM0_CH5 == PTD5)
            {
                Port_alt(TPM0_CH5,ALT4);
            }
            else if(TPM0_CH5 == PTC9)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            break;

        default:
            return;
        }
        break;

    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;                           //使能FTM1时钟
        switch(ch)
        {
        case CH0:
          Port_alt(TPM1_CH0,ALT3) ;
          break;
        case CH1:
          Port_alt(TPM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case CH0:
          Port_alt(TPM2_CH0,ALT3) ;
          break;

        case CH1:
          Port_alt(TPM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
    default:
        break;
    }
    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    TPM_CnSC_REG(TPMx[tpmn], ch) &= ~TPM_CnSC_ELSA_MASK;
    TPM_CnSC_REG(TPMx[tpmn], ch)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       边沿对齐PWM
    // ELSnB:ELSnA = 10     先高后低
    // ELSnB:ELSnA = 11     先低后高

    /******************** 配置时钟和分频 ********************/
    //SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        // 00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    TPM_SC_REG(TPMx[tpmn])    = ( 0
                                  //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | TPM_SC_PS(sc_ps)             //分频因子，分频系数 = 2^PS
                                  | TPM_SC_CMOD(1)
                                );
    TPM_MOD_REG(TPMx[tpmn])   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    TPM_CnV_REG(TPMx[tpmn], ch) = cv;
    TPM_CNT_REG(TPMx[tpmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}


/*************************************************************************
*                            野火嵌入式开发工作室
*
*  函数名称：TPM_PWM_Duty
*  功能说明：设置通道占空比
*  参数说明：TPMn        模块号（TPM0、  TPM1、  TPM2）
*            CHn         通道号
*            duty        占空比
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：
*************************************************************************/

void TPM_PWM_Duty(TPMn tpmn, CHn ch, uint32 duty)
{
    uint32 cv;
    uint32 mod = 0;
//  占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
    mod = TPM_MOD_REG(TPMx[tpmn]);        //读取 MOD 的值
    cv = (duty * (mod - 0 + 1)) / TPM_PRECISON;

    // 配置TPM通道值
    TPM_CnV_REG(TPMx[tpmn], ch) = cv;
}


/*************************************************************************
*                            野火嵌入式开发工作室
*
*  函数名称：TPM_PWM_freq
*  功能说明：设置FTM的频率
*  参数说明：TPMn        模块号（TPM0、  TPM1、  TPM2）
*            freq        频率（单位为Hz）
*  函数返回：无
*  修改时间：2012-2-14
*  备    注：同一个FTM，各通道的PWM频率是一样的，共3个FTM，即可以输出3个不同频率PWM
*************************************************************************/
void TPM_PWM_freq(TPMn tpmn, uint32 freq)             //设置TPM的频率
{
    uint32 clk_hz = (core_clk_khz * 1000) >> 1;        //bus频率/2
    uint32 mod;
    uint8 sc_ps;

    /*       计算频率设置        */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
    mod = (clk_hz >> sc_ps) / freq;


    /******************** 配置时钟和分频 ********************/
    TPM_SC_REG(TPMx[tpmn])    = TPM_SC_PS(sc_ps) | TPM_SC_CMOD(1);  //没有使能溢出中断
    TPM_MOD_REG(TPMx[tpmn])   = mod;                                                    //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    TPM_CNT_REG(TPMx[tpmn])   = 0;                                                      //计数器。只有低16位可用
    
}






/*************************************************************************
*                            野火嵌入式开发工作室
*
*  函数名称：TPM_Input_init
*  功能说明：输入捕捉初始化函数
*  参数说明：TPMn        模块号（TPM0、  TPM1、  TPM2）
*            CHn         通道号
*            Input_cfg   输入捕捉配置（Rising、Falling、Rising_or_Falling）上升沿捕捉、下降沿捕捉、跳变沿捕捉
*  函数返回：无
*  修改时间：2012-1-26
*  备    注：CH0~CH3可以使用过滤器，未添加这功能
*************************************************************************/
void TPM_Input_init(TPMn tpmn, CHn ch, Input_cfg cfg)
{
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;                           //使能FTM0时钟
        switch(ch)
        {
        case CH0:
            if((TPM0_CH0 == PTC1) || (TPM0_CH0 == PTD0))
            {
                Port_alt(TPM0_CH0,ALT4) ;
            }
            else if(TPM0_CH0 == PTE24)
            {
                Port_alt(TPM0_CH0,ALT3) ;
            }
            break;

        case CH1:
            if((TPM0_CH1 == PTC2) || (TPM0_CH0 == PTD1))
            {
                Port_alt(TPM0_CH1,ALT4) ;
            }
            else if((TPM0_CH1 == PTA4) || (TPM0_CH1 == PTE25))
            {
                Port_alt(TPM0_CH1,ALT3) ;
            }
            break;

        case CH2:
            if((TPM0_CH2 == PTC3) || (TPM0_CH2 == PTD2))
            {
                Port_alt(TPM0_CH2,ALT4) ;
            }
            else if((TPM0_CH2 == PTA5) || (TPM0_CH2 == PTE29))
            {
                Port_alt(TPM0_CH2,ALT3) ;
            }
            break;

        case CH3:
            if((TPM0_CH3 == PTC4) || (TPM0_CH3 == PTD3))
            {
                Port_alt(TPM0_CH3,ALT4) ;
            }
            else if(TPM0_CH3 == PTE30)
            {
                Port_alt(TPM0_CH3,ALT3) ;
            }
            break;

        case CH4:
            if(TPM0_CH4 == PTD4)
            {
                Port_alt(TPM0_CH4,ALT4) ;
            }
            else if((TPM0_CH4 == PTC8) || (TPM0_CH4 == PTE31))
            {
                Port_alt(TPM0_CH4,ALT3) ;
            }
            break;

        case CH5:
            if(TPM0_CH5 == PTD5)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            else if(TPM0_CH5 == PTC9)
            {
                Port_alt(TPM0_CH5,ALT4) ;
            }
            break;

        default:
            return;
        }
        break;

    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;                           //使能FTM1时钟
        switch(ch)
        {
        case CH0:
          Port_alt(TPM1_CH0,ALT3) ;
          break;
        case CH1:
          Port_alt(TPM1_CH1,ALT3) ;           
          break;

        default:
            return;
        }
        break;
    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case CH0:
          Port_alt(TPM2_CH0,ALT3) ;
          break;

        case CH1:
          Port_alt(TPM2_CH1,ALT3) ;
          break;

        default:
            return;
        }
        break;
    default:
        break;
    }

    switch(cfg)
    {
    case Rising:    //上升沿触发
        TPM_CnSC_REG(TPMx[tpmn], ch) |=  ( TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK );                   //置1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_ELSB_MASK  | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //清0
        break;

    case Falling:   //下降沿触发
        TPM_CnSC_REG(TPMx[tpmn], ch) |= (TPM_CnSC_ELSB_MASK  | TPM_CnSC_CHIE_MASK );                    //置1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //清0
        break;

    case Rising_or_Falling: //上升沿、下降沿都触发
        TPM_CnSC_REG(TPMx[tpmn], ch) |=  ( TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK ); //置1
        TPM_CnSC_REG(TPMx[tpmn], ch) &= ~( TPM_CnSC_MSB_MASK  | TPM_CnSC_MSA_MASK); //清0
        break;
    }

    TPM_SC_REG(TPMx[tpmn]) = TPM_SC_CMOD(0x1);       //System clock
    TPM_MOD_REG(TPMx[tpmn])   = 0xFFFF;
    TPM_CNT_REG(TPMx[tpmn])   = 0;                   //计数器。只有低16位可用（写任何值到此寄存器，都会清0）

    TPM_STATUS_REG(TPMx[tpmn]) = 0x00;               //清中断标志位

    //开启输入捕捉中断
    enable_irq(TPM0_irq_no + tpmn);
}



