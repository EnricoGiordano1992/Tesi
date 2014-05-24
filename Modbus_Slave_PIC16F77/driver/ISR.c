#include "../Includes.h"
#include "ISR.h"



extern void blink(int j);
extern void TX_ISR_deactivate();
extern void uart_rx_interrupt();
extern void timer_interrupt();


void interrupt ISR(void)
{


    /***************************************
     ****************************************
     *
     *  UART INTERRUPT
     *
     ***************************************
     ****************************************/
    
    if(RCIF)  // If UART Rx Interrupt
    {
        if(OERR) // If over run error, then reset the receiver
	{
            CREN = 0;
            CREN = 1;
	}
        uart_rx_interrupt();

	RCIF = 0;
    }




     /***************************************
     ****************************************
     *
     *  TIMER0 INTERRUPT
     *
     ***************************************
     ****************************************/


     if (T0IF) // timer0 interrupt
     {

        timer_interrupt();
         
        TMR0 = 100; // reset Timer0
        T0IF=0; // Resetto flag interrupt timer 0,
     }

}
