/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
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
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

#include "LPC17xx.h"
#include "port.h"
#include "uart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void sio_irq( void );//__irq;
     static void     prvvUARTTxReadyISR( void );
     static void     prvvUARTRxISR( void );

     volatile uint32_t UART0Status, UART1Status;
     volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1;
     volatile uint8_t UART0Buffer[BUFSIZE], UART1Buffer[BUFSIZE];
     volatile uint32_t UART0Count = 0, UART1Count = 0;

     void UART0_IRQHandler(void);


     /*----------------------------------------------------------------------------
       Defines for ring buffers
      *---------------------------------------------------------------------------*/
     #define SER_BUF_SIZE               (128)               // serial buffer in bytes (power 2)
     #define SER_BUF_MASK               (SER_BUF_SIZE-1ul)  // buffer size mask

     /* Buffer read / write macros */
     #define SER_BUF_RESET(serBuf)      (serBuf.rdIdx = serBuf.wrIdx = 0)
     #define SER_BUF_WR(serBuf, dataIn) (serBuf.data[SER_BUF_MASK & serBuf.wrIdx++] = (dataIn))
     #define SER_BUF_RD(serBuf)         (serBuf.data[SER_BUF_MASK & serBuf.rdIdx++])
     #define SER_BUF_EMPTY(serBuf)      (serBuf.rdIdx == serBuf.wrIdx)
     #define SER_BUF_FULL(serBuf)       (serBuf.rdIdx == serBuf.wrIdx+1)
     #define SER_BUF_COUNT(serBuf)      (SER_BUF_MASK & (serBuf.wrIdx - serBuf.rdIdx))

     // buffer type
     typedef struct __SER_BUF_T {
       unsigned char data[SER_BUF_SIZE];
       unsigned int wrIdx;
       unsigned int rdIdx;
     } SER_BUF_T;

     unsigned long          ser_txRestart;                  // NZ if TX restart is required
     unsigned short         ser_lineState;                  // ((msr << 8) | (lsr))
     SER_BUF_T              ser_out;                        // Serial data buffers
     SER_BUF_T              ser_in;

     void ser_InitPort0 (unsigned long baudrate, unsigned int  databits,
                       unsigned int  parity,   unsigned int  stopbits);

/* ----------------------- Start implementation -----------------------------*/
     void            vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
//        U1IER |= 0x01;
//    	LPC_UART0->IER |= 0x01; /* Re-enable RBR */
    	LPC_UART0->IER = IER_THRE | IER_RLS | IER_RBR;
    }
    else
    {
//        U1IER &= ~0x01;
//    	LPC_UART0->IER &= ~0x01; /* Disable RBR */
    	 LPC_UART0->IER = IER_THRE | IER_RLS;
    }
    if( xTxEnable )
    {
//        U1IER |= 0x02;
    	LPC_UART0->IER |= 0x02;
    	prvvUARTTxReadyISR(  );
    }
    else
    {
//        U1IER &= ~0x02;
    	LPC_UART0->IER &= ~0x02;
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
//    USHORT          cfg = 0;
//    ULONG           reload = ( ( PCLK / ulBaudRate ) / 16UL );
//    volatile char   dummy;

    ( void )ucPORT;
    /* Configure UART0 Pins */

    UARTInit(0, ulBaudRate);
    //ser_InitPort0(ulBaudRate, ucDataBits, eParity, 1);

    return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    LPC_UART0->THR = ucByte;

    /* Wait till U0THR and U0TSR are both empty */
    while( !( LPC_UART0->LSR & 0x20 ) )
    {
    }

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    while( !( LPC_UART0->LSR & 0x01 ) )
    {
    }

    /* Receive Byte */
    *pucByte = LPC_UART0->RBR;

    return TRUE;
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
//    LPC_GPIO2->FIOCLR = 0x02;
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
    LPC_GPIO2->FIOSET = 0x02;
}



/*----------------------------------------------------------------------------
  open the serial port
 *---------------------------------------------------------------------------*/
void ser_OpenPort (char portNum) {

  if ( portNum == 0 )
  {
	/* Port 0 */
	NVIC_DisableIRQ(UART0_IRQn);
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;     /* RxD0 is P0.3 and TxD0 is P0.2 */
  }
  else
  {
	/* Port 1 */
	NVIC_DisableIRQ(UART1_IRQn);
	LPC_PINCON->PINSEL4 &= ~0x0000000F;
	LPC_PINCON->PINSEL4 |= 0x0000000A;    /* Enable RxD1 P2.1, TxD1 P2.0 */
  }
  return;
}

/*----------------------------------------------------------------------------
  close the serial port
 *---------------------------------------------------------------------------*/
void ser_ClosePort (char portNum ) {
  if ( portNum == 0 )
  {
	/* POrt 0 */
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	/* Disable the interrupt in the VIC and UART controllers */
	LPC_UART0->IER = 0;
	NVIC_DisableIRQ(UART0_IRQn);
  }
  else
  {
	/* Port 1 */
	LPC_PINCON->PINSEL4 &= ~0x0000000F;
	/* Disable the interrupt in the VIC and UART controllers */
	LPC_UART1->IER = 0;
	NVIC_DisableIRQ(UART1_IRQn);
  }
  return;
}

/*----------------------------------------------------------------------------
  initialize the serial port
 *---------------------------------------------------------------------------*/
void ser_InitPort0 (unsigned long baudrate, unsigned int  databits,
                  unsigned int  parity,   unsigned int  stopbits) {

  unsigned char lcr_p, lcr_s, lcr_d;
  unsigned int dll;
  unsigned int pclkdiv, pclk;

  switch (databits) {
    case 5:                                            // 5 Data bits
      lcr_d = 0x00;
    break;
    case 6:                                            // 6 Data bits
      lcr_d = 0x01;
    break;
    case 7:                                            // 7 Data bits
      lcr_d = 0x02;
    break;
    case 8:                                            // 8 Data bits
    default:
      lcr_d = 0x03;
    break;
  }

  switch (stopbits) {
    case 1:                                            // 1,5 Stop bits
    case 2:                                            // 2   Stop bits
      lcr_s = 0x04;
    break;
    case 0:                                            // 1   Stop bit
    default:
      lcr_s = 0x00;
    break;
  }

  switch (parity) {
    case 1:                                            // Parity Odd
      lcr_p = 0x08;
    break;
    case 2:                                            // Parity Even
      lcr_p = 0x18;
    break;
    case 3:                                            // Parity Mark
      lcr_p = 0x28;
    break;
    case 4:                                            // Parity Space
      lcr_p = 0x38;
    break;
    case 0:                                            // Parity None
    default:
      lcr_p = 0x00;
    break;
  }

  SER_BUF_RESET(ser_out);                              // reset out buffer
  SER_BUF_RESET(ser_in);                               // reset in buffer

  /* Bit 6~7 is for UART0 */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

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

  dll = (pclk/16)/baudrate ;	/*baud rate */
  LPC_UART0->FDR = 0;                             // Fractional divider not used
  LPC_UART0->LCR = 0x80 | lcr_d | lcr_p | lcr_s;  // Data bits, Parity,   Stop bit
  LPC_UART0->DLL = dll;                           // Baud Rate depending on PCLK
  LPC_UART0->DLM = (dll >> 8);                    // High divisor latch
  LPC_UART0->LCR = 0x00 | lcr_d | lcr_p | lcr_s;  // DLAB = 0
  //LPC_UART0->IER = 0x03;                          // Enable TX/RX interrupts
  LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */

  LPC_UART0->FCR = 0x07;				/* Enable and reset TX and RX FIFO. */
  ser_txRestart = 1;                                   // TX fifo is empty

  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART0_IRQn);
  return;
}

/*----------------------------------------------------------------------------
  initialize the serial port
 *---------------------------------------------------------------------------*/
void ser_InitPort1 (unsigned long baudrate, unsigned int  databits,
                  unsigned int  parity,   unsigned int  stopbits) {

  unsigned char lcr_p, lcr_s, lcr_d;
  unsigned int dll;
  unsigned int pclkdiv, pclk;

  switch (databits) {
    case 5:                                            // 5 Data bits
      lcr_d = 0x00;
    break;
    case 6:                                            // 6 Data bits
      lcr_d = 0x01;
    break;
    case 7:                                            // 7 Data bits
      lcr_d = 0x02;
    break;
    case 8:                                            // 8 Data bits
    default:
      lcr_d = 0x03;
    break;
  }

  switch (stopbits) {
    case 1:                                            // 1,5 Stop bits
    case 2:                                            // 2   Stop bits
      lcr_s = 0x04;
    break;
    case 0:                                            // 1   Stop bit
    default:
      lcr_s = 0x00;
    break;
  }

  switch (parity) {
    case 1:                                            // Parity Odd
      lcr_p = 0x08;
    break;
    case 2:                                            // Parity Even
      lcr_p = 0x18;
    break;
    case 3:                                            // Parity Mark
      lcr_p = 0x28;
    break;
    case 4:                                            // Parity Space
      lcr_p = 0x38;
    break;
    case 0:                                            // Parity None
    default:
      lcr_p = 0x00;
    break;
  }

  SER_BUF_RESET(ser_out);                              // reset out buffer
  SER_BUF_RESET(ser_in);                               // reset in buffer

  /* Bit 8,9 are for UART1 */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;

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

  dll = (pclk/16)/baudrate ;	/*baud rate */
  LPC_UART1->FDR = 0;                             // Fractional divider not used
  LPC_UART1->LCR = 0x80 | lcr_d | lcr_p | lcr_s;  // Data bits, Parity,   Stop bit
  LPC_UART1->DLL = dll;                           // Baud Rate depending on PCLK
  LPC_UART1->DLM = (dll >> 8);                    // High divisor latch
  LPC_UART1->LCR = 0x00 | lcr_d | lcr_p | lcr_s;  // DLAB = 0
  LPC_UART1->IER = 0x03;                          // Enable TX/RX interrupts

  LPC_UART1->FCR = 0x07;				/* Enable and reset TX and RX FIFO. */
  ser_txRestart = 1;                                   // TX fifo is empty

  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART1_IRQn);
  return;
}

/*----------------------------------------------------------------------------
  read data from serial port
 *---------------------------------------------------------------------------*/
int ser_Read (char *buffer, const int *length) {
  int bytesToRead, bytesRead;

  /* Read *length bytes, block if *bytes are not avaialable	*/
  bytesToRead = *length;
  bytesToRead = (bytesToRead < (*length)) ? bytesToRead : (*length);
  bytesRead = bytesToRead;

  while (bytesToRead--) {
    while (SER_BUF_EMPTY(ser_in));                     // Block until data is available if none
    *buffer++ = SER_BUF_RD(ser_in);
  }
  return (bytesRead);
}

/*----------------------------------------------------------------------------
  write data to the serial port
 *---------------------------------------------------------------------------*/
int ser_Write (char portNum, const char *buffer, int *length) {
  int  bytesToWrite, bytesWritten;

  // Write *length bytes
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;

  while (!SER_BUF_EMPTY(ser_out));               // Block until space is available if none
  while (bytesToWrite) {
      SER_BUF_WR(ser_out, *buffer++);            // Read Rx FIFO to buffer
      bytesToWrite--;
  }

  if (ser_txRestart) {
    ser_txRestart = 0;
	if ( portNum == 0 )
	{
	  LPC_UART0->THR = SER_BUF_RD(ser_out);             // Write to the Tx Register
    }
	else
	{
      LPC_UART1->THR = SER_BUF_RD(ser_out);             // Write to the Tx Register
	}
  }

  return (bytesWritten);
}

/*----------------------------------------------------------------------------
  check if character(s) are available at the serial interface
 *---------------------------------------------------------------------------*/
void ser_AvailChar (int *availChar) {

  *availChar = SER_BUF_COUNT(ser_in);

}

/*----------------------------------------------------------------------------
  read the line state of the serial port
 *---------------------------------------------------------------------------*/
void ser_LineState (unsigned short *lineState) {

  *lineState = ser_lineState;
  ser_lineState = 0;

}

/*----------------------------------------------------------------------------
  serial port 0 interrupt
 *---------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
  volatile unsigned long iir;
  volatile char   dummy;

  while(1)
	  LPC_GPIO2->FIOSET = 0xff;
  iir = LPC_UART0->IIR;


  switch ( iir & 0x0E )
  {
  case 0x06:             /* Receive Line Status */
      dummy = LPC_UART0->LSR;      /* Just clear the interrupt source */
      break;

  case 0x04:             /* Receive Data Available */
  case 0x0C:             /* Character Time-Out */
      prvvUARTRxISR(  );
      break;

  case 0x02:             /* THRE Interrupt */
      prvvUARTTxReadyISR(  );
      break;

  case 0x00:             /* Modem Interrupt */
      dummy = LPC_UART0->LSR;      /* Just clear the interrupt source */
      break;

  default:
      break;


  }



  ser_lineState = LPC_UART0->LSR & 0x1E;            // update linestate
  return;
}


/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART port, setup pin select,
**						clock, parity, stop bits, FIFO, etc.
**
** parameters:			portNum(0 or 1) and UART baudrate
** Returned value:		true or false, return false only if the
**						interrupt handler can't be installed to the
**						VIC table
**
*****************************************************************************/
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate )
{
  uint32_t Fdiv;
  uint32_t pclkdiv, pclk;

  if ( PortNum == 0 )
  {
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 6~7 is for UART0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
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

    LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART0_IRQn);

    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
    return (TRUE);
  }
  else if ( PortNum == 1 )
  {
	LPC_PINCON->PINSEL4 &= ~0x0000000F;
	LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
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

    LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
   LPC_UART1->DLM = Fdiv / 256;
   LPC_UART1->DLL = Fdiv % 256;
	LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART1_IRQn);

    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
    return (TRUE);
  }
  return( FALSE );
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
**
*****************************************************************************/
void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length )
{
  if ( portNum == 0 )
  {
    while ( Length != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(UART0TxEmpty & 0x01) );
	  LPC_UART0->THR = *BufferPtr;
	  UART0TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  BufferPtr++;
	  Length--;
	}
  }
  else
  {
	while ( Length != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(UART1TxEmpty & 0x01) );
	  LPC_UART1->THR = *BufferPtr;
	  UART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  BufferPtr++;
	  Length--;
    }
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/