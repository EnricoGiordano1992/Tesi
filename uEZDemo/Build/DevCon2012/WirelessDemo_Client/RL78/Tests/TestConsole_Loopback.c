/*-------------------------------------------------------------------------*
 * File:  TestConsole_Loopback.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestConsole_Loopback
 * @{
 *     @brief Test Console loopback
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <system/platform.h>
#include <system/console.h>
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <sensors/Potentiometer.h>
#include "Tests.h"
#include <HostApp.h>
#include <CmdLib/GainSpan_SPI.h>

/*---------------------------------------------------------------------------*
 * Routine:  TestConsole_Loopback
 *---------------------------------------------------------------------------*/
/** With a loopback attached to the serial port
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestConsole_Loopback(void)
{
    uint32_t count;
    uint8_t c;
    volatile uint32_t i;
    volatile uint32_t num = 0;

    DisplayLCD(LCD_LINE2, "UART Loop");

    // Put in a pattern of 0-255 bytes into the test buffer
    for (count = 0; count < sizeof(G_testBuffer); count++) {
        G_testBuffer[count] = count & 0xFF;
    }

    // Clear input buffer
    while (Console_UART_ReceiveByte(&c))
        ;

    // Make sure all bytes sent are received in a repeated loop
    while (1) {
        // Send 461 bytes
        Console_UART_SendData(G_testBuffer, 461);
        while (!Console_UART_IsTransmitEmpty())
            ;

        /* Delay 1 stop bit turn around */
        //__delay_cycles(RX62N_SYSTEM_CLOCK / Console_UART_BAUD_RATE);

        // Now reading them back and very they are correct
        for (count = 0; count < 461; count++) {
            if (!Console_UART_ReceiveByte(&c))
                ErrorCode(2); // out of data
            if (c != (count & 0xFF))
                ErrorCode(3); // incorrect data
        }
        num++;
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestConsole_Loopback.c
 *-------------------------------------------------------------------------*/
