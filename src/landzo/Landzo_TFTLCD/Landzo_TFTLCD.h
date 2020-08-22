#ifndef _TFT_H_
#define _TFT_H_

//#include "K6x_gpio.h"
#include "include.h" 

//���򵼺�������
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4
#define MID   5

//ˢ���ٶȼ�¼
extern uint32_t PointNum;
extern uint8_t  FPS;

//tft���ƶ˿�
#define	LCD_CS_SET      gpio_set(PTB3,1)    //Ƭѡ�˿�  	  
#define	LCD_RS_SET	gpio_set(PTB8,1)   //����/����     	   
#define	LCD_WR_SET	gpio_set(PTB2,1)   //д����			  
#define	LCD_RD_SET	gpio_set(PTB9,1)    //������			 
								    
#define	LCD_CS_CLR      gpio_set(PTB3,0)    //Ƭѡ�˿�  
#define	LCD_RS_CLR	gpio_set(PTB8,0)     //����/����      
#define	LCD_WR_CLR	gpio_set(PTB2,0)     //д����			  
#define	LCD_RD_CLR	gpio_set(PTB9,0)     //������

//д16λ���ݺ���  
#define LCD_WR_DATA(data)   {LCD_RS_SET;LCD_CS_CLR;DataOut(data);LCD_WR_CLR;LCD_WR_SET;LCD_CS_SET;}

//д�Ĵ������� 
#define LCD_WR_REG(data)    {LCD_RS_CLR;LCD_CS_CLR;DataOut(data);LCD_WR_CLR;LCD_WR_SET;LCD_CS_SET;} 


#define LCD_W 160
#define LCD_H 128

#define MAX_CHAR_POSX   154
#define MAX_CHAR_POSY   116

#define MAX_CHINESE_POSX   144
#define MAX_CHINESE_POSY   112


//��ɫ
#define WHITE         	 0xFFFF
#define BLACK          	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 	         0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 	         0XBC40 //��ɫ
#define BRRED 	         0XFC07 //�غ�ɫ
#define GRAY  	         0X8430 //��ɫ

//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ

//������ɫΪPANEL����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	  

void LGDP4511_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t col,uint8_t mode,uint8_t mode2);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t color,uint8_t mode,uint8_t mode2);
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t cl);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode);
void LCD_ShowChar2(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode,uint8_t mode2);
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode,uint8_t mode2);


#endif

