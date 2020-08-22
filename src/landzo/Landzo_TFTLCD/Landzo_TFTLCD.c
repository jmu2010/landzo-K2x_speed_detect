//======================================================================
//�������--����
//�汾��V1.0
//ʵ��ƽ̨��K60DN512ϵͳ��
//�޸ģ�2014.1.10
//ʹ��˵�������ļ�������TFT�ĵײ�����������ʹ��PTC�˿ڣ�����KL25û��
//������16���ŵĶ˿ڡ�ʹ��ʱ����ϸ�Ķ�ע�͡�
//�������г�ʼ����Ӧ������
//
//�������ţ�
//PTC0-D0,PTC1-D1,PTC2-D2.PTC3-D3,PTC4-D4,PTC5-D5,PTC6-D6,PTC7-D7,PTC8-D8,PTC9-D9
//PTC10-D10,PTC11-D11,PTC12-D12,PTC13-D13,PTC16-D14,PTC17-D15
//
//�������ţ�
//PTB1-BK,PTB2-WR,PTB3-CS,PTB8-RS,PTB9-RD��PTB10-RST
//
//�������
//PTB11-UP,PTB16-MID,PTB17-DOWN,PTB18-RIGHT,PTB19-LEFT
//======================================================================



#include "KL2x_delay.h"
#include "font.h"
#include "Landzo_TFTLCD.h"

uint16_t BACK_COLOR = BLACK;                           //��ʼĬ�ϱ�����ɫ
uint16_t LCD_DeviceCode;
uint16_t PRINT_COLOR = WHITE;                          //ʹ��printf����ʱ��ʾ����ɫ
uint32_t PointNum = 0; //���ڲ���ˢ���ٶ�
uint8_t  FPS;



//=============================================
//���������������KL25û��һ���˿���������16����
//����˲���ֱ�������ݼĴ������,�˴�����ʹ��PTC
//������PTCȱʧ14��15���ţ�������Ҫ�����޸Ĳſ���
//
//=============================================
void DataOut(int32_t data)
{
    GPIO_PDOR_REG(PTC_BASE_PTR) = data | ((data&0x0000C000)<<2); //��dataд�����ݼĴ�������ȡ14��15λ�������Ƶ�16��17λ�����
    
}


int32_t DataIn()
{
    int32_t data = GPIO_PDIR_REG(PTC_BASE_PTR) | ((GPIO_PDIR_REG(PTC_BASE_PTR)&0x00030000)>>2);
    return data;
	
}

 
//=============================================
//m^n����
//
//=============================================
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;	 
    while(n--)result*=m;    
    return result;
}

 
//============================================
//��ָ���Ĵ�������
//
//============================================
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{										   
    uint16_t t,ID;

// 	LCD_WR_REG(__RBIT(LCD_Reg<<16));          //д��Ҫ���ļĴ�����  
      LCD_WR_REG(LCD_Reg);          //д��Ҫ���ļĴ�����  
    
    GPIO_PDDR_REG(PTC_BASE_PTR) &= 0xFFFF0000;      //PTC ��16λ��������	
    for(t=0;t<16;t++) PORT_PCR_REG(PORTC_BASE_PTR,t) |= PORT_PCR_PS_MASK;   //��16λȫ������ 
    LCD_RS_SET;
    LCD_CS_CLR;                  //��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
    LCD_RD_CLR;
                       
    LCD_RD_SET;
    ID = DataIn();  
    LCD_CS_SET; 

    GPIO_PDDR_REG(PTC_BASE_PTR) |= 0x0000FFFF;  //PTC ��16λ�������
    for(t=0;t<16;t++)  
      PORT_PCR_REG(PORTC_BASE_PTR,t) &= ~(PORT_PCR_PE_MASK);   //��16λ�ر�����������
    GPIO_PDOR_REG(PTC_BASE_PTR) |= 0x0000FFFF;  //PTC ��16λȫ�������
    
    return ID;  
} 
 
//====================================================================
//��ָ���Ĵ�����ַд����
//
//====================================================================
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
  LCD_WR_REG( (LCD_Reg));  
  LCD_WR_DATA( (LCD_RegValue));	 	
}

//====================================================================
// ���ù��λ��
//Xpos:������
//Ypos:������
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
//����
//x:0~159
//y:0~127
//2013.06.04
//====================================================================
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	  if((x >= LCD_W) || (y >= LCD_H) ) return;
    
	 LCD_SetCursor(x,y);               //���ù��λ�� 
// 	 LCD_WR_REG(__RBIT(0x22<<16));     //��ʼд��GRAM
// 	 LCD_WR_DATA(__RBIT(color<<16));   //д��ɫ	
	
	 LCD_WR_REG((0x22));     //��ʼд��GRAM
	 LCD_WR_DATA((color));   //д��ɫ		
}
 
//====================================================================
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//color:�ַ���ɫ
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//2013.6.4 
//====================================================================
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode)
{  
    uint8_t temp;
    uint8_t pos,t;
    uint16_t colortemp=color;  
 
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;   
	
    num=num-' ';                                           //�õ�ƫ�ƺ��ֵ
    if(!mode)                                              //�ǵ��ӷ�ʽ
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];          //����1206����
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)colortemp=color;
                else colortemp=BACK_COLOR;
                LCD_DrawPoint(x+t,y+pos,colortemp);	
                temp>>=1; 
            }
    	}	
    }
    else                                                      //���ӷ�ʽ
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];              //����1206����
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);   //��һ����     
                temp>>=1; 
            }
    	}
    }           	 	  
}
//====================================================================
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//color:�ַ���ɫ
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//mode2: (1)������ʾ����0����ɫ��ʾ
//2013.1.14 
//==================================================================== 
void LCD_ShowChar2(uint16_t x,uint16_t y,uint8_t num,uint16_t color,uint8_t mode,uint8_t mode2)
{  
    uint8_t temp;
    uint8_t pos,t;
    uint16_t colortemp=color;  
 
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;      	   
    num=num-' ';                                           //�õ�ƫ�ƺ��ֵ
    if(!mode)                                              //�ǵ��ӷ�ʽ
    {
        for(pos=0;pos<12;pos++)
    	{
          temp=asc2_1206[num][pos];          //����1206����
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
    }else                                                      //���ӷ�ʽ
    {
        for(pos=0;pos<12;pos++)
    	{
            temp=asc2_1206[num][pos];              //����1206����
            for(t=0;t<6;t++)
            {                 
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);   //��һ����     
                temp>>=1; 
            }
    	}
    }           	 	  
}  
 

//====================================================================
// ��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//col:������ɫ
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//mode2:������ʾ��ɫ��ʾ
//��16����
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
//��ʾ��ֵ
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//mode2:������ʾ��ɫ��ʾ
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
//����
//x1,y1:�������
//x2,y2:�յ����� 
//cl: ��ɫ
//2013.6.4 
//====================================================================
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t cl)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 

  delta_x=x2-x1;                               //������������ 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1;                        //���õ������� 
  else if(delta_x==0)incx=0;                  //��ֱ�� 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;                  //ˮƽ�� 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x;       //ѡȡ�������������� 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )                 //������� 
  {  
          LCD_DrawPoint(uRow,uCol,cl);         //���� 
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
//                                ��ָ�����������ָ����ɫ
//�����С: (xend-xsta)*(yend-ysta)
//2013.1.14 
//====================================================================
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
  uint16_t i,j;
  uint16_t xlen=0;
  xlen=xend-xsta+1;	   
  
  for(i=ysta;i<=yend;i++)
    {
          LCD_SetCursor(xsta,i);                  //���ù��λ�� 
// 		 LCD_WR_REG(__RBIT(0x22<<16));          //��ʼд��GRAM
                   LCD_WR_REG( (0x22));          //��ʼд��GRAM
          for(j=0;j<xlen;j++)LCD_WR_DATA(color);  //д����ɫ	   	    
    }					  	    
} 

//====================================================================
//��������
//Color:Ҫ���������ɫ
//2013.06.04
//====================================================================
void LCD_Clear(uint16_t Color)
{
    uint32_t index=0; 
    BACK_COLOR=Color;    
    LCD_SetCursor(0,0);                   //���ù��λ�� 
    // 	LCD_WR_REG(__RBIT(0x22<<16));         //��ʼд��GRAM	 
    LCD_WR_REG((0x22));         //��ʼд��GRAM	 	 	
    for(index=0;index<20480;index++)
    {
    // 		LCD_WR_DATA(__RBIT(Color<<16));  
      LCD_WR_DATA( (Color));
    //����ˢ���ٶ�
      PointNum++;
  }  	
}  
 
//====================================================================
//LCD��ʼ��
//
//====================================================================
void LGDP4511_Init(void)
{      
        gpio_init(PTB2,GPO,1); //����WR �����������ʼ����ߵ�ƽ 
        gpio_init(PTB8,GPO,1); //����RS
        gpio_init(PTB9,GPO,1); //����RD
        gpio_init(PTB3,GPO,1); //����CS
        gpio_init(PTB1,GPO,1); //����BK�������
	
	//���򵼺���IO��ʼ��,�˴�ʹ��ֱ�Ӷ�ȡIO��ƽ������
	//��Ҫ�ж�ģʽ��ֱ���޸ĳ�ʼ���������IRQMODE
	//�����ж���ں����м����������	
        gpio_init(PTB11,GPI,1);
        gpio_init(PTB16,GPI,1);
        gpio_init(PTB17,GPI,1);
        gpio_init(PTB18,GPI,1);
        gpio_init(PTB19,GPI,1);
        
	//�����������IOΪ���������
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
	//TFT��λ
        gpio_init(PTB10,GPO,0);   //����RST�����ͽ��и�λ
	delay_ms(50);
	gpio_init(PTB10,GPO,1);   //����RST��ʼ����
	
        LCD_WriteReg(0x00,0x00001);        //����ʱ��
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
        LCD_WriteReg(0x03,0x0028);     //0x03,entry mode set ,�˼Ĵ����ɵ���RAM��ַ�Լӵ�˳��
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
