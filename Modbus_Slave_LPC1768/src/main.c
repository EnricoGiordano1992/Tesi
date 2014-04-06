/*
===============================================================================
 Name        : main.c
 Author      : Enrico Giordano
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#include "lcd_horizontal.h"
#include "lpc1768.h"

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

/*
 *
 * 	Discrete Input		Single bit		Read-Only
	Coils				Single bit		Read-Write
	Input Registers		16-bit word		Read-Only
	Holding Registers	16-bit word		Read-Write
 *
 *
 */


/*
 * The Discrete Inputs represent a single bit (Boolean) which can only be read.
 * In other words, the master can only perform a read action on the discrete inputs.
 * The same holds for the Input Registers.
 * The master can only read the slave’s Input Registers.
 * The difference between the Discrete Inputs and the Input Registers is
 * that the Input Registers represent 16 bits while the Discrete Inputs are only a single bit.
 * The Coils also represent a Boolean data type which can be read and written to by the master.
 * The Holding Registers represent a 16 bit word that can be read and written to.

 *
 */

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

#define REG_HOLDING_START           2000
#define REG_HOLDING_NREGS           130

static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{
    int n = 0;
    int delay_da_utente = 0;

	//initialize all

	SystemInit();

	resetGPIO();

	setOutput_LEDint();

	setOutput_ADC_Rint();

	setJoySwitch();

	turn_off_the_LEDS();

	Initial_Hardware();	//init lcd hardware

	Initial_GLCD_Hor();	//init lcd "software"

	bg_color(BLACK);

	int sto_premendo = 0;

    eMBErrorCode    eStatus;

    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, /*MB_PAR_EVEN*/ MB_PAR_NONE );

    /* Initialize the holding register values before starting the
     * Modbus stack
     */
    int i;

    for( i = 0; i < REG_HOLDING_NREGS; i++ )
    {
        usRegHoldingBuf[i] = ( unsigned short )i;
    }


    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    usRegInputBuf[0] = 0;
    usRegInputBuf[1] = 0;
    usRegInputBuf[2] = 0;

    for( ;; )
    {
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;

        if(joyswitch_left())
        	if(sto_premendo == 0)
        	{
        		sto_premendo = 1;
        		usRegInputBuf[1]--;
        	}
        if(joyswitch_right())
        	if(sto_premendo == 0)
        	{
        		sto_premendo = 1;
        		usRegInputBuf[1]++;
        	}

        if(joyswitch_down())
        	if(sto_premendo == 0)
        	{
        		sto_premendo = 1;
        		usRegInputBuf[2]--;
        	}

        if(joyswitch_up())
        	if(sto_premendo == 0)
        	{
        		sto_premendo = 1;
        		usRegInputBuf[2]++;
        	}

        if(!joyswitch_down() && !joyswitch_left() && !joyswitch_right() && !joyswitch_up())
        	sto_premendo = 0;

        usRegHoldingBuf[0] = n = convert_from_ADC_VR() ;

//		LPC_GPIO2->FIOSET = (1<< ((n / 512 ) + 1));		// ON LED[0]


    }
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
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
