#include "../Includes.h"
#include "ISR.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "../mb.h"
#include "../mbport.h"

unsigned short modbus_time;
unsigned short actual_time;


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

	prvvUARTRxISR();
    }

    if(TXIF == 0)   // uart tx empty
    {
        prvvUARTTxReadyISR();
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
        if(++actual_time == modbus_time)
        {
            pxMBPortCBTimerExpired(  );
            actual_time = 0;
        }
        
        TMR0 = 100; // reset Timer0
        T0IF=0; // Resetto flag interrupt timer 0,
     }

}