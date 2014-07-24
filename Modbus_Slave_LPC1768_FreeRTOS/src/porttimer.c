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
 * File: $Id: porttimer.c,v 1.1 2007-04-24 23:15:18 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include <LPC17xx.h>
#include "port.h"
#include "lcd_horizontal.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
void TIMER0_IRQHandler(void);

volatile uint32_t timer0_counter = 0;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    // Timer0 Configuration
//	timer0_counter = 0;
//	LPC_TIM0->MR0 = ( PCLK / 20000 ) * usTim1Timerout50us; //TimerInterval;
//	18000

	LPC_TIM0->PR = 0;                   // Prscaler Register = 0
	LPC_TIM0->PC = 0;                   // Prscaler Counter = 0

	LPC_TIM0->TC = 0;                   // Timer Counter = 0


	LPC_TIM0->MR0 = ( PCLK / 20000 ) * usTim1Timerout50us;

	LPC_TIM0->MCR = 3;										/* Interrupt and Reset on MR0 */

    LPC_TIM0->TCR = 0;                  // Timer Counter and Prescale Counter Disabled

	NVIC_EnableIRQ(TIMER0_IRQn);

    return TRUE;
}


void
vMBPortTimersEnable(  )
{
	LPC_TIM0->TCR = 0x02;
	LPC_TIM0->TCR = 1;
}

void
vMBPortTimersDisable(  )
{
//	LPC_TIM0->TCR = 0;
	LPC_TIM0->TCR = 0x02;
}

void TIMER0_IRQHandler(void)
{

    ( void )pxMBPortCBTimerExpired(  );

    LPC_TIM0->IR = 1;											/* clear interrupt flag */
//    timer0_counter++;
    return;

}
