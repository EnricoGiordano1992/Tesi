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
    return TRUE;
}


void
vMBPortTimersEnable(  )
{
}

void
vMBPortTimersDisable(  )
{
}

void TIMER0_IRQHandler(void)
{

    ( void )pxMBPortCBTimerExpired(  );


}

void vMBPortTimersDelay(USHORT usTimeOutMs)
{

}