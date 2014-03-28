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
#include "port.h"
#include "driver/TIMER.h"
#include "driver/ISR.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/

extern unsigned short modbus_time;
extern unsigned short actual_time;


/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    timer_settings();
    modbus_time = usTim1Timerout50us;
    actual_time = 0;
    
    return TRUE;
}


void
vMBPortTimersEnable(  )
{

    // Interrupt su Timer0 attivato
    TMR0IE = 1;
    
    // Flag interrupt su Timer0
    T0IF = 0;

    // Imposto Timer0 a 100   (1 tick per 1 ms)
    TMR0=100;

    actual_time = 0;

}

void
vMBPortTimersDisable(  )
{
    // Interrupt su Timer0 disattivato
    TMR0IE = 0;

    // Flag interrupt su Timer0
    T0IF = 0;

    // Imposto Timer0 a 100   (1 tick per 1 ms)
    TMR0=100;


}

void DelayUs(unsigned short uSec)
{
    unsigned char _dcnt;
    _dcnt = (uSec)*((20000000)/(12000000));
    while(--_dcnt != 0);
}

void vMBPortTimersDelay(USHORT usTimeOutMs)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
		} while(--i);
	} while(--usTimeOutMs);
}