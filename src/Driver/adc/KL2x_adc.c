
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：adc.c
 * 描述         ：adc驱动函数
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/	

/***********************************************************
 * 文件名       ：adc.c
 * 说明         ：adc初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/
#include "common.h"
#include "KL2x_adc.h"
#include "gpio_cfg.h"
#include "port.h"


ADC_MemMapPtr ADCx[1] = {ADC0_BASE_PTR}; //定义两个指针数组保存 ADCx 的地址
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：adc_DMA_init
*  功能说明：AD初始化，使能时钟
*  参数说明：ADCn        模块号（ ADC0）
*  函数返回：无ao
*  备    注：参考苏州大学的例程
*************************************************************************/
void adc_DMA_init(ADC_Ch ch)
{
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_init(ch)  ;
    //初始化ADC默认配置
    ADC_CFG1_REG(ADCx[adcn])  = ADLPC_LOW
                                 | ADC_CFG1_ADIV(ADIV_4)
                                 | ADLSMP_SHORT
                                 | ADC_CFG1_MODE(0)                   //8位
                                 | ADC_CFG1_ADICLK(ADICLK_BUS);
    ADC_CFG2_REG(ADCx[adcn])  = (ch>>5) // MUXSEL_ADCB    //MUXSEL_ADCA
                                 | ADACKEN_ENABLED
                                 | ADHSC_HISPEED
                                 | ADC_CFG2_ADLSTS(ADLSTS_2) ;
    
    
   
    ADC_SC2_REG(ADCx[adcn]) = ADTRG_SW                       //硬件触发
                                | ACFE_DISABLED 
                                | ACFGT_GREATER 
                                | ACREN_DISABLED 
                                | DMAEN_DISABLED               //使能DMA DMAEN_ENABLED
                                | ADC_SC2_REFSEL(REFSEL_EXT);
    
    ADC_SC3_REG(ADCx[adcn])  = CAL_OFF 
                                 | ADCO_CONTINUOUS 
                                 | AVGE_DISABLED 
                                 | ADC_SC3_AVGS(AVGS_4);
  
    ADC_SC1_REG(ADCx[adcn],ch>>5) = AIEN_ON | DIFF_SINGLE |      //
                                 ADC_SC1_ADCH( ch );
 
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：adc_init
*  功能说明：初始化PIN为AD模式
*  参数说明：ADCn      AD模块（AD0)
*            ADC_Ch    AD通道（1 2 3 ......)
*  函数返回：无
*  修改时间：2012-5-15   已测试
*  备    注：
*************************************************************************/
void adc_init(ADC_Ch ch)
{

  SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
  SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
  SIM_SOPT7 = SIM_SOPT7_ADC0TRGSEL(0);

  switch(ch)
  {
    case ADC0_SE4a:  //ADC0_SE4a-- PTE21
      Port_alt(PTE21,ALT0);
      break;
    case ADC0_SE7a:  //ADC0_SE7a -- PTE23
      Port_alt(PTE23,ALT0);
      break;
    case ADC0_SE5b:  //ADC0_SE5b -- PTD1
      Port_alt(PTD1,ALT0);
      break;
    case ADC0_SE6b:   //ADC0_SE6b -- PTD5
      Port_alt(PTD5,ALT0);
      break;
    case ADC0_SE8:  //ADC0_SE8 -- PTB0
      Port_alt(PTB0,ALT0);
      break;
    case ADC0_SE9:  //ADC0_SE9 -- PTB1
      Port_alt(PTB1,ALT0);
      break;
    case ADC0_SE11:  //ADC0_SE11 -- PTC2
      Port_alt(PTC2,ALT0);
      break;
    case ADC0_SE12:  //ADC0_SE12 -- PTB2
      Port_alt(PTB2,ALT0);
      break;
    case ADC0_SE13:  //ADC0_SE13 -- PTB3
      Port_alt(PTB3,ALT0);
      break;
    case ADC0_SE14:  //ADC0_SE14 -- PTC0
      Port_alt(PTC0,ALT0);
      break;
    case ADC0_SE15:   //ADC0_SE15 -- PTC1
      Port_alt(PTC1,ALT0);
      break;
    case ADC0_SE23:   //ADC0_SE23 -- PTE30
      Port_alt(PTE30,ALT0);
      break;
    default:
          break;
  }
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：ad_once
*  功能说明：采集一次一路模拟量的AD值
*  参数说明：ADCn        模块号（ ADC0）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*  函数返回：无符号结果值
*  修改时间：2012-2-10
*  备    注：参考苏州大学的例程
*************************************************************************/
uint16_t ad_once(ADC_Ch ch, ADC_nbit bit) //采集某路模拟量的AD值
{
    uint16_t result = 0;
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_start(ch, bit);	  //启动ADC转换

    while (( ADC_SC1_REG(ADCx[adcn], 0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
    result = ADC_R_REG(ADCx[adcn], 0);
    ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：ad_ave
*  功能说明：多次采样，取平均值
*  参数说明：ADCx        模块号（ ADC0）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*            N           均值滤波次数(范围:0~255)
*  函数返回：16位无符号结果值
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
uint16_t ad_ave(ADC_Ch ch, ADC_nbit bit, uint8_t N) //均值滤波
{
    uint32_t tmp = 0;
    uint8_t  i;

    for(i = 0; i < N; i++)
        tmp += ad_once(ch, bit);
    tmp = tmp / N;
    return (uint16_t)tmp;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：ad_SUM
*  功能说明：多次采样，取平均值
*  参数说明：ADCx        模块号（ ADC0）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*            N           均值滤波次数(范围:0~255)
*  函数返回：16位无符号结果值
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
uint16_t ad_sum(ADC_Ch ch, ADC_nbit bit, uint8_t N) //采样集合
{
    uint32_t tmp = 0;
    uint8_t  i;
    ADCn adcn = (ADCn)(ch >> 5) ;
    adc_start(ch, bit);	  //启动ADC转换
    
    for(i = 0; i < N; i++)
    {
      while (( ADC_SC1_REG(ADCx[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
      tmp += ADC_R_REG(ADCx[adcn], 0);
      ADC_SC1_REG(ADCx[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    }
    return (uint16_t)tmp;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：adc_start
*  功能说明：启动adc软件采样，B通道不能用于软件触发！！！！
*  参数说明：ADCx        模块号（ ADC0）
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
*  函数返回：无
*  修改时间：2012-2-10
*  备    注：修改苏州大学的例程
*************************************************************************/
void adc_start(ADC_Ch ch, ADC_nbit bit)
{
  ADCn adcn = (ADCn)(ch >> 5) ;
  //初始化ADC默认配置
  ADC_CFG1_REG(ADCx[adcn])  = ADLPC_NORMAL
                               | ADC_CFG1_ADIV(ADIV_4)
                               | ADLSMP_LONG
                               | ADC_CFG1_MODE(bit)
                               | ADC_CFG1_ADICLK(ADICLK_BUS);
  ADC_CFG2_REG(ADCx[adcn])  =  ((ch>>5)  << ADC_CFG2_MUXSEL_SHIFT)   //MUXSEL_ADCB    //MUXSEL_ADCA
                               | ADACKEN_DISABLED
                               | ADHSC_HISPEED
                               | ADC_CFG2_ADLSTS(ADLSTS_20) ;
  ADC_SC1_REG(ADCx[adcn],0) = AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH( ch );
   
}





