//======================================================================
//蓝宙电子--例程
//版本：V1.0
//实验平台：K60DN512系统板
//修改：2014.1.10
//使用说明：本文件包含了TFT的底层驱动，驱动使用PTC端口，由于KL25没有
//完整的16引脚的端口。使用时请仔细阅读注释。
//本例程中初始化对应的引脚
//
//数据引脚：
//PTC0-D0,PTC1-D1,PTC2-D2.PTC3-D3,PTC4-D4,PTC5-D5,PTC6-D6,PTC7-D7,PTC8-D8,PTC9-D9
//PTC10-D10,PTC11-D11,PTC12-D12,PTC13-D13,PTC16-D14,PTC17-D15
//
//控制引脚：
//PTB1-BK,PTB2-WR,PTB3-CS,PTB8-RS,PTB9-RD，PTB10-RST
//
//五向键：
//PTB11-UP,PTB16-MID,PTB17-DOWN,PTB18-RIGHT,PTB19-LEFT
//======================================================================



#include "KL2x_delay.h"
#include "font.h"
#include "Landzo_TFTLCD.h"

uint16_t BACK_COLOR = BLACK;                           //初始默认背景颜色
uint16_t LCD_DeviceCode;
uint16_t PRINT_COLOR = WHITE;                          //使用printf函数时显示的颜色
uint32_t PointNum = 0; //用于测试刷屏速度
uint8_t  FPS;



//=============================================
//数据输入输出，因KL25没有一个端口是完整的16引脚
//，因此不能直接用数据寄存器输出,此代码是使用PTC
//口由于PTC缺失14，15引脚，所以需要部分修改才可行
//
//=============================================
void DataOut(int32_t data)
{
    GPIO_PDOR_REG(PTC_BASE_PTR) = data | ((data&0x0000C000)<<2); //将data写入数据寄存器并提取14和15位数据左移到16，17位输出，
    
}


int32_t DataIn()
{
    int32_t data = GPIO_PDIR_REG(PTC_BASE_PTR) | ((GPIO_PDIR_REG(PTC_BASE_PTR)&0x00030000)>>2);
    return data;
	
}

 
//=============================================
//m^n函数
//
//=============================================
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;	 
    while(n--)result*=m;    
    return result;
}

 
//============================================
//读指定寄存器数据
//
//============================================
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{										   
    uint16_t t,ID;

// 	LCD_WR_REG(__RBIT(LCD_Reg<<16));          //写入要读的寄存器号  
      LCD_WR_REG(LCD_Reg);          //写入要读的寄存器号  
    
    GPIO_PDDR_REG(PTC_BASE_PTR) &= 0xFFFF0000;      //PTC 低16位设置输入	
    for(t=0;t<16;t++) PORT_PCR_REG(PORTC_BASE_PTR,t) |= PORT_PCR_PS_MASK;   //低16位全部上拉 
    LCD_RS_SET;
    LCD_CS_CLR;                  //读取数据(读寄存器时,并不需要读2次)
    LCD_RD_CLR;
                       
    LCD_RD_SET;
    ID = DataIn();  
    LCD_CS_SET; 

    GPIO_PDDR_REG(PTC_BASE_PTR) |= 0x0000FFFF;  //PTC 低16位设置输出
    for(t=0;t<16;t++)  
      PORT_PCR_REG(PORTC_BASE_PTR,t) &= ~(PORT_PCR_PE_MASK);   //低16位关闭上下拉电阻
    GPIO_PDOR_REG(PTC_BASE_PTR) |= 0x0000FFFF;  //PTC 低16位全部输出高
    
    return ID;  
} 
 
//====================================================================
//对指定寄存器地址写数据
//
//====================================================================
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
  LCD_WR_REG( (LCD_Reg));  
  LCD_WR_DATA( (LCD_RegValue));	 	
}

//====================================================================
// 设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
//2013.06.04
//====================================================================
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  if(Xpos > LCD_W) Xpos = LCD_W;
  if(Ypos > LCD_H) Ypos = LCD_H;

  LCD_WriteReg(0x44, (uint16_t)((0x83<<8)|Xpos)); //128 Line
  LCD_WriteReg(0x45, (uint16_t)((0x9F<<8)|Ypos)); //160 Dots			

  Ypos = 131 - Ypos; 
  LCD_WriteReg(0x21,(uint16_t)((Xpos<<8)|Ypos)); //Set RAM address	
} 



//====================================================================
//画点
//x:0~159
//y:0~127
//2013.06.04
//====================================================================
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	  if((x >= LCD_W) || (y >= LCD_H) ) return;
    
	 LCD_SetCursor(x,y);               //设置光标位置 
// 	 LCD_WR_REG(__RBIT(0x22<<16));     //开始写入GRAM
// 	 LCD_WR_DATA(__RBIT(color<<16));   //写颜色	
	
	 LCD_WR_REG((0x22));     //开始写入GRAM
	 LCD_WR_DATA((color));   //写颜色		
}
 
//====================================================================
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//color:字符颜色
//mode:叠加方式(1)还是非叠加方式(0)
//2013.6.4 
//====================================================================
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode)
{  
    uint8_t temp;
    uint8_t pos,t;
    uint16_t colortemp=color;  
 
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;   
	
    num=num-' ';                                           //得到偏移后的值
    if(!mode)                                              //非叠加方式
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];          //调用1206字体
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)colortemp=color;
                else colortemp=BACK_COLOR;
                LCD_DrawPoint(x+t,y+pos,colortemp);	
                temp>>=1; 
            }
    	}	
    }
    else                                                      //叠加方式
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];              //调用1206字体
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);   //画一个点     
                temp>>=1; 
            }
    	}
    }           	 	  
}
//====================================================================
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//color:字符颜色
//mode:叠加方式(1)还是非叠加方式(0)
//mode2: (1)正常显示，（0）反色显示
//2013.1.14 
//==================================================================== 
void LCD_ShowChar2(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode,uint8_t mode2)
{  
    uint8_t temp;
    uint8_t pos,t;
    uint16_t colortemp=color;  
 
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;      	   
    num=num-' ';                                           //得到偏移后的值
    if(!mode)                                              //非叠加方式
    {
        for(pos=0;pos<12;pos++)
    	{
          temp=asc2_1206[num][pos];          //调用1206字体
          for(t=0;t<6;t++)
          {     
            if(mode2)
            {            
              if(temp&0x01)colortemp=color;
              else colortemp=BACK_COLOR;
            }
            else 
            {
              if(temp&0x01)colortemp=BACK_COLOR;
              else colortemp=color;
            }
            LCD_DrawPoint(x+t,y+pos,colortemp);	
            temp>>=1; 
          }
    	}	
    }else                                                      //叠加方式
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];              //调用1206字体
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);   //画一个点     
                temp>>=1; 
            }
    	}
    }           	 	  
}  
 

//====================================================================
// 显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//col:字体颜色
//mode:叠加方式(1)还是非叠加方式(0)
//mode2:正常显示或反色显示
//用16字体
//2013.6.4 
//====================================================================
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t col,uint8_t mode,uint8_t mode2)
{   
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=12;}
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(BACK_COLOR);}
				
        LCD_ShowChar2(x,y,*p,col,mode,mode2);
        x+=6;
        p++;
    }  
}

//==================================================================== 
//显示数值
//x,y :起点坐标	 
//len :数字的位数
//color:颜色
//num:数值(0~4294967295);	
//mode:叠加方式(1)还是非叠加方式(0)
//mode2:正常显示或反色显示
//====================================================================
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t color,uint8_t mode,uint8_t mode2)
{         	
  uint8_t t,temp;
  uint8_t enshow=0;						   
  for(t=0;t<len;t++)
  {
    temp=(num/mypow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
          LCD_ShowChar2(x+6*t,y,' ',color,mode,mode2);
          continue;
      }else enshow=1; 
             
    }
    LCD_ShowChar2(x+6*t,y,temp+'0',color,mode,mode2); 
  }
} 
 


//====================================================================
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标 
//cl: 颜色
//2013.6.4 
//====================================================================
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t cl)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 

  delta_x=x2-x1;                               //计算坐标增量 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1;                        //设置单步方向 
  else if(delta_x==0)incx=0;                  //垂直线 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;                  //水平线 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x;       //选取基本增量坐标轴 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )                 //画线输出 
  {  
          LCD_DrawPoint(uRow,uCol,cl);         //画点 
          xerr+=delta_x ; 
          yerr+=delta_y ; 
          if(xerr>distance) 
          { 
                  xerr-=distance; 
                  uRow+=incx; 
          } 
          if(yerr>distance) 
          { 
                  yerr-=distance; 
                  uCol+=incy; 
          } 
  }  
}  



//====================================================================
//                                在指定区域内填充指定颜色
//区域大小: (xend-xsta)*(yend-ysta)
//2013.1.14 
//====================================================================
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
  uint16_t i,j;
  uint16_t xlen=0;
  xlen=xend-xsta+1;	   
  
  for(i=ysta;i<=yend;i++)
    {
          LCD_SetCursor(xsta,i);                  //设置光标位置 
// 		 LCD_WR_REG(__RBIT(0x22<<16));          //开始写入GRAM
                   LCD_WR_REG( (0x22));          //开始写入GRAM
          for(j=0;j<xlen;j++)LCD_WR_DATA(color);  //写点颜色	   	    
    }					  	    
} 

//====================================================================
//清屏函数
//Color:要清屏的填充色
//2013.06.04
//====================================================================
void LCD_Clear(uint16_t Color)
{
    uint32_t index=0; 
    BACK_COLOR=Color;    
    LCD_SetCursor(0,0);                   //设置光标位置 
    // 	LCD_WR_REG(__RBIT(0x22<<16));         //开始写入GRAM	 
    LCD_WR_REG((0x22));         //开始写入GRAM	 	 	
    for(index=0;index<20480;index++)
    {
    // 		LCD_WR_DATA(__RBIT(Color<<16));  
      LCD_WR_DATA( (Color));
    //测试刷屏速度
      PointNum++;
  }  	
}  
 
//====================================================================
//LCD初始化
//
//====================================================================
void LGDP4511_Init(void)
{      
        gpio_init(PTB2,GPO,1); //设置WR 推挽输出，初始输出高电平 
        gpio_init(PTB8,GPO,1); //设置RS
        gpio_init(PTB9,GPO,1); //设置RD
        gpio_init(PTB3,GPO,1); //设置CS
        gpio_init(PTB1,GPO,1); //设置BK背光控制
	
	//五向导航键IO初始化,此处使用直接读取IO电平方法，
	//需要中断模式的直接修改初始化函数里的IRQMODE
	//并在中断入口函数中加入操作即可	
        gpio_init(PTB11,GPI,1);
        gpio_init(PTB16,GPI,1);
        gpio_init(PTB17,GPI,1);
        gpio_init(PTB18,GPI,1);
        gpio_init(PTB19,GPI,1);
        
	//设置数据输出IO为推挽输出；
        gpio_init(PTC0,GPO,1);	
        gpio_init(PTC1,GPO,1);
        gpio_init(PTC2,GPO,1);
        gpio_init(PTC3,GPO,1);
        gpio_init(PTC4,GPO,1);
        gpio_init(PTC5,GPO,1);
        gpio_init(PTC6,GPO,1);
        gpio_init(PTC7,GPO,1);
        gpio_init(PTC8,GPO,1);
        gpio_init(PTC9,GPO,1);
        gpio_init(PTC10,GPO,1);
        gpio_init(PTC11,GPO,1);
        gpio_init(PTC12,GPO,1);
        gpio_init(PTC13,GPO,1);
        gpio_init(PTC16,GPO,1);
        gpio_init(PTC17,GPO,1);
	//TFT复位
        gpio_init(PTB10,GPO,0);   //设置RST并拉低进行复位
	delay_ms(50);
	gpio_init(PTB10,GPO,1);   //拉高RST开始工作
	
        LCD_WriteReg(0x00,0x00001);        //启动时钟
        delay_ms(15);	
	
	LCD_DeviceCode = LCD_ReadReg(0x00);    
        
//Power Control
        LCD_WriteReg(0x11,0x0000);    //0x11, 
        LCD_WriteReg(0x12,0x000D);    //0x12,
        LCD_WriteReg(0x13,0x0E0D);    //0x13,
        LCD_WriteReg(0x10,0x0004);    //0x10,
        LCD_WriteReg(0x10,0x0014);    //0x10,
        LCD_WriteReg(0x12,0x001D);	  //0x12,											  
        delay_ms(40);
        LCD_WriteReg(0x13,0x2e0d);    //0x13,2e0d
        LCD_WriteReg(0x10,0x0710);    //0x10,
				
//LCD Display Control
        LCD_WriteReg(0x01,0x0113);	   //0x01,											  
        LCD_WriteReg(0x02,0x0300);		 //0x02,							  
        LCD_WriteReg(0x03,0x0028);     //0x03,entry mode set ,此寄存器可调整RAM地址自加的顺序
        LCD_WriteReg(0x07,0x0000);		 //0x07,		
        LCD_WriteReg(0x08,0x0808);     //0x08
        LCD_WriteReg(0x09,0x0000);     //0x09
        LCD_WriteReg(0x0B,0x0100);		 //0x0B		 								  
        LCD_WriteReg(0x0C,0x0000);     //0x0C
        LCD_WriteReg(0x0D,0x3229);     //0x0D
				
        LCD_WriteReg(0x1A,0x0001);		 //0x1A												  

//RAM Access
        LCD_WriteReg(0x21,0x0000);     //0x21
        LCD_WriteReg(0x23,0x0000);     //0x23
        LCD_WriteReg(0x24,0x0000);     //0x24

//Gamma Control
        LCD_WriteReg(0x30,0x0000);     //0x30
        LCD_WriteReg(0x31,0x0707);     //0x31
        LCD_WriteReg(0x32,0x0000);		 //0x32
        LCD_WriteReg(0x33,0x0007);     //0x33
        LCD_WriteReg(0x34,0x0707);     //0x34
        LCD_WriteReg(0x35,0x0003);     //0x35
        LCD_WriteReg(0x36,0x0707);     //0x36
        LCD_WriteReg(0x37,0x0706);     //0x37
        LCD_WriteReg(0x38,0x110F);     //0x38
        LCD_WriteReg(0x39,0x1008);     //0x39
				
//Coordinate Control 
        LCD_WriteReg(0x40,0x0000);     //0x40
        LCD_WriteReg(0x41,0x0000);     //0x41
        LCD_WriteReg(0x42,0x9F00);     //0x42
        LCD_WriteReg(0x43,0x0000);     //0x43
        LCD_WriteReg(0x44,0x8300);     //0x44
        LCD_WriteReg(0x45,0x9F00);     //0x45


        LCD_WriteReg(0x10,0x3710);     //0x10
        LCD_WriteReg(0x51,0x0000);     //0x51
        LCD_WriteReg(0x07,0x0005);     //0x07
        delay_ms(40);
        LCD_WriteReg(0x07,0x0025);     //0x07
	LCD_WriteReg(0x07,0x0027);     //0x07
        delay_ms(40); 
        LCD_WriteReg(0x09,0x0000);     //0x09
        LCD_WriteReg(0x07,0x0037);     //0x07
        delay_ms(50);				
        LCD_Clear(BLACK); 
}
