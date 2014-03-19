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
 * File: $Id: porttimer.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "LPC17xx.h"
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );
//    __irq;

//void TIMER0_IRQHandler (void);

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	LPC_SC->PCONP |= 1 << 1; //Power up Timer 0
	LPC_SC->PCLKSEL0 |= 1 << 3; // Clock for timer = CCLK/2

    // Timer0 Configuration
/*	LPC_TIM0->PR = 0;                   // Prscaler Register = 0
	LPC_TIM0->PC = 0;                   // Prscaler Counter = 0

	LPC_TIM0->TC = 0;                   // Timer Counter = 0
*/
	LPC_TIM0->MR0 = ( PCLK / 20000 ) * usTim1Timerout50us;      // Interval of (50us * usTim1Timerout50us)
//	LPC_TIM0->MCR = 3;                  // Bit 0 = 1 - Interruption on MR0
    // Bit 1 = 1 - Reset on MR0

	LPC_TIM0->MCR |= 1 << 0; // Interrupt on Match0 compare
	LPC_TIM0->TCR |= 1 << 1; // Reset Timer0

	NVIC_EnableIRQ(TIMER0_IRQn); // Enable timer interrupt
	LPC_TIM0->TCR |= 1 << 0; // Start timer

//	LPC_TIM0->TCR = 0;                  // Timer Counter and Prescale Counter Disabled
/*	LPC_SC->PCONP |= 1 << 1;
	LPC_TIM0->TCR = 0 << 1;
	LPC_TIM0->MCR = 0x00000003;
	//LPC_TIM0->MR0 = 12000000; //frequency of toggling
	NVIC_EnableIRQ(TIMER0_IRQn); //enable interrupts
	LPC_TIM0->TCR |= 0x00000001; //start timer0
    // Configure Timer0 Interruption
//    VICVectAddr1 = ( unsigned int )prvvTIMERExpiredISR; // Timer0 Interruption - Priority 1
	NVIC_EnableIRQ(TIMER0_IRQn);
//	VICVectCntl1 = 0x20 | 4;
//    VICIntEnable = ( 1 << 4 );  // Enable Timer0 Interruption
*/
    return TRUE;
}


void
vMBPortTimersEnable(  )
{
//	LPC_TIM0->TCR = 0x02;               // Disable Timer and Reset Counter
//	LPC_TIM0->TCR = 0x01;               // Enable Timer
	LPC_TIM0->TCR |= 1 << 1;			//reset counter
}

void
vMBPortTimersDisable(  )
{
	LPC_TIM0->TCR = 0x02;               // Disable Timer and Reset Counter
}

static void
prvvTIMERExpiredISR( void ) //__irq
{
    ( void )pxMBPortCBTimerExpired(  );

//    LPC_TIM0->IR = 0xFF;		//reset interrupt
//    VICVectAddr = 0xFF;         // Acknowledge Interrupt
//    NVIC_SetPendingIRQ(TIMER0_IRQn);
//    VICVectAddr = 0;
}

void TIMER0_IRQHandler(void)
{
    //LPC_GPIO2->FIOSET = ~LPC_GPIO2->FIOSET ;
    LPC_GPIO2->FIOPIN ^= 0x000000FF;
    LPC_TIM0->IR |= 1 << 0;

    prvvTIMERExpiredISR();
}
