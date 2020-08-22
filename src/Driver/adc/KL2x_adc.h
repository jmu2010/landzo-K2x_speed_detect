/***********************************************************
 * �ļ���       ��adc.h
 * ˵��         ��ADCͨ����ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/


#ifndef __KL2x_ADC_H__
#define __KL2x_ADC_H__ 

typedef enum ADCn  //ADC�˿�
{
    ADC0,
} ADCn;

//ADCͨ��
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
    
    ADC0_SE4b = 4,   // PTE29     ��֧�����ADC�����ݽ��������ADC���ᵱ�� aͨ������
    ADC0_SE5b = 5,   // PTD1      ��֧�����ADC�����ݽ��������ADC���ᵱ�� aͨ������
    ADC0_SE6b = 6,   // PTD5      ��֧�����ADC�����ݽ��������ADC���ᵱ�� aͨ������
    ADC0_SE7b = 7,   // PTD6      ��֧�����ADC�����ݽ��������ADC���ᵱ�� aͨ������
    ADC0_SE8,       // PTB0
    ADC0_SE9,       // PTB1
    ADC0_RES0,      // ����
    ADC0_SE11,      // PTC2
    ADC0_SE12,      // PTB2
    ADC0_SE13,      // PTB3
    ADC0_SE14,      // PTC0
    ADC0_SE15,      // PTC1
    ADC0_RES1,      // ����
    ADC0_RES2,      // ����
    ADC0_RES3,      // ����
    ADC0_RES4,      // ����
    ADC0_RES5,      // ����
    ADC0_RES6,      // ����
    ADC0_RES7,      // ����
    ADC0_SE23,      // PTE30
    DAC0_OUT = ADC0_SE23, // PTE30  DAC0��� ,���� ADC�����ᵱ�� ADC0_SE23  ����
    ADC0_RES8,      // ����
    ADC0_RES9,      // ����

    Temp0_Sensor,   // Temperature Sensor,�ڲ��¶Ȳ���������ADC����
    Bandgap0,       // �¶Ȳ����ṹ��϶��׼Դ   ��֧��ADC
    ADC0_RES10,     // ����
    VREFH0,         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    VREFL0,         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
    Module0_Dis,    // ��֧�� ADC
} ADC_Ch;

//����λ��
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


#define Module_disabled    AD31         //   ����ģ��
// Conversion Complete (COCO) mask
#define COCO_COMPLETE     ADC_SC1_COCO_MASK
#define COCO_NOT          0x00

// ADC interrupts: enabled, or disabled.
// ADC �ж�: ʹ�ܻ��߽�ֹ
#define AIEN_ON           ADC_SC1_AIEN_MASK
#define AIEN_OFF          0x00

// Differential or Single ended ADC input
//��ֻ��ߵ���ADC����
#define DIFF_SINGLE       0x00
#define DIFF_DIFFERENTIAL ADC_SC1_DIFF_MASK

// Power setting of ADC
//ADC��Դ����
#define ADLPC_LOW         ADC_CFG1_ADLPC_MASK
#define ADLPC_NORMAL      0x00

// Clock divisor
//ʱ�ӷ�Ƶ
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

// Long samle time, or Short sample time
// ������ʱ����߶̲���ʱ��
#define ADLSMP_LONG       ADC_CFG1_ADLSMP_MASK
#define ADLSMP_SHORT      0x00


// ADC Input Clock Source choice? Bus clock, Bus clock/2, "altclk", or the
//                                ADC's own asynchronous clock for less noise
//ADC����ʱ��Դѡ�� ���ߣ�����/2����altclk������ADC�����첽ʱ���Լ�������
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03

//// ADCCFG2

// ѡ��ͨ��A��ͨ��B
#define MUXSEL_ADCB       ADC_CFG2_MUXSEL_MASK
#define MUXSEL_ADCA       0x00

// Ansync clock output enable: enable, or disable the output of it
// �첽ʱ�����ʹ�ܣ�ʹ�ܣ����߽�ֹ���
#define ADACKEN_ENABLED   ADC_CFG2_ADACKEN_MASK
#define ADACKEN_DISABLED  0x00

// High speed or low speed conversion mode
// ���ٵ���ת��ʱ��
#define ADHSC_HISPEED     ADC_CFG2_ADHSC_MASK
#define ADHSC_NORMAL      0x00

// Long Sample Time selector: 20, 12, 6, or 2 extra clocks for a longer sample time
// ������ʱ��ѡ��20,12,6����2�������ʱ�Ӷ��ڳ�����ʱ��
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03


// Read-only status bit indicating conversion status
// ֻ��״̬λֻ��ת��״̬
#define ADACT_ACTIVE       ADC_SC2_ADACT_MASK
#define ADACT_INACTIVE     0x00

// Trigger for starting conversion: Hardware trigger, or software trigger.
// For using PDB, the Hardware trigger option is selected.
// ������ʼת��:Ӳ���������������
#define ADTRG_HW           ADC_SC2_ADTRG_MASK
#define ADTRG_SW           0x00

// ADC Compare Function Enable: Disabled, or Enabled.
//ADC�ȽϹ���ʹ�ܣ���ֹ����ʹ��
#define ACFE_DISABLED      0x00
#define ACFE_ENABLED       ADC_SC2_ACFE_MASK

// Compare Function Greater Than Enable: Greater, or Less.
// �ȽϹ��ܴ��ڱȽ�ʹ�ܣ����ڻ���С��
#define ACFGT_GREATER      ADC_SC2_ACFGT_MASK
#define ACFGT_LESS         0x00

// Compare Function Range Enable: Enabled or Disabled.
// �ȽϹ��ܷ�Χʹ�ܣ�ʹ�ܻ��߽�ֹ
#define ACREN_ENABLED      ADC_SC2_ACREN_MASK
#define ACREN_DISABLED     0x00

// DMA enable: enabled or disabled.
// DMAʹ�ܣ�ʹ�ܻ��߽�ֹ
#define DMAEN_ENABLED      ADC_SC2_DMAEN_MASK
#define DMAEN_DISABLED     0x00


//ADCת���ĵ�ѹ�ο�ѡ��
#define REFSEL_EXT         0x00
#define REFSEL_ALT         0x01
#define REFSEL_RES         0x02     /* reserved */
#define REFSEL_RES_EXT     0x03     /* reserved but defaults to Vref */

////ADCSC3

// У׼��ʼ���߹ر�
#define CAL_BEGIN          ADC_SC3_CAL_MASK
#define CAL_OFF            0x00


// ָʾУ׼ʧ�ܳɹ���״̬
#define CALF_FAIL          ADC_SC3_CALF_MASK
#define CALF_NORMAL        0x00

// ADC����ת������һ��ת��
#define ADCO_CONTINUOUS    ADC_SC3_ADCO_MASK
#define ADCO_SINGLE        0x00

// ƽ������ʹ�ܻ��߽�ֹ
#define AVGE_ENABLED       ADC_SC3_AVGE_MASK
#define AVGE_DISABLED      0x00

// MCU�����ж�ǰ��ƽ������4,8,16������32
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03



//�ⲿ�����ӿ�����
void  adc_init(ADC_Ch);              //AD��ʼ��
uint16_t ad_once(ADC_Ch, ADC_nbit);    //�ɼ�һ��һ·ģ������ADֵ
uint16_t ad_ave(ADC_Ch ch, ADC_nbit bit, uint8_t N); //��ֵ�˲�
uint16_t ad_sum(ADC_Ch ch, ADC_nbit bit, uint8_t N);
void  adc_start (ADC_Ch, ADC_nbit);    //��ʼadcת��
void  adc_stop  (ADCn);                      //ֹͣADCת��
void  adc_DMA_init(ADC_Ch ch) ;


#endif /* __ADC16_H__ */
