#include "lcd_horizontal.h"


extern const unsigned char ascii_7x11[95][14]; 	// font.c  : mi dà una tabella ASCII


// Variable for Touch Screen Function GLCD
unsigned char dif_adc_X,dif_adc_Y;
unsigned char buf_adc_X[100];
unsigned char buf_adc_Y[100];
long ave_adc_X,ave_adc_Y;
long tcs_ave_X[3];																			// Keep Value adc Touch Point X
long tcs_ave_Y[3];																			// Keep Value adc Touch Point Y
long dis_XD_hor[3] = {32,287,160}; 															// Value refer Point X at 10% of Display(X=320:0-319) 3 Position
long dis_YD_hor[3] = {215,120,24}; 															// Value refer Point Y at 10% of Display(Y=240:0-239) 3 Position
long divider,An,Bn,Cn,Dn,En,Fn,X,Y;															// Valiable for keep coefficient Calibrat and position touch Screen
char num1,num2,num3;																		// Variable for keep data Dec to Ascii


/*********************************/
/* SysTick Interrupt Handler(1ms)*/
/*********************************/
void SysTick_Handler (void)
{
  timeval++;
}



void delay_ms(unsigned long ms)
 {
  timeval = 0;  //Restart Time(mS) Count
  while(timeval != ms){timeval++;}
 }


/*****************************************/
/* Convert Dec to Ascii 3 digit(000-999) */
/*****************************************/        
void dec_to_ascii(long num)
{
  num1 = (num/100) + 0x30;		//Digit-100
  num  =  num%100;
  num2 = (num/10) + 0x30;		//Digit-10
  num3 =  (num%10) + 0x30;		//Digit-1	 
}

/******************************************/
/* Config Hardware Interface GLCD & Touch */
/* (LCD Driver SPFD5408A + Touch ADS7846) */
/* GLCD(ET-TFT240320P) SPI Mode Interface */
/* -> P0.6(GPIO:Out)    = CS GLCD         */
/* -> P0.7(SCK1:SSP1)   = SCL	GLCD	  */
/* -> P0.8(MISO1:SSP1)  = SDO GLCD		  */
/* -> P0.9(MOSI1:SSP1)  = SDI	GLCD	  */
/* -> P4.28(GPIO:Out)   = BL GLCD		  */
/* Touch GLCD : ADS7846 SPI Interface     */
/* -> P1.20(SCK0:SSP0)  = DCLK ADS7846    */
/* -> P1.21(GPIO:Out)   = CS ADS7846	  */
/* -> P1.23(MISO0:SSP0) = DOUT ADS7846	  */
/* -> P1.24(MOSI0:SSP0) = DIN ADS7846	  */
/* -> P0.21(GPIO:In)    = PENIRQ ADS7846  */
/******************************************/
void Initial_Hardware()
{
  /* Config P1.20,P1.21,P1.23,P1.24 to SSP0 For Read Touch LCD(ADS7846) */
  // P1.20(SCK0:SSP0)  = DCLK ADS7846
  // P1.21(GPIO:Out)   = CS ADS7846
  // P1.23(MISO0:SSP0) = DOUT ADS7846
  // P1.24(MOSI0:SSP0) = DIN ADS7846
  // P0.21(GPIO:In)    = PENIRQ ADS7846              
  LPC_PINCON->PINSEL3 &= ~(3UL<<10); 														// Reset P1.21 Mode = GPIO
  LPC_GPIO1->FIODIR   |=  (1UL<<21);														// P1.21 = ADS7846 CS(Output)
  LPC_GPIO1->FIOPIN   |=  (1UL<<21);    													// P1.21 = High 

  LPC_PINCON->PINSEL1 &= ~(3UL<<10); 														// Reset P0.21 Mode = GPIO
  LPC_GPIO0->FIODIR   &= ~(1UL<<21);														// P0.21 = PENIRQ(Input)

  //Config SSP0 Pin Connect
  LPC_PINCON->PINSEL3 |=  (3UL<<8);         												// Select P1.20 = SCK0(SSP0) 
  LPC_PINCON->PINSEL3 |=  (3UL<<14);        												// Select P1.23 = MISO0(SSP0) 
  LPC_PINCON->PINSEL3 |=  (3UL<<16);        												// Select P1.24 = MOSI0(SSP0) 
  	
  LPC_SC->PCONP       |=  (1 << 21);        												// Enable power to SSPI0 block  
  LPC_SC->PCLKSEL1    &= ~(3<<10);          												// PCLKSP0 = CCLK/4 (18MHz) 
  LPC_SC->PCLKSEL1    |=  (1<<10);          												// PCLKSP0 = CCLK   (72MHz) 
  LPC_SSP0->CPSR       =   72;              												// 72MHz / 72 = 1MHz(maximum of 2MHz is possible)

  LPC_SSP0->CR0        =  (   0 << 7) | 													// CPHA = 0                           
                          (   0 << 6) | 													// CPOL = 0                           
                          (   0 << 4) | 													// Frame format = SPI                 
                          ((8-1)<< 0) ; 													// Data size = 8 bits 
  LPC_SSP0->CR1        =  (   1 << 1);  													// Enable SSP-                       

  /* Config P0.6,P0.7,P0.8,P0.9 to SSP1 For Control GLCD */
  // P0.6(GPIO:Out)   = CS GLCD
  // P0.7(SCK1:SSP1)  = SCL	GLCD
  // P0.8(MISO1:SSP1) = SDO GLCD
  // P0.9(MOSI1:SSP1) = SDI	GLCD
  // P4.28(GPIO:Out)  = BL GLCD
  LPC_PINCON->PINSEL9 &= ~(3 << 24); 														// Reset P4.28 Mode = GPIO
  LPC_GPIO4->FIODIR   |=  (1UL<<28);       													// Pin P4.28 = Output(GLCD Backlight)
  LPC_GPIO4->FIOPIN   &= ~(1UL<<28);       													// Turn-OFF GLCD Backlight 

  LPC_PINCON->PINSEL0 &= ~(3UL<<12); 														// Reset P0.6 Mode = GPIO
  LPC_GPIO0->FIODIR   |=  (1 <<  6);    													// Pin P0.6 is GPIO output(CS GLCD)    
  LPC_GPIO0->FIOSET    =  (1 <<  6);    													// Set P0.6 = High 

  LPC_PINCON->PINSEL0 &= ~(3UL<<14); 														// Reset P0.7 Mode = GPIO
  LPC_PINCON->PINSEL0 |=  (2UL<<14); 														// Select P0.7 = SCK1(SSP1)
  LPC_PINCON->PINSEL0 &= ~(3UL<<16); 														// Reset P0.8 Mode = GPIO
  LPC_PINCON->PINSEL0 |=  (2UL<<16); 														// Select P0.8 = MISO1(SSP1)
  LPC_PINCON->PINSEL0 &= ~(3UL<<18); 														// Reset P0.9 Mode = GPIO
  LPC_PINCON->PINSEL0 |=  (2UL<<18); 														// Select P0.9 = MOSI1(SSP1)

  LPC_SC->PCONP       |=  (1 << 10);    													// Enable power to SSP1 block       
  LPC_SC->PCLKSEL0    |=  (2 << 20);    													// SSP1 clock = CCLK/2 (36MHz)   
  LPC_SSP1->CPSR       =  2;            													// Clock Rate = 18MHz         

  LPC_SSP1->CR0        =  (   1 << 7) | 													// CPHA = 1                           
                          (   1 << 6) | 													// CPOL = 1                           
                          (   0 << 4) | 													// Frame format = SPI                 
                          ((8-1)<< 0) ; 													// Data size = 8 bits                          
  LPC_SSP1->CR1        =  (   1 << 1);  													// Enable SSP1                       
}

/****************************/
/* GLCD SPI Sent Data 8 bit */
/****************************/
unsigned char GLCD_SPI_Read_Write(unsigned char DataByte)    
{
  LPC_SSP1->DR = DataByte;
  while (LPC_SSP1->SR & (1 << 4));      													// Wait for transfer to finish       
  return (LPC_SSP1->DR);                													// Return received value              
}

/******************************************************/
/* Write Address Command(Index Reg.)(Use Device ID=0) */
/******************************************************/
void GLCD_Write_Command(unsigned char GLCD_Command)
{
  GLCD_CS_LOW();                     														// Enable GLCD Interface

  GLCD_SPI_Read_Write(0x70);																// Sent Byte 1 = [Device ID Code:01110[0]]+[RS:0] + [R/W:0]
  GLCD_SPI_Read_Write(0x00);																// Sent Byte 2 = data 8 bit High Index Reg.: 0x00
  GLCD_SPI_Read_Write(GLCD_Command);														// Sent Byte 3 = data 8 bit Low index reg. : cmm
    
  GLCD_CS_HIGH();                           												// Disable GLCD Interface
}
 
/***************************************/
/* Write data to LCD (Use Device ID=0) */
/***************************************/
void GLCD_Write_Data(unsigned int GLCD_Data)
{	
  GLCD_CS_LOW();                     														// Enable GLCD Interface
  
  GLCD_SPI_Read_Write(0x72);                												// Byte 1 = [Device ID Code:01110[0]]+[RS:1] + [R/W:0]
  GLCD_SPI_Read_Write(GLCD_Data >> 8);      												// Byte 2 = Data 8 bit High 
  GLCD_SPI_Read_Write(GLCD_Data);           												// Byte 3 = Data 8 bit Low 
  
  GLCD_CS_HIGH();                           												// Disable GLCD Interface
}

/********************************/
/* Initial GLCD : ET-TFT240320P */
/*      (Driver SPFD5408A)      */
/********************************/
void Initial_GLCD_Hor(void)
{
  unsigned long cnt;

  GLCD_Write_Command(0x00);	 																// Command Start Oscillater
  GLCD_Write_Data(0x0001);	 																// Enable the oscillator.
  delay_ms(10);    																			// Delay for 10ms while the oscillator stabilizes.

  // Display Setting 
  GLCD_Write_Command(0x01);	 																// Configure the output drivers.
  GLCD_Write_Data(0x0100);
  GLCD_Write_Command(0x02);	 																// Configure the LCD A/C drive waveform.
  GLCD_Write_Data(0x0700);	 																// Line inversion 0x0700
  GLCD_Write_Command(0x03);    																// Configure Entry Mode
  GLCD_Write_Data(0x1228); 	 																// Color:RGB ,Incremen Ver.& decrement Hor. address ,Address update Ver.
   
  // Configure and enable the LCD power supply.
  // Power Control 
  GLCD_Write_Command(0x07);    																// Enable internal operation of the LCD controller.
  GLCD_Write_Data(0x0101);	 
  GLCD_Write_Command(0x10);
  GLCD_Write_Data(0x0000);  	
  GLCD_Write_Command(0x11);
  GLCD_Write_Data(0x0007);   	
  GLCD_Write_Command(0x12);
  GLCD_Write_Data(0x0000);  	
  GLCD_Write_Command(0x13);
  GLCD_Write_Data(0x0000); 	 	
  delay_ms(20);

  GLCD_Write_Command(0x10);
  GLCD_Write_Data(0x16B0);
  GLCD_Write_Command(0x11);
  GLCD_Write_Data(0x0037);
  delay_ms(20); 

  GLCD_Write_Command(0x12);
  GLCD_Write_Data(0x013E);
  delay_ms(20);

  GLCD_Write_Command(0x13);
  GLCD_Write_Data(0x1A00);
  GLCD_Write_Command(0x29);
  GLCD_Write_Data(0x000F);      
  delay_ms(40);              																// Delay for 40ms while the power supply stabilizes.
    	 						   
  // Clear the contents of the display buffer. 
  GLCD_Write_Command(0x22);
  for(cnt = 0;cnt < (320 * 240);cnt++)
  {
    GLCD_Write_Data(BLACK);    																// Back-Ground Color Black
  }

  GLCD_Write_Command(0x20);     															// Command Horizontal GRAM Address Set
  GLCD_Write_Data(0x0000);    																// Address Start 0x0000 
  GLCD_Write_Command(0x21);     															// Command Vertical GRAM Address Set
  GLCD_Write_Data(0x0000);    																// Address Start 0x0000 

  GLCD_Write_Command(0x50);     															// Command Horizontal Address Start Position
  GLCD_Write_Data(0x0000);      
  GLCD_Write_Command(0x51);     															// Command Horizontal Address End Position(239)
  GLCD_Write_Data(0x00EF);      
	  
  GLCD_Write_Command(0x52);     															// Command Vertical Address Start Position
  GLCD_Write_Data(0x0000);      
  GLCD_Write_Command(0x53);     															// Command Verticall Address End Position(239)
  GLCD_Write_Data(0x013F);      
  
  GLCD_Write_Command(0x60);     															// Set the number of lines to scan.
  GLCD_Write_Data(0x2700);    

  GLCD_Write_Command(0x61);     															// Enable grayscale inversion of the source outputs.
  GLCD_Write_Data(0x0001);    
    
   // GAMMA Control 
   GLCD_Write_Command(0x30);        
   GLCD_Write_Data(0x0007);    
   GLCD_Write_Command(0x31);        
   GLCD_Write_Data(0x0403);    
   GLCD_Write_Command(0x32);        
   GLCD_Write_Data(0x0404);    
   GLCD_Write_Command(0x35);        
   GLCD_Write_Data(0x0002);    
   GLCD_Write_Command(0x36);        
   GLCD_Write_Data(0x0707);    
   GLCD_Write_Command(0x37);        
   GLCD_Write_Data(0x0606);    
   GLCD_Write_Command(0x38);        
   GLCD_Write_Data(0x0106);     
   GLCD_Write_Command(0x39);        
   GLCD_Write_Data(0x0007);    
   GLCD_Write_Command(0x3C);        
   GLCD_Write_Data(0x0700);    
   GLCD_Write_Command(0x3D);        
   GLCD_Write_Data(0x0707);    
 
   GLCD_Write_Command(0x07);    															//Enable the image display.
   GLCD_Write_Data(0x0173);    		

   GLCD_BL_ON();	 																		//Back Light ON	  
   delay_ms(20);	 																		//Delay for 20ms, which is equivalent to two frames.


}

/********************************************/
/* Function Check Status Press Touch Screen */
/* Return Value : 0 = Not Touched screen    */
/*                1 = Touched screen        */
/********************************************/
char Get_Status_Touch(void)
{
  uint32_t pen_val;  
  
  pen_val = PENIRQ_READ();																	// Read PENIRQ Pin Logic
  if(pen_val == PENIRQ_MASK)																// Logic "1" = Not Press
  	return 0 ;			  																	// Not Press
  else
    return 1 ;			  																	// Press Touch screen
}

/***************************************************************************/
/*    Function SPI Write and Read Data 1 Byte from Touch Screen ADS7846    */
/***************************************************************************/
/* Parameter    : DataByte = data or command control ADS7846 (1Byte)       */ 
/* Return Value : Return value adc from touched times 1 byte Pass Function */         
/***************************************************************************/
unsigned char TCS_SPI_Write(unsigned char DataByte)			  	
{
  LPC_SSP0->DR = DataByte; 																	// Write and Read a byte on SPI interface.
  while (LPC_SSP0->SR & 0x10);          													// Wait BYS for transfer to finish 
  return (LPC_SSP0->DR);                													// Return received value 
}

/*****************************************************************/ 
/* Function Read X-Y-Position ADC Touch Screen-12 bit (ADS7846)  */
/* Parameter    : None                                           */
/* Return Value : dif_adc_X = Keep Result ADC X-Position(12 bit) */
/*                dif_adc_Y = Keep result ADC Y-Position(12 bit) */                      
/*****************************************************************/
void TCS_SPI_Read_Hor(void)
{
  long tcs_adc_X ,tcs_adc_Y;
  unsigned char buf_data[4];

  if(Get_Status_Touch())
  {
    TCS_CS_LOW();																			// Enable Touch Interface

    buf_data[0] = TCS_SPI_Write(0xD0);														// Write Command Measure X-Position 
	buf_data[0] = TCS_SPI_Write(0x00);														// Read ADC data X-Position (7-bit byte High) data: 0ddddddd	(bit) 
	buf_data[1] = TCS_SPI_Write(0x90);														// Write Command Measure Y-Position ; Read ADC data X-Position (5-bit byte Low)  data:ddddd000(bit)
	buf_data[2] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(7-bit byte High) data: 0ddddddd (bit)
	buf_data[3] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(5-bit byte Low)  data: ddddd000 (bit)

	tcs_adc_X  = buf_data[0];																// Mark ADC Data X-Position 12 bit
	tcs_adc_X  = tcs_adc_X << 5;
	tcs_adc_X |= buf_data[1] >> 3;
	tcs_adc_X  = tcs_adc_X & 0x00000FFF;
	 
	tcs_adc_Y  = buf_data[2];   															// Mark ADC Data Y-Position 12 bit
	tcs_adc_Y  = tcs_adc_Y << 5;   															// Shift 7 bit High
	tcs_adc_Y |= buf_data[3] >> 3;   														// Shift 5 bit low
	tcs_adc_Y  = tcs_adc_Y & 0x00000FFF;													// total ADC data 12 bit
	
	TCS_CS_HIGH();																			// Disable Touch Interface			

    //Result	 	 
    dif_adc_X = 4095-tcs_adc_Y;  															// ADC 12 bit :LCD Start Landscape
	dif_adc_Y = tcs_adc_X;	
  }
}

/************************************************************/
/*              Function Calibrate Touch Green              */
/* Touch at Point + on Screen 3 Point(3 step) for Caribrate */
/* Parameter    : None                                      */
/* Return value : None                                      */
/*              											*/
/************************************************************/
void Touch_Calibrate_Hor(void)
{									
  char n,touch_mark_point=0;

  plot_mark_hor(25,215,32,222,BRIGHT_RED);                                  				// Plot Signe + for Position Calibrate Touch Point1
  lcd_printStr_hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);   				// Plot Text	Color Bright blue
  lcd_printStr_hor("        STEP 1 of 3         ",45,144,BRIGHT_BLUE,BLACK);  
  lcd_printStr_hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        				// Plot Text	Color Black
  lcd_printStr_hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);       

  while(touch_mark_point != 3)	        													// Number Mark Point Calibrate = 3 Point  
  {
    if(Get_Status_Touch())                 													// return (1) if touch screen is detected.
    {	  
	  for(n=0;n<50;n++)
	  {
	    TCS_SPI_Read_Hor();            														// Read portrait(ADC X,Y) by touch value from ADS7846 and update dif_adc_X & dif_adc_Y
		buf_adc_X[n] = dif_adc_X;  															// General buffer for averaging and filter touch screen adc read outs.
	    buf_adc_Y[n] = dif_adc_Y;  															// General buffer for averaging and filter touch screen adc read outs.
	  }
	  TCS_Average_X_Y(50);             														// Take average out of 50 touch measurements, excluding max and min measurement.

	  tcs_ave_X[touch_mark_point] =  ave_adc_X;
	  tcs_ave_Y[touch_mark_point] =  ave_adc_Y;
				 
      touch_mark_point++;            														// touch_cal_status : 0=xy1 cal, 1=xy2 cal, 2=xy3 cal, 3=calibration success. 0xff=calibration not performed.	

	  while(Get_Status_Touch());        													// Waite until not Touch screen 
      switch(touch_mark_point)																// Select Caribrate Next Step 2 and 3 
	  {
	    case 1: bg_color(BLACK);                                                        	// Clear Screen
				plot_mark_hor(280,120,287,127,BRIGHT_RED);                                  // Plot Signe + for Position Calibrate Touch Point2

                lcd_printStr_hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);  // Plot Text Color Bright blue
                lcd_printStr_hor("        STEP 2 of 3         ",45,144,BRIGHT_BLUE,BLACK);  
                lcd_printStr_hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        // Plot Text Color white
                lcd_printStr_hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);         
		        break;

		case 2:	bg_color(BLACK);                                                         	//Clear Screen
		        plot_mark_hor(153,24,160,31,BRIGHT_RED);                                    //Plot Signe + for Position Calibrate Touch Point3
						
				lcd_printStr_hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);  //Plot Text	Color Bright blue
                lcd_printStr_hor("        STEP 3 of 3         ",45,144,BRIGHT_BLUE,BLACK);   
                lcd_printStr_hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        //Plot Text	Color white
                lcd_printStr_hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);          
				break;

		case 3: bg_color(BLACK);                                                         	//Clear Screen

				lcd_printStr_hor("<Touch Screen Calibratetion>",45,159,BRIGHT_BLUE,BLACK);  //Plot Text	Color Bright blue
                lcd_printStr_hor("          Succeded!         ",45,119,BRIGHT_GREEN,BLACK);  
                lcd_printStr_hor("   -Tap Screen for exit-    ",45,79,WHITE,BLACK);          
                       
				while(!Get_Status_Touch());
				delay_ms(100);
				bg_color(BLACK);                                                         	// Clear Screen
	            break;
	  }		 
	}
    delay_ms(100);	
  }															 
  TCS_Set_Matrix_Hor(); 																	// Calculate Coefficient for use find Position Address real on Display
	
  //You can Keep Value data in valiabie devider,An,Bn,Cn,Dn,En,Fn to E2Prom  so that not 
  //come back	 Caribrate.

  ////printf("\n\r divider = %d",divider) ;
  //printf("\n\r      An = %d",An) ;
  //printf("\n\r      Bn = %d",Bn) ;
  //printf("\n\r      Cn = %d",Cn) ;
  //printf("\n\r      Dn = %d",Dn) ;
  //printf("\n\r      En = %d",En) ;
  //printf("\n\r      Fn = %d",Fn) ;


}

/************************************************************************************/
/* Function Calculate average of measurements,by cut value max and min measurement. */
/************************************************************************************/
/* Parameter    : num = number times of measurement adc from touch screen           */
/* Return value : ave_adc_X = Keep result average adc X-Position			        */
/*                ave_adc_Y = Keep result average adc Y-Position                    */     
/************************************************************************************/
void TCS_Average_X_Y(unsigned char num)				
{
  unsigned char i;
  unsigned long sum_x, sum_y;
  unsigned long min_x, min_y, max_x, max_y;

  sum_x=0;sum_y=0;min_x=0xffff;min_y=0xffff,max_x=0;max_y=0;
  for(i=0;i<num;i++)
  {
    sum_x += buf_adc_X[i];
	sum_y += buf_adc_Y[i];

	if(buf_adc_X[i]< min_x)		                        									// search for minimum X value
	{
	  min_x = buf_adc_X[i];
    }
	else if (buf_adc_X[i]> max_x)	                    									// search for maximum X value
	{
	  max_x = buf_adc_X[i];
	}

    if(buf_adc_Y[i]< min_y)	  	                        									// search for minimum Y value
	{
	  min_y = buf_adc_Y[i];
	}
	else if(buf_adc_Y[i]> max_y)	  	                									// search for maximum Y value
	{
	  max_y = buf_adc_Y[i];
	}
  }
  ave_adc_X = (sum_x - min_x - max_x ) / (num-2); 											// update x average value
  ave_adc_Y = (sum_y - min_y - max_y ) / (num-2);	    									// update y average value
}

/************************************************************/
/*     Function Calculate Matrix Find value Coeffcient      */
/************************************************************/
/* Parameter    : None                                      */
/* Return Value : Keep value Coeffcient for use calculate   */                           
/*                X-Y-Position in variable divider,An,Bn,Cn */                   
/*                Dn,En,Fn                                  */
/************************************************************/
void TCS_Set_Matrix_Hor()
{
  divider = ((tcs_ave_X[0]-tcs_ave_X[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
	        ((tcs_ave_X[1]-tcs_ave_X[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

       An = ((dis_XD_hor[0]-dis_XD_hor[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
			((dis_XD_hor[1]-dis_XD_hor[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	   Bn = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_XD_hor[1]-dis_XD_hor[2]))-
				((dis_XD_hor[0]-dis_XD_hor[2])*(tcs_ave_X[1]-tcs_ave_X[2]));
			
	   Cn = (tcs_ave_X[2]*dis_XD_hor[1]-tcs_ave_X[1]*dis_XD_hor[2])*tcs_ave_Y[0]+
		    (tcs_ave_X[0]*dis_XD_hor[2]-tcs_ave_X[2]*dis_XD_hor[0])*tcs_ave_Y[1]+
			(tcs_ave_X[1]*dis_XD_hor[0]-tcs_ave_X[0]*dis_XD_hor[1])*tcs_ave_Y[2];
	    
	   Dn = ((dis_YD_hor[0]-dis_YD_hor[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
			((dis_YD_hor[1]-dis_YD_hor[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	   En = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_YD_hor[1]-dis_YD_hor[2]))-
			((dis_YD_hor[0]-dis_YD_hor[2])*(tcs_ave_X[1]-tcs_ave_X[2]));

	   Fn = (tcs_ave_X[2]*dis_YD_hor[1]-tcs_ave_X[1]*dis_YD_hor[2])*tcs_ave_Y[0]+
		    (tcs_ave_X[0]*dis_YD_hor[2]-tcs_ave_X[2]*dis_YD_hor[0])*tcs_ave_Y[1]+
			(tcs_ave_X[1]*dis_YD_hor[0]-tcs_ave_X[0]*dis_YD_hor[1])*tcs_ave_Y[2];
}

/*******************************************************************/
/*          Function Get Position Address real of Display          */
/*******************************************************************/
/* Parameter    : num = Number times for Measure adc from Touched  */ 
/* Return Value : X = Keep value address X-Position		           */
/*                Y = Keep value address Y-Position                */ 
/*******************************************************************/

void TCS_Get_Point(char num)
{
  char nm; 
	
  nm=0;
  while(Get_Status_Touch() && nm<num)	  													// Measure touch x,y 10 times if the Touch kept pressed,
  {
    TCS_SPI_Read_Hor();                     												// Read value ADC Touch X-Y 
	buf_adc_X[nm]=	dif_adc_X; 																// keep value ADC Touch-X
	buf_adc_Y[nm]=	dif_adc_Y; 																// keep value ADC Touch-Y

 	nm++; 
  }

  if(nm==num)	                      														// if the touch xy successfuly collected,
  {	
    TCS_Average_X_Y(num); 																	// take average out of 10 touch measurements, excluding max and min measurement.
			    
    dif_adc_X = ave_adc_X; 																	// keep average ADC Touch-X
	dif_adc_Y = ave_adc_Y; 																	// keep average ADC Touch-Y
               
	X = ((An*dif_adc_X)+(Bn*dif_adc_Y)+Cn)/divider; 
	Y = ((Dn*dif_adc_X)+(En*dif_adc_Y)+Fn)/divider;
  }
}

/******************************************************/
/*   Function Set  Background color or Clear Screen   */
/******************************************************/
/* Parameter : bg_color =  BackGround color of Screen */
/******************************************************/
void bg_color(long bg_color)
{	
  long cnt;
 
  GLCD_Write_Command(0x20);    																// Command Horizontal GRAM Address Set
  GLCD_Write_Data(0x0000);	   																// Address Start 0x0000 
  GLCD_Write_Command(0x21);    																// Command Vertical GRAM Address Set
  GLCD_Write_Data(0x0000);	   																// Address Start 0x0000  
    
  GLCD_Write_Command(0x22);
  for(cnt = 0;cnt < (320 * 240);cnt++)
  {
    GLCD_Write_Data(bg_color);    															// Back-Ground Color Black
  } 
}

/****************************************************/
/*               Function Plot Signe+               */
/****************************************************/
/* Paramiter : x1,y1 = Position start Line '-'(Hor) */
/*                     plot from Left to Right      */
/*             x2,y2 = Position start Line '|'(Ver) */
/*                     plot from botton to top      */
/****************************************************/
void plot_mark_hor(long x1,long y1,long x2,long y2,long color)
{
  char i ;

  // Plot Hor Line
  GLCD_Write_Command(0x20);  																// Command Set Adddress Hor(X)
  GLCD_Write_Data(y1);  																	// Sent X_address  CGRAM
	 		
  GLCD_Write_Command(0x21);  																// Command Set Address Ver(Y)
  GLCD_Write_Data(x1);  																	// Sent Y_address  CGRAM
	 
  GLCD_Write_Command(0x22);  																// Command Write Data RGB   
  for(i=0;i<15;i++) 
  GLCD_Write_Data(color);

  // Plot Ver Line
  GLCD_Write_Command(0x03);   																// Command Configure Entry Mode
  GLCD_Write_Data(0x1220); 	 																// Color:RGB ,Incremen Ver & decrement Hor. address,Address update Hor.

  GLCD_Write_Command(0x20);   																// Command Set Adddress Hor(X)
  GLCD_Write_Data(y2);   																	// Sent X_Address CGRAM 
	 		
  GLCD_Write_Command(0x21);   																// Command Set Address Ver(Y)
  GLCD_Write_Data(x2);	 																	// Sent Y_Address CGRAM 
	 
  GLCD_Write_Command(0x22);   																// Command Write Data RGB   
  for(i=0;i<15;i++) 
  GLCD_Write_Data(color);
 
  GLCD_Write_Command(0x03);   																// Command Configure Entry Mode
  GLCD_Write_Data(0x1228); 	 																// Color:RGB ,Incremen Ver.& Decrement Hor. address,Address update Ver. 	
}
 
/*****************************************************/
/*              Function Plot Dot 1 Pixel            */
/*****************************************************/
/* Parameter : Xadd,Yadd = Position X,Y for Plot Dot */
/*             color = Color of dot                  */
/*****************************************************/
void plot_dot_hor(unsigned long Xadd,unsigned long Yadd,unsigned long color)
{	
  GLCD_Write_Command(0x20);  																// Command Set Adddress Hor(X)
  GLCD_Write_Data(Yadd);  																	// Sent X_address for CGRAM 		
  GLCD_Write_Command(0x21);  																// Command Set Address Ver(Y)
  GLCD_Write_Data(Xadd);  																	// Sent Y_address for CGRAM 	  			
  GLCD_Write_Command(0x22);  																// Command Write Data RGB
  GLCD_Write_Data(color);  																	// write data
}

/****************************************************************************/
/**                Function Print Text 1 Charecter size 7x11                */
/****************************************************************************/
/* Parameter : row      = Ascii Code (Position buffer keep text)		    */
/*             adx,ady  = Position X,Y for begin plot text by will 			*/
/*                        begin plot from bottom left to top left   		*/
/*             fg_clr   = Color of text										*/
/*             bg_clr   = Color background of text(if bg_clr = no_bg or 1=	*/
/*                        non color background)								*/
/****************************************************************************/
void text_7x11_hor(char row,long adx,long ady,long fg_clr,long bg_clr)
{
  long ax,ay;
  unsigned char m,n,tx;
     
  ax = adx;
  ay = ady; 

  row = row-0x20;

  // Print Text 1 Charecter(data 14 Byte) 
  for(m=0;m<14;m++)
  {
    // Sent data byte1=8 bit
    tx = ascii_7x11[row][m];  																//Read data ASCII
	
	for(n=0;n<8;n++)		       															//Loop Sent data  1 byte(8bit)
	{
	  if(tx & 0x80)				   															//if data bit7 = 1 ,Plot Color area Charecter
	  {              
	    GLCD_Write_Command(0x20);  															//Command Set Adddress Hor(X)
        GLCD_Write_Data(ay);  																//Sent X_Address CGRAM	 		
        GLCD_Write_Command(0x21);  															//Command Set Address Ver(Y)
        GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
		GLCD_Write_Command(0x22);  															//Command Write data 
        GLCD_Write_Data(fg_clr);
	   }
	   else						   															//if data bit7 = 0 ,Plot Color area back ground Charecter
	   {
	     if(bg_clr != 1)     
         {
		    GLCD_Write_Command(0x20);  														//Command Set Adddress Hor(X)
            GLCD_Write_Data(ay);  															//Sent X_Address CGRAM	
            GLCD_Write_Command(0x21);  														//Command Set Adddress Ver(Y)
            GLCD_Write_Data(ax);  															//Sent Y_Address CGRAM
			GLCD_Write_Command(0x22);  														//Command Write data
            GLCD_Write_Data(bg_clr);  														//Sent Data
		  }
		}

		tx <<= 1;  																			// Shift Right data 1 bit
		ay   = ay+1;  																		// Increment Y-address
	} 
	m++;  																					//Increment Next pointter byte Data 


	// Sent data byte2=3 bit 
	tx = ascii_7x11[row][m];  																//Read data byte2
	for(n=0;n<3;n++)			   															//Loop sent data byte2 = 3 bit
	{						     
	  if(tx & 0x80)				   															//if data bit7 = 1 ,Plot Color area Charecter
	  {              
	    GLCD_Write_Command(0x20);  															//Command Set Adddress Hor(X)
        GLCD_Write_Data(ay);  																//Sent X_Address CGRAM		
        GLCD_Write_Command(0x21);  															//Command Set Adddress Ver(Y)
        GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
		GLCD_Write_Command(0x22);  															//Command Write data
        GLCD_Write_Data(fg_clr);
      }
	  else						   															//if data bit7 = 0 ,Plot Color area back ground Charecter
	  {
	    if(bg_clr != 1)     
        {
		  GLCD_Write_Command(0x20);  														//Command Set Adddress Hor(X)
          GLCD_Write_Data(ay);  															//Sent X_Address CGRAM	 		
          GLCD_Write_Command(0x21);  														//Command Set Adddress Ver(Y)
          GLCD_Write_Data(ax);  															//Sent Y_Address CGRAM
		  GLCD_Write_Command(0x22);  														//Command Write data
          GLCD_Write_Data(bg_clr);
		}
	  }

	  tx <<= 1;  																			//Shift Right data 1 bit
	  ay = ay+1;  																			//Increment Y-address
	} 

	ax = ax+1; 																				//Complet sent data 2 byte(11bit) Increment X-Address
	ay = ady; 																				//Set Position Y-address old
  }	

  // Fill Back ground Color Position space between Charecter 1 Colum 
  if(bg_clr != 1)     
  {
    for(n=0;n<11;n++)
	{
	  GLCD_Write_Command(0x20);  															//Command Set Adddress Hor(X)
      GLCD_Write_Data(ay);  																//Sent X_Address CGRAM	 		
      GLCD_Write_Command(0x21);  															//Command Set Adddress Ver(Y)
      GLCD_Write_Data(ax);  																//Sent Y_Address CGRAM
	  GLCD_Write_Command(0x22);  															//Command Write data
      GLCD_Write_Data(bg_clr);
	  ay = ay+1;  																			//Increment Y-Address
    }
  }
}

/**********************************************************************/
/*                         Function Print String                      */
/**********************************************************************/
/* Parameter : *str = Charecter ASCII (String)					      */
/*             cur_x,cur_y = Position X,Y for begin plot text by will */
/*                           begin plot from bottom left to top left  */
/*             fg_color = color of Text								  */
/*             bg_color = color Background of text                    */											
/**********************************************************************/
void lcd_printStr_hor(char *str,long cur_x,long cur_y,long fg_color,long bg_color)
{
  unsigned char i;

  for (i=0; str[i] != '\0'; i++)
  {					 
    text_7x11_hor(str[i],cur_x,cur_y,fg_color,bg_color);
	cur_x += 8; 	
  }
}
 
/***********************************/
/* Function Print Text 3 Charecter */
/***********************************/
void lcd_print3Cha_hor(char ch1,char ch2,char ch3,long cur_x,long cur_y,long fg_color,long bg_color)
{	  			 
  text_7x11_hor(ch1,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8; 
  text_7x11_hor(ch2,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8; 
  text_7x11_hor(ch3,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8;  	
}

/*************************************/
/*         Function Plot TAB         */
/*************************************/
/* Parameter : color = color for tab */
/*************************************/	     
void plot_tab_hor(unsigned long color)
{
  char rx,ry;
	  
  for(ry=0;ry<26;ry++)
  { 
    GLCD_Write_Command(0x20);  																//Command Set Adddress Hor(X)
    GLCD_Write_Data(226-ry);  																//Sent X_address for CGRAM 
	GLCD_Write_Command(0x21);  																//Command Set Address Ver(Y)
    GLCD_Write_Data(309);  																	//Sent Y_address for CGRAM 			
    GLCD_Write_Command(0x22);  																//Command Write Data RGB
	
	for(rx=0;rx<6;rx++) 
    GLCD_Write_Data(color);  																//write data
  }
}

/********************************************************/
/*               Function Draw Line 		     		*/
/********************************************************/
/* Function : Line(x_1,y_1,x_2,y_2,line_type);	        */
/*												     	*/
/* Input  : x_1,y_1    = Start Coordinate Line			*/
/*          x_2,y_2    = End Coordinate Line			*/
/*          line_type  = Value Thick of Line (1-100)    */     
/*	        color      = Color of line			        */
/********************************************************/
void Line(long x_1,long y_1,long x_2,long y_2,char line_type,long color)
{
  char tick;
  long x1=x_1,xx1=x_1;
  long y1=y_1;
  long x2=x_2,xx2=x_2;
  long y2=y_2;
  long dx,dy,inc_dec,two_dx,two_dy,two_dy_dx,two_dx_dy,x,y,p;

  if(x_1>x_2)
  {
    x1=x_2;
	y1=y_2;

	x2=x_1;
	y2=y_1;
  }

  dx  = fabs(x2-x1);
  dy  = fabs(y2-y1);

  if(y2>=y1)
  inc_dec = 1;
  else
  inc_dec = -1;

  if(dx>dy)
  {
    two_dy=(2*dy);
	two_dy_dx=(2*(dy-dx));
	p=((2*dy)-dx);

	x=x1;
	y=y1;

	while(x<=x2)
	{
	  for(tick=0;tick<line_type;tick++)
	  {	   
	    if(xx1==xx2)
		plot_dot_hor(x+tick,y,color);
		else
		plot_dot_hor(x,y+tick,color);	  
	  }

	  x++;
	  if(p<0)
	  p += two_dy;
	  else
	  {
	    y += inc_dec;
		p += two_dy_dx;
	  }
	}  
  }	 

  else
  {
    two_dx=(2*dx);
	two_dx_dy=(2*(dx-dy));
	p=((2*dx)-dy);

	x=x1;
	y=y1;

	while(y!=y2)
	{
	  for(tick=0;tick<line_type;tick++)
	  {		     
	    if(xx1==xx2)					    
		plot_dot_hor(x+tick,y,color);
		else
		plot_dot_hor(x,y+tick,color);	  
	  }
	  y+=inc_dec;

	  if(p<0)
	  p+=two_dx;

	  else
	  {
	    x++;
		p += two_dx_dy;
	  }
	}  
  }	
}	    


/*******************************************************************************/
/*                         Function Draw Circle 					           */
/*******************************************************************************/   
/* Function : Circle(CX,CY,Radius,tick,color,fill)		                       */
/* Input    : CX,CY       = Center Point of Circle							   */
/*            Radius      = radius of Circle					               */
/*            tick(1-100) = bold of line (have effect when fill=0)	           */
/*            color       = color of Circle						   	           */
/*            fill        = 0: No fill color in Circle,1:Fill color in Circle  */
/*******************************************************************************/
void Circle(long CX,long CY,char Radius,char tick,long color,char fill)
{
  long tswitch=0;
  char d,x=0,y=0,bd;

  d = CY-CX;
  y = Radius;
  tswitch = 3-(2*Radius);

  while(x<=y)	
  {
    if(fill==1)
	{
	  Line(CX-x,CY+y,CX+x,CY+y,1,color);
	  Line(CX-x,CY-y,CX+x,CY-y,1,color);
	  Line(CX-y,CY+x,CX+y,CY+x,1,color);
	  Line(CX-y,CY-x,CX+y,CY-x,1,color);
	}
	else
	{
	  for(bd=0;bd<tick;bd++)
	  {
	    plot_dot_hor((CX+x)+bd,(CY+y)+bd,color);
		plot_dot_hor((CX+x)+bd,(CY-y)+bd,color);

		plot_dot_hor((CX-x)+bd,(CY+y)+bd,color);
		plot_dot_hor((CX-x)+bd,(CY-y)+bd,color);

		plot_dot_hor((CY+y-d)+bd,(CY+x)+bd,color);
		plot_dot_hor((CY+y-d)+bd,(CY-x)+bd,color);

		plot_dot_hor((CY-y-d)+bd,(CY+x)+bd,color);
		plot_dot_hor((CY-y-d)+bd,(CY-x)+bd,color);
	  }
	}

	if(tswitch<0)
	tswitch += (4*x)+6;
	else
	{
      tswitch += 4*(x-y)+10;
	  y--;
	}
    x++;
  }              
}
  
/********************************************************************************/
/*                           Function Draw Ellipse 							    */
/********************************************************************************/   
/* Function : Ellipse(h,k,rx,ry,tick,color,fill)								*/
/* Input    : h,k         = Center Point of Ellipes  				            */
/*            rx,ry       = radius of Ellipse in X,Y axis			            */
/*            tick(1-100) = bold of line (have effect when fill=0)	            */
/*            color       = color of Ellipse						   	        */
/*            fill        = 0: No fill color in Ellipes,1:Fill color in Ellipes */
/********************************************************************************/ 
void Ellipse(unsigned int h,unsigned int k,long rx,long ry,char tick,long color,char fill)
{
  char bd;
  long   ax  =(rx*rx);
  long   by  =(ry*ry);
  float  ax2 =(ax*2);
  float  by2 =(by*2);

  long   x   = 0;
  long   y   = ry;

  float  fx  = 0;
  float  fy  = (ax2*ry);

  float     p=(int)(by-(ax*ry)+(0.25*ax)+0.5);


  if(fill==1)
  {
    Line(h-x,k+y,h+x,k+y,1,color);
	Line(h-x,k-y,h+x,k-y,1,color);
  }
  else
  {
    for(bd=0;bd<tick;bd++)
	{
      plot_dot_hor((h+x)+bd,(k+y)+bd,color);
	  plot_dot_hor((h+x)+bd,(k-y)+bd,color);
	  plot_dot_hor((h-x)+bd,(k-y)+bd,color);
	  plot_dot_hor((h-x)+bd,(k+y)+bd,color);
	}
  }
  			   
  while(fx<fy)
  {
    x++;
	fx+=by2;

	if(p<0)
	p+=(fx+by);

	else
	{
	  y--;
	  fy-=ax2;
	  p+=(fx+by-fy);
	}

	if(fill==1)
	{
	  Line(h-x,k+y,h+x,k+y,1,color);
	  Line(h-x,k-y,h+x,k-y,1,color);
	}
	else
	{
	  for(bd=0;bd<tick;bd++)
	  {		  
	    plot_dot_hor((h+x)+bd,(k+y)+bd,color);
		plot_dot_hor((h+x)+bd,(k-y)+bd,color);
		plot_dot_hor((h-x)+bd,(k-y)+bd,color);
		plot_dot_hor((h-x)+bd,(k+y)+bd,color);
	  }
	}
  }

  p=(int)((by*(x+0.5)*(x+0.5))+(ax*(y-1)*(y-1))-(ax*by)+0.5);

  while(y>0)
  {
    y--;
	fy-=ax2;

	if(p>=0)
	p+=(ax-fy);

	else
	{
	  x++;
	  fx+=by2;
	  p+=(fx+ax-fy);
	}

	if(fill==1)
	{
	  Line(h-x,k+y,h+x,k+y,1,color);
	  Line(h-x,k-y,h+x,k-y,1,color);
	}
	else
	{
	  for(bd=0;bd<tick;bd++)
	  {
	    plot_dot_hor((h+x)+bd,(k+y)+bd,color);
		plot_dot_hor((h+x)+bd,(k-y)+bd,color);
		plot_dot_hor((h-x)+bd,(k-y)+bd,color);
		plot_dot_hor((h-x)+bd,(k+y)+bd,color);
	  }
	}   
  }	 
}
 

/************************************************************************************/
/*                          Function Draw a rectangle 			                    */
/************************************************************************************/
/* Function : Rectan(x1,y1,x2,y2,tick,color,fill)									*/	
/* Inputs   : (x1, y1)    = Start Point coordinate Rectan							*/
/*            (x2, y2)    = End Point coordinate  Rectan							*/
/*            tick(1-100) = bold of line(have effect when fill=0					*/
/*            color       =  color of rectangle									    */
/*            fill        = 0: No fill color in rectangle,1:Fill color in rectangle */
/*       																		    */
/*            (x1,y1) -------------												    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                    -------------	(x2,y2)										    */
/*																					*/
/************************************************************************************/																						  
void Rectan(long x1,long y1,long x2,long y2,char tick,long color,char fill)
{
  long xmin,xmax,ymin,ymax,i;
  char tk = tick-1;

  if(fill==1)
  {
    if(x1 < x2)                            													//  Find x min and max
    {
      xmin = x1;
      xmax = x2;
	}
	else
	{
	  xmin = x2;
	  xmax = x1;
	}			
	if(y1 < y2)                            													// Find the y min and max
	{
	  ymin = y1;
	  ymax = y2;
	}
	else
	{
	  ymin = y2;
	  ymax = y1;
	}
			
	for(; xmin<=xmax;xmin++)
	{
	  for(i=ymin; i<=ymax; i++)
	  plot_dot_hor(xmin,i,color);
	}
  }
  else	
  {
    Line(x1,y1-tk,x2,y1-tk,tick,color);  													//Line Ho.r Top
	Line(x1,y2,x2,y2,tick,color);  															//Line Hor. Botton
	Line(x1,y1,x1,y2,tick,color);  															//Line Ver. Left
	Line(x2-tk,y1,x2-tk,y2,tick,color);  													//Line Ver. Right
  }      		 
}

/*************************************************************************/
/*                    Function build Button (Draw-rectang)               */
/*    Start Plot button from  botton left to botton Right of button      */  
/*************************************************************************/
/*																	     */
/* Inputs : px,py      = Position X,Y  begin build  button               */
/*          width,high = width and high of button(unit Pixel)            */
/*                       for width not exceed 50 pixel will good         */
/*          c_bt       = Color of button                                 */
/*          *tx        = ASCII Charecter on Button                       */
/*                       1 Charecter width = 9pixel. Number pixel of	 */
/*                       charecter total must less pixel width of button */  
/*                     												     */
/*          num_tx     = Number Charecter at display on button           */
/*          c_tx       = Color charecter on button                       */
/*       																 */
/*          |<--- width --->|								    	     */
/*           -------------->   ---									     */
/*          |           end |	 ^									     */
/*          |               |	 |									     */
/*          |               |	high								     */
/*          |start          |	 |									     */
/*   (px,py) -------------->	---									     */
/*																	     */
/*************************************************************************/
/*void button_build_hor(long px,long py,long width,long high,long c_bt,unsigned char *tx,char num_tx,long c_tx)
{
  long x1,y1,x2,y2,ay,tpx;
  char tab,n;
  long cbt1,cbt2,sub;

  switch(c_bt)
  {
    case BT_RED:      c_bt = 0xF800; 														//Color 
				      cbt1 = c_bt; 															//color start Max
				      cbt2 = 0x9800; 														//color last  Min (cbt1-(sub*12))
				      sub  = 0x0800; 														//decrement color
		              break;

	case BT_GREEN:    c_bt = 0x07C0; 														//Color 
				      cbt1 = c_bt; 															//color start Max
				      cbt2 = 0x04D0; 														//color last  Min (cbt1-(sub*12))
				      sub  = 0x0040; 														//decrement color		         
			          break;

	case BT_BLUE:     c_bt = 0x001F; 														//Color 
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x0014; 														//color last  Min (cbt1-(sub*12))
					  sub  = 0x0001; 														//decrement color
		              break;

    case BT_YELLOW:   c_bt = 0xFFC0; 														//Color 
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x9CC0; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0840; 														//decrement color
		              break;

	case BT_WHITE:    c_bt = 0xFFFF; 														//Color 
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x9CF3; 														//color last  Min (cbt1-(sub*12))
					  sub  = 0x0841; 														//decrement color
		              break;

	case BT_CYAN:     c_bt = 0x87FF; 														//Color 
				      cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x24F3; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0841; 														//decrement color
		              break;


	case BT_MAGENTA:  c_bt = 0xFC1F; 														//Color 
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x9913; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0841; 														//decrement color
		              break;

	case BT_BROWN:    c_bt = 0xEC00; 														//Color 
				      cbt1 = c_bt; 															//color start Max
				      cbt2 = 0x8900; 														//color last  Min(cbt1-(sub*12))
				      sub  = 0x0840; 														//decrement color
		              break;
	
	case BT_LRED:     c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0xFCF3; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0041; 														//decrement color
		              break;	
				 
	case BT_LGREEN:   c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0x9FF3; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0801; 														//decrement color
		              break;	
				 
	case BT_LBLUE:    c_bt = 0xFFFF;
				      cbt1 = c_bt; 															//color start Max
				      cbt2 = 0x9CFF; 														//color last  Min(cbt1-(sub*12))
				      sub  = 0x0840; 														//decrement color
		              break;

	  
	case BT_LYELLOW:  c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0xFFC3; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0001; 														//decrement color
		              break;
         
	case BT_LWHITE:   c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0xFFDE; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0021; 														//decrement color
		              break;


	case BT_LCYAN:    c_bt = 0xFFFF;
				      cbt1 = c_bt; 															//color start Max
				      cbt2 = 0x9FFF; 														//color last  Min(cbt1-(sub*12))
				      sub  = 0x0800; 														//decrement color
		              break;
						
	case BT_LMAGENTA: c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0xFCFF; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0040; 														//decrement color
		              break;	
				 
	case BT_LBROWN:   c_bt = 0xFFFF;
					  cbt1 = c_bt; 															//color start Max
					  cbt2 = 0xFE73; 														//color last  Min(cbt1-(sub*12))
					  sub  = 0x0021; 														//decrement color
		              break;												
  }

  x1   = px;    
  y1   = py;
  x2   = px+width; 
  y2   = py+high-1;
  
  high  = high/2;
  ay    = py+high;
  tab   = high/12;
	
  if(tab==0)
     tab=1;
  else
  {
    if((high%12)>3)
	tab = tab+1;
  }

  // Start Plot halft button from botton to top 

  while(y1<=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line(x1,y1,x2,y1,1,cbt1);
      y1++;
	   
	  if(y1 >= ay)
	  n=n+20;
    }
    cbt1 = cbt1-sub;
     
	if(cbt1<cbt2)
    cbt1=cbt2;
  }

  // Plot halft button from top to botton
 
  cbt1 = c_bt; 

  while(y2>=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line(x1,y2,x2,y2,1,cbt1);
      y2--;
	 
	  if(y2 <= ay)
	    n=n+20;
    }
    cbt1 = cbt1-sub;
     
	if(cbt1<cbt2)
    cbt1=cbt2;
  }

  // write Text 
  tpx = (width-(num_tx*8))/2;
  tpx = tpx+px;
		
  py = (py+high)-6;
	

  for (n=0; tx[n] != '\0'; n++)
  {					 
    text_7x11_hor(tx[n],tpx,py,c_tx,no_bg);
	tpx += 9; 		
  }
}
*/
/*********************************************************************/
/*         Function Plot Picture(BMP) to Display                     */
/*  Start Plot data of picture from  top left to top Right           */
/*********************************************************************/
/*  Inputs : *bmp       = Name reg. array keep data of picture       */
/*           xad,yad    = Position X,Y for begin plot data picture   */
/*           width,high = Size Picture unit Pixel                    */
/*********************************************************************/
void plot_picture_hor(const char *bmp,long xad,long yad,long width,long high)        
{
  unsigned long pt=0,picture;
  unsigned long hi,wid;
  long xad1,yad1;

  xad1 = xad;
  yad1 = yad;

  for(hi=0;hi<high;hi++)		        													// Number X-Pixel(High) of Picture
  {      		
    GLCD_Write_Command(0x20);  																// Command Set Address Hor(X)
    GLCD_Write_Data(yad1);  																// Sent X_Address CGRAM		 		  

	for(wid=0;wid<width;wid++)	        													// number X-pixel(Width) of Picture  
	{
	  GLCD_Write_Command(0x21);  															// Command Set Adddress Ver(Y)
      GLCD_Write_Data(xad1);  																// Sent Y_Address CGRAM	
		   				 
	  GLCD_Write_Command(0x22);  															// Command Write data 	
			 	  
	  picture = bmp[pt+1];  																// Read data picture byte High
	  picture =	picture <<8;
			  
	  picture|= bmp[pt];  																	// read data picture Byte low

      GLCD_Write_Data(picture);
	  pt = pt+2;  																			// Increment pointer of data picture 2 byte
			 
	  xad1++;			  
	}

	yad1--;
	xad1 = xad;
  }
}

