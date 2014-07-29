//#include "font.c"
#ifndef LCD_HORIZONTAL_H
#define LCD_HORIZONTAL_H

#include "LPC17xx.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************/
/* Colori ammessi	      */
/* Color = RRRRR GGGGGG BBBBB */
/******************************/
#define no_bg           0x0001			// nessun colore
#define BLACK           0x0000
#define WHITE           0xFFFF
#define RED             0x8000
#define GREEN           0x0400
#define BLUE            0x0010
#define YELLOW          0xFFF0
#define CYAN            0x0410
#define MAGENTA         0x8010
#define BROWN           0xFC00
#define OLIVE	        0x8400

#define BRIGHT_RED      0xF800
#define BRIGHT_GREEN    0x07E0
#define BRIGHT_BLUE     0x001F
#define BRIGHT_YELLOW   0xFFE0
#define BRIGHT_CYAN     0x07FF
#define BRIGHT_MAGENTA  0xF81F

#define LIGHT_GRAY      0x8410
#define DARK_GRAY       0x4208
#define LIGHT_BLUE      0x841F
#define LIGHT_GREEN     0x87F0
#define LIGHT_CYAN      0x87FF
#define LIGHT_RED       0xFC10
#define LIGHT_MAGENTA   0xFC1F

#define GRAY0       	0xE71C
#define GRAY1         	0xC618
#define GRAY2           0xA514
#define GRAY3           0x630C
#define GRAY4           0x4208
#define GRAY5	        0x2104
#define	GRAY6	        0x3186	

#define	BLUE0	        0x1086	
#define	BLUE1       	0x3188 
#define	BLUE2          	0x4314
#define BLUE3         	0x861C	

#define	CYAN0	        0x3D34
#define	CYAN1           0x1DF7		

#define	GREEN0          0x0200	
#define	GREEN1          0x0208	

//------------ Color For Build Button -------------
#define BT_RED          1
#define BT_GREEN        2
#define BT_BLUE         3
#define	BT_YELLOW       4
#define BT_WHITE        5
#define BT_CYAN         6
#define BT_MAGENTA      7
#define BT_BROWN        8

#define BT_LRED         9
#define BT_LGREEN       10
#define BT_LBLUE        11
#define	BT_LYELLOW      12
#define BT_LWHITE       13
#define BT_LCYAN        14
#define BT_LMAGENTA     15
#define BT_LBROWN       16


//-------------interfaccia HW ---------------------

unsigned long timeval;

//Hardware Interface
void delay_ms(unsigned long ms);															// Delay Time


/* GLCD & Touch(ADS7846) Hardware Interface */
#define GLCD_CS_HIGH()  LPC_GPIO0->FIOPIN |=  (1 << 6);										// P0.6(CS=High:Disable)
#define GLCD_CS_LOW()	LPC_GPIO0->FIOPIN &= ~(1 << 6);										// P0.6(CS=Low:Enable)

#define GLCD_BL_ON()	LPC_GPIO4->FIOPIN |=  (1UL << 28); 									// P4.28(BL=High:ON)
#define	GLCD_BL_OFF()   LPC_GPIO4->FIOPIN &= ~(1UL << 28); 									// P4.28(BL=Low:OFF)

#define	TCS_CS_HIGH()	LPC_GPIO1->FIOPIN |=  (1UL<<21);    								// P1.21(CS=High:Disable)
#define	TCS_CS_LOW()    LPC_GPIO1->FIOPIN &= ~(1UL<<21);   									// P1.21(CS=Low:Enable)

#define PENIRQ_MASK     0x20 																// 00000000 00100000 00000000 00000000 = P0.21 => 8Bit Mask(00100000)
#define PENIRQ_READ()  (LPC_GPIO0->FIOPIN >> 16) & PENIRQ_MASK;   							// 00000000 00100000 00000000 00000000 = P0.21 => 8Bit Mask(00100000)



/* Prototype Function */
void Initial_Hardware(void);											// Config Hardware Interface

/* Function Control Graphic LCD ET-TFT240320TP-3.2 Rev.B (Driver SPFD5408A)*/
unsigned char GLCD_SPI_Read_Write(unsigned char DataByte);
void GLCD_Write_Command(unsigned char GLCD_Command);
void GLCD_Write_Data(unsigned int GLCD_Data);
void Initial_GLCD_Hor(void);

/* Function Control SPI Touch-Screen(ADS7846)*/
char Get_Status_Touch(void);
unsigned char TSC_SPI_Write(unsigned char DataByte);
void TCS_SPI_Read_Hor(void);
void TCS_Average_X_Y(unsigned char num);
void Touch_Calibrate_Hor(void);
void TCS_Set_Matrix_Hor(void);
void TCS_Get_Point(char num);

/* Function Application of GLCD */
void bg_color(long bg_color);
void plot_dot_hor(unsigned long Xadd,unsigned long Yadd,unsigned long color);
void plot_mark_hor(long x1,long y1,long x2,long y2,long color);
void text_7x11_hor(char row,long adx,long ady,long fg_clr,long bg_clr);
void lcd_printStr_hor(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
void lcd_print3Cha_hor(char ch1,char ch2,char ch3, long cur_x,long cur_y,long fg_color,long bg_color);
void dec_to_ascii(long num);
void plot_tab_hor(unsigned long color);      
void Line(long x_1,long y_1,long x_2,long y_2,char line_type,long color);
void Circle(long CX,long CY,char Radius,char tick,long color,char fill);
void Ellipse(unsigned int h,unsigned int k, long rx,long ry,char tick,long color,char fill);
void Rectan(long x1,long y1,long x2,long y2, char tick,long color,char fill);     
void button_build_hor(long px,long py,long width,long high, long c_bt,unsigned char *tx,char num_tx,long c_tx);
void plot_picture_hor(const char *bmp,long xad,long yad,long width,long high);  

#endif
