/*
===============================================================================
 Name        : main.c
 Author      : Enrico Giordano
 Version     :
 Copyright   : Copyright (C)
 Description : main definition
===============================================================================
*/

/*----------------------------- Includes ------------------------------------*/


#include "stdint.h"
#include "string.h"
#include "Includes.h"
#include "driver/ISR.h"
#include "driver/delay.h"


// Config word (fuses)
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_ON & BOREN_OFF & CP_OFF & CPD_OFF & MCLRE_OFF);


void blink(int j);



/* ----------------------- Start implementation -----------------------------*/







int
main( void )
{

    CMCON = 0x07;

    /*
     * 1 uscita per rele
     * 2 uscita led (diagnostica)
     * 3 uscita led (diagnostica)
    */
    
    TRISA0 = 0;
    TRISA1 = 0;
    TRISA3 = 0;

 
    InitUART(BAUDRATE);
    timer_settings();

    //PEIE, Interrupt di periferica attivato
    //GIE, Gestione Interrupt attiva

    GIE  = 1;  							// Enable global interrupts
    PEIE = 1;  							// Enable Peripheral Interrupts

    blink(5);

   while(1)
   {
       ;
   }

 
}




void blink(int j)
{
    int i;
    int k;
    for(k = 0; k < j; k++)
    {
        LED1 = 1;
        LED2 = 1;
        for(i = 0; i < 1000; i ++)
            ;
        for(i = 0; i < 1000; i ++)
            ;

        LED1 = 0;
        LED2 = 0;

        for(i = 0; i < 1000; i ++)
            ;
        for(i = 0; i < 1000; i ++)
            ;

    }

}
