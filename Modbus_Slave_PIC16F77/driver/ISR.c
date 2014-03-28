#include "../Includes.h"
#include "ISR.h"


void interrupt ISR(void)
{
	if(RCIF)  // If UART Rx Interrupt
	{
		if(OERR) // If over run error, then reset the receiver
		{
			CREN = 0;
			CREN = 1;
		}

		prvvUARTRxISR();
	}

        if(TXIF == 0)   // uart tx empty
        {
            prvvUARTTxReadyISR();
        }


        	if (T0IF) // timer0 interrupt
		{
                    TMR0 = 100; // reset Timer0
                    T0IF=0; // Resetto flag interrupt timer 0,
		}

}