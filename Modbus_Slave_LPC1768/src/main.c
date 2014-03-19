/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;


/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{

	SystemInit();

	//__disable_irq();

	LPC_PINCON->PINSEL4 &= ~(0xFFFF); 	// Reset P2[0..7] = GPIO
	LPC_GPIO2->FIODIR |= 0xFF;			// P2[0..7] = Outputs
	LPC_GPIO2->FIOCLR =	0xFF;


    eMBErrorCode    eStatus;

    eStatus = eMBInit( MB_RTU, /*0x0A*/ 10, 0, 38400, MB_PAR_EVEN );
//    if(eStatus != MB_ENOERR )
//    	while(1)
//    		LPC_GPIO2->FIOSET = 0xff;

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );
//    if(eStatus != MB_ENOERR )
//    	while(1)
//    		LPC_GPIO2->FIOSET = 0xFF;

    //__enable_irq();

    for( ;; )
    {
        ( void )eMBPoll(  );


        //LPC_GPIO2->FIOSET = 0x01;

        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;

    }

    return 0;
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    ( void )eMode;
    return MB_ENOREG;
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
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
