/***********************************************************
 * 文件名       ：main.c
 * 说明         ：用户应用程序
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/


#include "include.h" // Include.h 包含用户自定义的头文件 


/***********************参数定义***************************/
extern uint8 TIME0flag_5ms   ;
extern uint8 TIME0flag_10ms  ;
extern uint8 TIME0flag_20ms  ;
extern uint8 TIME0flag_50ms  ;
extern uint8 TIME0flag_80ms  ;
extern uint8 TIME0flag_100ms ;
extern uint8 TIME0flag_1s ;
extern  uint8_t  LPT_INT_count ; //LPT 脉冲计数器
uint16 count = 0;
uint16 lptmr_CNT = 0;
/***********************参数定义***************************/
#define  LIN_COUT    100



/************************主函数****************************/
void main(void)
{
  DisableInterrupts; //禁止总中断 
  gpio_init (PTD6, GPO, 1);//LED GPIO引脚初始化配置 推挽输出 输出为高电平
  pit_init_ms(PIT0, 5);    //初始化PIT0，定时时间为： 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //设置中断优先级 8
  setIRQadrr(INT_PIT,PIT_IRQHandler);    //设置PIT0定时器中断地址函数 

  lptmr_counter_init(LPT0_ALT2, LIN_COUT, 2, LPT_Rising);//初始化脉冲计数器 PTC5输入捕获脚，LIN_cout =100，上升沿捕获 
  set_irq_priority((INT_LPTimer + INC_IRQ), 0);//设置中断优先级 0
  setIRQadrr(INT_LPTimer, LPT_Handler);  //设置中断地址函数 

  OLED_Init(); // OLED初始化
  lptmr_counter_clean();  //清空脉冲计数器计算值
  EnableInterrupts ;
  
  OLED_Print_Str(0,0,"Positec");  //汉字字符串显示
  OLED_Print_Str(0,2,"Speed:"); 

  while(1)
  {
    if(TIME0flag_1s == 1)
    {
      TIME0flag_1s = 0;
      count = lptmr_pulse_get();   // 读取计数
      //保存脉冲计数器计算值
      lptmr_CNT = LPT_INT_count * LIN_COUT +  count;  //间隔10ms的脉冲次数 
 
      OLED_Print_Str(75,2,"   ");    //显示计数值
      OLED_Print_Num(75,2,lptmr_CNT);    //显示计数值
      //delay_ms(50);
      //OLED_Fill(0x0);//黑屏 
      LPT_INT_count = 0;  //清空LPT中断次数   
    }
    if(TIME0flag_10ms == 1) 
    {
      TIME0flag_10ms = 0;
      PTD6_OUT = ~PTD6_OUT;
    }
  }
}