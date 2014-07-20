/*  File:     UART.c
    Purpose:  To provide UART functionality for PIC uC
*/

#include "../Includes.h"

extern void blink(int j);

void InitUART(unsigned long baudrate)
{
	TRISB2 = 0;   					// TX Pin , 0 == output
	TRISB1 = 1;   					// RX Pin,  1 == input
	
	SPBRG = ((_XTAL_FREQ/16)/baudrate) - 1;
	BRGH  = 1;                   	// Fast baudrate
	SYNC  = 0;						// Asynchronous
	SPEN  = 1;						// Enable serial port pins
	CREN  = 1;						// Enable reception
	SREN  = 0;						// No effect
        TXIE  = 0;						// Disable tx interrupts
        TXIF = 0;
	RCIE  = 1;						// Enable rx interrupts
	TX9   = 0;						// 8-bit transmission
	RX9   = 0;						// 8-bit reception
	TXEN  = 0;						// Reset transmitter
	TXEN  = 0;						// Disable the transmitter
}


void SendByteSerially(unsigned char Byte)  // Writes a character to the serial port
{
	while(!TXIF);  // wait for previous transmission to finish
	TXREG = Byte;
}

unsigned char ReceiveByteSerially(void)   // Reads a character from the serial port
{
	if(OERR) // If over run error, then reset the receiver
	{
		CREN = 0;
		CREN = 1;
	}
	
	while(!RCIF);  // Wait for transmission to receive
	
	return RCREG;
}

void SendStringSerially(const unsigned char* st)
{
	while(*st)
		SendByteSerially(*st++);
}

bit isTXEmpty()
{
    return TXIF;
}


void RX_ISR_activate()
{
    RCIE = 1;
}

void RX_ISR_deactivate()
{
    RCIE = 0;
}