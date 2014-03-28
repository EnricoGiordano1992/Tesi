/*
===============================================================================
 Name        : main.c
 Author      : Enrico Giordano
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/


#include "Includes.h"

// Config word (fuses)
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & BOREN_ON & CP_OFF);


/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- Defines ------------------------------------------*/

#define REG_HOLDING_START           1000
#define REG_HOLDING_NREGS           10

static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];


/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{

    eMBErrorCode    eStatus;

    
    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, MB_PAR_NONE );

    /* Initialize the holding register values before starting the
     * Modbus stack
     */

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    //PEIE, Interrupt di periferica attivato
    //GIE, Gestione Interrupt attiva

    GIE  = 1;  							// Enable global interrupts
    PEIE = 1;  							// Enable Peripheral Interrupts

    while(1)
    {
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */



    }
}


eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
/*    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    ( void )eMode;
*/
	{
	    eMBErrorCode    eStatus = MB_ENOERR;
	    int             iRegIndex;

	    if( ( usAddress >= REG_HOLDING_START ) &&
	        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
	    {
	        iRegIndex = ( int )( usAddress - usRegHoldingStart );
	        switch ( eMode )
	        {
	            /* Pass current register values to the protocol stack. */
	        case MB_REG_READ:
	            while( usNRegs > 0 )
	            {
	                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
	                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
	                iRegIndex++;
	                usNRegs--;
	            }
	            break;

	            /* Update current register values with new values from the
	             * protocol stack. */
	        case MB_REG_WRITE:
	            while( usNRegs > 0 )
	            {
	                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
	                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
	                iRegIndex++;
	                usNRegs--;
	            }
	        }
	    }
	    else
	    {
	        eStatus = MB_ENOREG;
	    }
	    return eStatus;
	}

//	return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
        return MB_ENOREG;

}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
