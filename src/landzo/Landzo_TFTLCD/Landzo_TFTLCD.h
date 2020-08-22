#ifndef _TFT_H_
#define _TFT_H_

//#include "K6x_gpio.h"
#include "include.h" 

//五向导航键定义
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4
#define MID   5

//刷屏速度记录
extern uint32_t PointNum;
extern uint8_t  FPS;

//tft控制端口
#define	LCD_CS_SET      gpio_set(PTB3,1)    //片选端口  	  
#define	LCD_RS_SET	gpio_set(PTB8,1)   //数据/命令     	   
#define	LCD_WR_SET	gpio_set(PTB2,1)   //写数据			  
#define	LCD_RD_SET	gpio_set(PTB9,1)    //读数据			 
								    
#define	LCD_CS_CLR      gpio_set(PTB3,0)    //片选端口  
#define	LCD_RS_CLR	gpio_set(PTB8,0)     //数据/命令      
#define	LCD_WR_CLR	gpio_set(PTB2,0)     //写数据			  
#define	LCD_RD_CLR	gpio_set(PTB9,0)     //读数据

//写16位数据函数  
#define LCD_WR_DATA(data)   {LCD_RS_SET;LCD_CS_CLR;DataOut(data);LCD_WR_CLR;LCD_WR_SET;LCD_CS_SET;}

//写寄存器函数 
#define LCD_WR_REG(data)    {LCD_RS_CLR;LCD_CS_CLR;DataOut(data);LCD_WR_CLR;LCD_WR_SET;LCD_CS_SET;} 


#define LCD_W 160
#define LCD_H 128

#define MAX_CHAR_POSX   154
#define MAX_CHAR_POSY   116

#define MAX_CHINESE_POSX   144
#define MAX_CHINESE_POSY   112


//颜色
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
#define BROWN 	         0XBC40 //棕色
#define BRRED 	         0XFC07 //棕红色
#define GRAY  	         0X8430 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色

//以上三色为PANEL的颜色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	  

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

