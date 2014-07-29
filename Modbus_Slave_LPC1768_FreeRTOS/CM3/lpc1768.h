/*
===============================================================================
 Name        : lpc1768.h
 Author      : Enrico Giordano
 Version     : 1.0
 Copyright   : Copyright (C) 2013
 Description : funzioni elementari di lpc1768
===============================================================================
*/
#ifndef __LPC1768_H

#define __LPC1768_H

#include "LPC17xx.h"


void Delay(int delay);

/*********************/
/* FUNZIONI DI SETUP */
/*********************/

//bisogna usare questa funzione per resettare la board
void resetGPIO(void);

//per usare la fila di 8 led integrata nella board
void setOutput_LEDint(void);

//per usare il convertitore analogico-digitale il cui segnale deriva dalla resistenza variabile integrata nella board
void setOutput_ADC_Rint(void);

//converte il segnale proveniente dalla resistenza variabile integrata e ne restituisce il valore 
int convert_from_ADC_VR(void);

void setButton_reset(void);

void setJoySwitch(void);

void setBeep(void);

void setInput_P2(void);


/********************/
/*FUNZIONI DI AZIONI*/
/********************/


//azioni sui led
void turn_off_the_LEDS(void);
void turn_on_single_LED(int led);
void turn_off_single_LED(int led);

//bottoni
//come un booleano: se viene premuto il bottore ritorna 1, altrimenti se viene rilasciato ritorna 0
int reset_is_pressed(void);
int joyswitch_left(void);
int joyswitch_right(void);
int joyswitch_up(void);
int joyswitch_down(void);
int joyswitch_pressed(void);

//suoni
void beep(int durata, int frequenza);
void stop_beep(void);


#define ADC_OFFSET          0x10
#define ADC_INDEX           4

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000
#define ADC_ADINT           0x00010000

#define ADC_NUM			8		/* for LPCxxxx */
#define ADC_CLK			1000000		/* set to 1Mhz */


extern void ADCInit( uint32_t ADC_Clk );
extern uint32_t ADCRead( uint8_t channelNum );


#endif
