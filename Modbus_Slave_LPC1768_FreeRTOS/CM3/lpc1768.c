/*
===============================================================================
 Name        : lpc1768.c
 Author      : Enrico Giordano
 Version     : 1.0
 Copyright   : Copyright (C) 2013
 Description : funzioni elementari di lpc1768
===============================================================================
*/

#include "lpc1768.h"


void Delay(int delay)
{
	while(delay >= 0)
		delay--;
}


void resetGPIO(void)
{
	LPC_PINCON->PINSEL4 &= ~(0xFFFF); 	// Reset P2[0..7] = GPIO
	LPC_PINCON->PINSEL3 &= ~(3UL<<30); 	// Reset P1.31 = GPIO
	LPC_PINCON->PINSEL4 &= ~(0x03<<20);	// Reset P2.10 = GPIO
	LPC_PINCON->PINSEL3 &= ~(0x03<<18);
	LPC_PINCON->PINSEL3 &= ~(0x03<<20);
	LPC_PINCON->PINSEL3 &= ~(0x03<<24);
	LPC_PINCON->PINSEL3 &= ~(0x03<<26);
	LPC_PINCON->PINSEL7 &= ~(3 << 20);



}


void setOutput_LEDint(void)
{
	// Configurazione Pin GPIO = P2[0..7] Drive LED
		LPC_GPIO2->FIODIR |= 0xFF;			// P2[0..7] = Outputs
}

void setButton_reset(void)
{
	LPC_GPIO2->FIODIR &= ~(1UL<<10);	
}



void setOutput_ADC_Rint(void)
{
	// Configurazione convertitore A/D usando resistenza variabile integrata nel circuito
		LPC_PINCON->PINSEL3 |= (3UL << 30);		//configura pin1.31 per ricevere corrente
		LPC_SC->PCONP |= (1UL << 12); 			//abilita corrente sul convertitore AD
		LPC_ADC->ADCR = (1UL << 5)			//seleziona AD 0.5 per convertitore (dove c'è la resistenza variabile)
					  | (1UL << 8)		//conversione a 18 MHz / 2
					  | (1UL << 21);	//abilita ADC

}


void setOutput_ADC_Temp(void)
{
//		LPC_PINCON->PINSEL4 |= (3 << 0);
		LPC_PINCON->PINSEL3 |= (3UL << 30);		//configura pin1.31 per ricevere corrente
		LPC_SC->PCONP |= (1UL << 12); 			//abilita corrente sul convertitore AD
		LPC_ADC->ADCR = (1UL << 5)			//seleziona AD 0.5 per convertitore (dove c'è la resistenza variabile)
					  | (1UL << 8)		//conversione a 18 MHz / 2
					  | (1UL << 21);	//abilita ADC

}


void P123ADCInit(){

	LPC_SC->PCONP       |=  (1<<12);         /* Enable power to ADC block */

	  LPC_PINCON->PINSEL1 &= ~(3UL<<14);       /* P1.23 is GPIO */
	  LPC_PINCON->PINSEL1 |=  (1UL<<14);       /* P1.23 is AD0.0 */

	  LPC_ADC->ADCR        =  (1UL << 0) |    /* select AD0.0 */
	                          (1 << 8) |  /* ADC clock */
	                          (1UL << 16) |      /* ADC is in the burst mode */
	                          (1UL << 21);       /* enable ADC */

}


void setBeep(void)
{
	LPC_PINCON->PINSEL7 &= ~(3 << 20);
	LPC_GPIO3->FIODIR |= (1UL<<26);
	LPC_GPIO3->FIOPIN ^= (1 << 26);

}

void stop_beep(void)
{
	if(!LPC_GPIO3->FIOPIN)
		LPC_GPIO3->FIOPIN ^= (1 << 26);
}

void setJoySwitch(void)
{
	LPC_GPIO1->FIODIR &= ~((1UL<<25)|(1UL<<26)|(1UL<<28)|(1UL<<29));	//P1.25,26,28,29=In

}

void turn_off_the_LEDS(void)
{
	LPC_GPIO2->FIOCLR =	0xFF;			// spegne tutti i LED
}


void turn_on_single_LED(int led)
{
	LPC_GPIO2->FIOSET = (1<<led);		// ON LED[led]
}


void turn_off_single_LED(int led)
{
	LPC_GPIO2->FIOCLR = (1<<led);		// OFF LED[led]
}


int reset_is_pressed(void)
{
	if ((LPC_GPIO2->FIOPIN >> 10) & 0x01)	//se viene rilasciato
		return 0;
	else					//se viene premuto
		return 1;

}


int joyswitch_up(void)
{
	if ((LPC_GPIO1->FIOPIN >> 25) & 0x01)
		return 0;
	else
		return 1;
}


int joyswitch_down(void)
{
	if ((LPC_GPIO1->FIOPIN >> 26) & 0x01)
		return 0;
	else
		return 1;
}

int joyswitch_right(void)
{
	if ((LPC_GPIO1->FIOPIN >> 28) & 0x01)
		return 0;
	else
		return 1;
}


int joyswitch_left(void)
{
	if ((LPC_GPIO1->FIOPIN >> 29) & 0x01)
		return 0;
	else
		return 1;
}

int joyswitch_pressed(void)
{
	if ((LPC_GPIO3->FIOPIN >> 25) & 0x01)
		return 0;
	else
		return 1;
}


void beep(int durata, int frequenza)
{
	while(durata)
	{
			LPC_GPIO3->FIOPIN ^= (1 << 26);
			Delay(frequenza);
			durata--;
	}
}


int convert_from_ADC_VR(void)
{
	int val;
	LPC_ADC->ADCR |= (1<<24);				// abilita la conversione
	while (!(LPC_ADC->ADGDR & (1UL<<31)));	//finchè non ha finito di convertire attendi
	val = ((LPC_ADC->ADGDR >> 4) & 0xFFF);	//ottieni il valore
	LPC_ADC->ADCR &= ~(7<<24);				//blocca la conversione fino alla prossima chiamata di funzione

	return val;
}

void setInput_P2(void)
{
	// Configurazione Pin GPIO = P2[0..7] Drive LED
		LPC_GPIO2->FIODIR &= 0x00;			// P2[0..7] = Input

}

volatile uint32_t ADCValue[ADC_NUM];

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		None
**
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
  uint32_t i, pclkdiv, pclk;

  /* Enable CLOCK into ADC controller */
  LPC_SC->PCONP |= (1 << 12);

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADCValue[i] = 0x0;
  }

  /* all the related pins are set to ADC inputs, AD0.0~7 */
  LPC_PINCON->PINSEL0 &= ~0x000000F0;	/* P0.2~3, A0.6~7, function 10 */
  LPC_PINCON->PINSEL0 |= 0x000000A0;
  LPC_PINCON->PINSEL1 &= ~0x003FC000;	/* P0.23~26, A0.0~3, function 01 */
  LPC_PINCON->PINSEL1 |= 0x00154000;
  LPC_PINCON->PINSEL3 |= 0xF0000000;	/* P1.30~31, A0.4~5, function 11 */
  /* No pull-up no pull-down (function 10) on these ADC pins. */
  LPC_PINCON->PINMODE0 &= ~0x000000F0;
  LPC_PINCON->PINMODE0 |= 0x000000A0;
  LPC_PINCON->PINMODE1 &= ~0x003FC000;
  LPC_PINCON->PINMODE1 |= 0x002A8000;
  LPC_PINCON->PINMODE3 &= ~0xF0000000;
  LPC_PINCON->PINMODE3 |= 0xA0000000;

  /* By default, the PCLKSELx value is zero, thus, the PCLK for
  all the peripherals is 1/4 of the SystemFrequency. */
  /* Bit 24~25 is for ADC */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 24) & 0x03;
  switch ( pclkdiv )
  {
	case 0x00:
	default:
	  pclk = SystemCoreClock/4;
	break;
	case 0x01:
	  pclk = SystemCoreClock;
	break;
	case 0x02:
	  pclk = SystemCoreClock/2;
	break;
	case 0x03:
	  pclk = SystemCoreClock/8;
	break;
  }

  LPC_ADC->ADCR = ( 0x01 << 0 ) |  /* SEL=1,select channel 0~7 on ADC0 */
		( ( pclk  / ADC_Clk - 1 ) << 8 ) |  /* CLKDIV = Fpclk / ADC_Clk - 1 */
		( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
		( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
		( 1 << 21 ) |  		/* PDN = 1, normal operation */
		( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
		( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */

  /* If POLLING, no need to do the following */
#if ADC_INTERRUPT_FLAG
  NVIC_EnableIRQ(ADC_IRQn);
#if BURST_MODE
  LPC_ADC->ADINTEN = 0xFF;		/* Enable all interrupts */
#else
  LPC_ADC->ADINTEN = 0x1FF;		/* Enable all interrupts */
#endif
#endif
  return;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
**
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{
#if !ADC_INTERRUPT_FLAG
  uint32_t regVal, ADC_Data;
#endif

  /* channel number is 0 through 7 */
  if ( channelNum >= ADC_NUM )
  {
	channelNum = 0;		/* reset channel number to 0 */
  }
  LPC_ADC->ADCR &= 0xFFFFFF00;
  LPC_ADC->ADCR |= (1 << 24) | (1 << channelNum);
				/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
  while ( 1 )			/* wait until end of A/D convert */
  {
      switch (channelNum) {
			case 0:
				regVal = LPC_ADC->ADDR0;
				break;
			case 1:
				regVal = LPC_ADC->ADDR1;
				break;
			case 2:
				regVal = LPC_ADC->ADDR2;
				break;
			case 3:
				regVal = LPC_ADC->ADDR3;
				break;
			case 4:
				regVal = LPC_ADC->ADDR4;
				break;
			case 5:
				regVal = LPC_ADC->ADDR5;
				break;
			case 6:
				regVal = LPC_ADC->ADDR6;
				break;
			case 7:
				regVal = LPC_ADC->ADDR7;
				break;
      }
	//regVal = LPC_ADC->DR[channelNum];
	/* read result of A/D conversion */
	if ( regVal & ADC_DONE )
	{
	  break;
	}
  }

  LPC_ADC->ADCR &= 0xF8FFFFFF;	/* stop ADC now */
  if ( regVal & ADC_OVERRUN )	/* save data when it's not overrun, otherwise, return zero */
  {
	return ( 0 );
  }
  ADC_Data = ( regVal >> 4 ) & 0xFFF;
  return ( ADC_Data );	/* return A/D conversion value */
#else
  return ( channelNum );	/* if it's interrupt driven, the ADC reading is
							done inside the handler. so, return channel number */
#endif
}


