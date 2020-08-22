/***********************************************************
 * 文件名       ：adc.h
 * 说明         ：ADC通过初始化
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V2.0
 *时间          ：15.5.26
************************************************************/


#ifndef __KL2x_ADC_H__
#define __KL2x_ADC_H__ 

typedef enum ADCn  //ADC端口
{
    ADC0,
} ADCn;

//ADC通道
typedef enum ADC_Ch
{
    ADC0_DP0 = 0,   // PTE20
    ADC0_SE0 = 0,

    ADC0_DP1 = 1,   // PTE16
    ADC0_SE1 = 1,

    ADC0_DP2 = 2,   // PTE18
    ADC0_SE2 = 2,

    ADC0_DP3 = 3,   // PTE22
    ADC0_SE3 = 3,

    ADC0_DM0 = 4,   // PTE21
    ADC0_SE4a = 4,

    ADC0_DM1 = 5,   // PTE17
    ADC0_SE5a = 5,

    ADC0_DM2 = 6,   // PTE19
    ADC0_SE6a = 6,

    ADC0_DM3 = 7,   // PTE23
    ADC0_SE7a = 7,
    
    ADC0_SE4b = 4,   // PTE29     不支持软件ADC，传递进软件触发ADC，会当作 a通道处理
    ADC0_SE5b = 5,   // PTD1      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理
    ADC0_SE6b = 6,   // PTD5      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理
    ADC0_SE7b = 7,   // PTD6      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理
    ADC0_SE8,       // PTB0
    ADC0_SE9,       // PTB1
    ADC0_RES0,      // 保留
    ADC0_SE11,      // PTC2
    ADC0_SE12,      // PTB2
    ADC0_SE13,      // PTB3
    ADC0_SE14,      // PTC0
    ADC0_SE15,      // PTC1
    ADC0_RES1,      // 保留
    ADC0_RES2,      // 保留
    ADC0_RES3,      // 保留
    ADC0_RES4,      // 保留
    ADC0_RES5,      // 保留
    ADC0_RES6,      // 保留
    ADC0_RES7,      // 保留
    ADC0_SE23,      // PTE30
    DAC0_OUT = ADC0_SE23, // PTE30  DAC0输出 ,传入 ADC函数会当作 ADC0_SE23  处理
    ADC0_RES8,      // 保留
    ADC0_RES9,      // 保留

    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    ADC0_RES10,     // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC
} ADC_Ch;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


#define Module_disabled    AD31         //   禁用模块
// Conversion Complete (COCO) mask
#define COCO_COMPLETE     ADC_SC1_COCO_MASK
#define COCO_NOT          0x00

// ADC interrupts: enabled, or disabled.
// ADC 中断: 使能或者禁止
#define AIEN_ON           ADC_SC1_AIEN_MASK
#define AIEN_OFF          0x00

// Differential or Single ended ADC input
//查分或者单端ADC输入
#define DIFF_SINGLE       0x00
#define DIFF_DIFFERENTIAL ADC_SC1_DIFF_MASK

// Power setting of ADC
//ADC电源设置
#define ADLPC_LOW         ADC_CFG1_ADLPC_MASK
#define ADLPC_NORMAL      0x00

// Clock divisor
//时钟分频
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

// Long samle time, or Short sample time
// 长采样时间或者短采样时间
#define ADLSMP_LONG       ADC_CFG1_ADLSMP_MASK
#define ADLSMP_SHORT      0x00


// ADC Input Clock Source choice? Bus clock, Bus clock/2, "altclk", or the
//                                ADC's own asynchronous clock for less noise
//ADC输入时钟源选择 总线，总线/2，”altclk“或者ADC自身异步时钟以减少噪声
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03

//// ADCCFG2

// 选择通道A，通道B
#define MUXSEL_ADCB       ADC_CFG2_MUXSEL_MASK
#define MUXSEL_ADCA       0x00

// Ansync clock output enable: enable, or disable the output of it
// 异步时钟输出使能：使能，或者禁止输出
#define ADACKEN_ENABLED   ADC_CFG2_ADACKEN_MASK
#define ADACKEN_DISABLED  0x00

// High speed or low speed conversion mode
// 高速低速转换时间
#define ADHSC_HISPEED     ADC_CFG2_ADHSC_MASK
#define ADHSC_NORMAL      0x00

// Long Sample Time selector: 20, 12, 6, or 2 extra clocks for a longer sample time
// 长采样时间选择：20,12,6或者2个额外的时钟对于长采样时间
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03


// Read-only status bit indicating conversion status
// 只读状态位只是转换状态
#define ADACT_ACTIVE       ADC_SC2_ADACT_MASK
#define ADACT_INACTIVE     0x00

// Trigger for starting conversion: Hardware trigger, or software trigger.
// For using PDB, the Hardware trigger option is selected.
// 触发开始转换:硬件触发，软件触发
#define ADTRG_HW           ADC_SC2_ADTRG_MASK
#define ADTRG_SW           0x00

// ADC Compare Function Enable: Disabled, or Enabled.
//ADC比较功能使能：禁止或者使能
#define ACFE_DISABLED      0x00
#define ACFE_ENABLED       ADC_SC2_ACFE_MASK

// Compare Function Greater Than Enable: Greater, or Less.
// 比较功能大于比较使能：大于或者小于
#define ACFGT_GREATER      ADC_SC2_ACFGT_MASK
#define ACFGT_LESS         0x00

// Compare Function Range Enable: Enabled or Disabled.
// 比较功能范围使能：使能或者禁止
#define ACREN_ENABLED      ADC_SC2_ACREN_MASK
#define ACREN_DISABLED     0x00

// DMA enable: enabled or disabled.
// DMA使能：使能或者禁止
#define DMAEN_ENABLED      ADC_SC2_DMAEN_MASK
#define DMAEN_DISABLED     0x00


//ADC转换的电压参考选择
#define REFSEL_EXT         0x00
#define REFSEL_ALT         0x01
#define REFSEL_RES         0x02     /* reserved */
#define REFSEL_RES_EXT     0x03     /* reserved but defaults to Vref */

////ADCSC3

// 校准开始或者关闭
#define CAL_BEGIN          ADC_SC3_CAL_MASK
#define CAL_OFF            0x00


// 指示校准失败成功的状态
#define CALF_FAIL          ADC_SC3_CALF_MASK
#define CALF_NORMAL        0x00

// ADC连续转换或者一次转换
#define ADCO_CONTINUOUS    ADC_SC3_ADCO_MASK
#define ADCO_SINGLE        0x00

// 平均采样使能或者禁止
#define AVGE_ENABLED       ADC_SC3_AVGE_MASK
#define AVGE_DISABLED      0x00

// MCU产生中断前的平均次数4,8,16，或者32
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03



//外部函数接口声明
void  adc_init(ADC_Ch);              //AD初始化
uint16_t ad_once(ADC_Ch, ADC_nbit);    //采集一次一路模拟量的AD值
uint16_t ad_ave(ADC_Ch ch, ADC_nbit bit, uint8_t N); //均值滤波
uint16_t ad_sum(ADC_Ch ch, ADC_nbit bit, uint8_t N);
void  adc_start (ADC_Ch, ADC_nbit);    //开始adc转换
void  adc_stop  (ADCn);                      //停止ADC转换
void  adc_DMA_init(ADC_Ch ch) ;


#endif /* __ADC16_H__ */
