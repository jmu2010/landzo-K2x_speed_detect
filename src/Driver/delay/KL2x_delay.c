/***********************************************************
 * �ļ���       ��KL2x_delay.c
 * ˵��         ����ʱ
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V2.0
 *ʱ��          ��15.5.26
************************************************************/
#include "KL2x_delay.h"




/**********************************************************
*                    ������ӹ�����
*
* �������ƣ�delay_ms
* ����˵������ʱ��������׼ȷ�����ں�Ƶ��Ϊ100Mʱ��Ϊ׼ȷ
* ����˵������
* �������أ���
* ��ע    ��
***********************************************************/
void delay_ms(uint32 time)
{
  uint32 i,j;
  for(i = 0;i < time/4.9;i ++)
  {
    for(j = core_clk_khz;j > 0;j --)
    {
       asm("nop");    
    }
  }
}