#include "../Includes.h"
#include "ISR.h"



extern void blink(int j);
extern void TX_ISR_deactivate();
extern void uart_rx_interrupt();
extern void timer_interrupt();


int is_first = 1;

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

void timer_interrupt(void)
{
    is_first = 1;
    timer_disable();
}


void uart_rx_interrupt(void)
{
    
   char c = ReceiveByteSerially();

   if(is_first) {
       
       switch(c){

           case 0x00:
               break;

           case 0x01:
               SIGNAL = 0;
               LED1 = 1;
               LED2 = 0;
               break;

           default:
               SIGNAL = 1;
               LED1 = 0;
               LED2 = 1;
               break;

       }

       is_first = 0;
       timer_enable();
    }
}