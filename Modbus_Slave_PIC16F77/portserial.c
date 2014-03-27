/*
 * FreeModbus Libary: LPC17xx Port
 * Copyright (C) 2013 Enrico Giordano <enricogiordano1992@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2007-04-24 23:15:18 wolti Exp $
 */

//#include <LPC17xx.h>
#include "port.h"
//#include "uart.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/

void UART0_IRQHandler(void);

static void     prvvUARTTxReadyISR( void );

static void     prvvUARTRxISR( void );


volatile uint32_t UART0Status, UART2Status;
volatile uint8_t UART0TxEmpty = 1, UART2TxEmpty = 1;
//volatile uint8_t UART0Buffer[BUFSIZE], UART2Buffer[BUFSIZE];
volatile uint32_t UART0Count = 0, UART2Count = 0;

/* ----------------------- Start implementation -----------------------------*/
     void            vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
//        LPC_UART0->IER |= 0x01;	//enable RxInterrupt
    }
    else
    {
//        LPC_UART0->IER &= ~0x01;
    }
    if( xTxEnable )
    {
//        LPC_UART0->IER |= 0x02;	//enable TxInterrupt
        prvvUARTTxReadyISR(  );
    }
    else
    {
//        LPC_UART0->IER &= ~0x02;
    }
}

void
vMBPortClose( void )
{
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bInitialized = TRUE;

    ( void )ucPORT;
    uint32_t Fdiv;
    uint32_t pclkdiv, pclk;

    if ( ucPORT == 0 )
    {
//    	LPC_PINCON->PINSEL0 = 0;
    	/* RxD0 is P0.3 and TxD0 is P0.2 */
//      LPC_PINCON->PINSEL0 &= ~(0x03<<4);			// Reset P0.2 = GPIO
//      LPC_PINCON->PINSEL0 |=  (0x01<<4);			// Config P0.2 = TxD0
//  	LPC_PINCON->PINSEL0 &= ~(0x03<<6);			// Reset P0.3 = GPIO
//      LPC_PINCON->PINSEL0 |=  (0x01<<6);			// Config P0.3 = RxD0

  	/* By default, the PCLKSELx value is zero, thus, the PCLK for
  	all the peripherals is 1/4 of the SystemCoreClock. */
  	/* Bit 6~7 is for UART0 */
  	//pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
  	switch ( pclkdiv )
  	{
  	  case 0x00:
  	  default:
//  		pclk = SystemCoreClock/4;
  		break;
  	  case 0x01:
  //		pclk = SystemCoreClock;
  		break;
  	  case 0x02:
  //		pclk = SystemCoreClock/2;
  		break;
  	  case 0x03:
  //		pclk = SystemCoreClock/8;
  		break;
  	}

    //  LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
  	Fdiv = ( pclk / 16 ) / ulBaudRate ;	/*baud rate */
      //LPC_UART0->DLM = Fdiv / 256;
     // LPC_UART0->DLL = Fdiv % 256;
  	//LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
      //LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

     	//NVIC_EnableIRQ(UART0_IRQn);

//      LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
    }

    else bInitialized = FALSE;

    return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
//	static int txCount = 0;

	// There is no "TX FIFO full" flag, so the best we can do is
	// conservatively estimate that it's full every 16 calls.
		// Wait for the THR to empty.
//	while (!(LPC_UART0->LSR & 0x20));
//		txCount = 0;

	//LPC_UART0->THR = ucByte;
//	txCount++;

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	// Spin until a char is available on RX FIFO
//	while (!(LPC_UART0->LSR & 0x01));

    /* Receive Byte */
  //  *pucByte = LPC_UART0->RBR;

    return TRUE;
}


void
UART0_IRQHandler( void )
{
}


/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void
prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void
prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
