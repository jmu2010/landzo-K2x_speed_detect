/***********************************************************
 * �ļ���       ��main.c
 * ˵��         ���û�Ӧ�ó���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/


#include "include.h" // Include.h �����û��Զ����ͷ�ļ� 


/***********************��������***************************/
extern uint8 TIME0flag_5ms   ;
extern uint8 TIME0flag_10ms  ;
extern uint8 TIME0flag_20ms  ;
extern uint8 TIME0flag_50ms  ;
extern uint8 TIME0flag_80ms  ;
extern uint8 TIME0flag_100ms ;
extern uint8 TIME0flag_1s ;
extern  uint8_t  LPT_INT_count ; //LPT ���������
uint16 count = 0;
uint16 lptmr_CNT = 0;
/***********************��������***************************/
#define  LIN_COUT    100



/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  gpio_init (PTD6, GPO, 1);//LED GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  pit_init_ms(PIT0, 5);    //��ʼ��PIT0����ʱʱ��Ϊ�� 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //�����ж����ȼ� 8
  setIRQadrr(INT_PIT,PIT_IRQHandler);    //����PIT0��ʱ���жϵ�ַ���� 

  lptmr_counter_init(LPT0_ALT2, LIN_COUT, 2, LPT_Rising);//��ʼ����������� PTC5���벶��ţ�LIN_cout =100�������ز��� 
  set_irq_priority((INT_LPTimer + INC_IRQ), 0);//�����ж����ȼ� 0
  setIRQadrr(INT_LPTimer, LPT_Handler);  //�����жϵ�ַ���� 

  OLED_Init(); // OLED��ʼ��
  lptmr_counter_clean();  //����������������ֵ
  EnableInterrupts ;
  
  OLED_Print_Str(0,0,"Positec");  //�����ַ�����ʾ
  OLED_Print_Str(0,2,"Speed:"); 

  while(1)
  {
    if(TIME0flag_1s == 1)
    {
      TIME0flag_1s = 0;
      count = lptmr_pulse_get();   // ��ȡ����
      //�����������������ֵ
      lptmr_CNT = LPT_INT_count * LIN_COUT +  count;  //���10ms��������� 
 
      OLED_Print_Str(75,2,"   ");    //��ʾ����ֵ
      OLED_Print_Num(75,2,lptmr_CNT);    //��ʾ����ֵ
      //delay_ms(50);
      //OLED_Fill(0x0);//���� 
      LPT_INT_count = 0;  //���LPT�жϴ���   
    }
    if(TIME0flag_10ms == 1) 
    {
      TIME0flag_10ms = 0;
      PTD6_OUT = ~PTD6_OUT;
    }
  }
}