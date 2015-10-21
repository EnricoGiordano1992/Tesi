/*-------------------------------------------------------------------------*
 * File:  TestGainSpan_ATCommands.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestGainSpan_ATCommands
 * @{
 *     @brief Test the GainSpan SPI driver by sending AT commands and looking
 *     for proper response commands.  Run as fast as possible.
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
 * Routine:  TestGainSpan_ATCommands
 *---------------------------------------------------------------------------*/
/** In an infinite loop, send AT commands and look for the resulting
 *      OK response using the upper level GainSpan SPI functions.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestGainSpan_ATCommands(void)
{
    bool send = true;
    const uint8_t match[] = "AT\r\r\nOK\r\n\n\r\n";
    uint8_t in[sizeof(match)];
    uint8_t pos;
    uint8_t c;
    uint32_t start;

    DisplayLCD(LCD_LINE2, "GS ATCmd");

    /* Reset the connection by flushing any waiting data */
    start = MSTimerGet();
    while (MSTimerDelta(start) < 500) {
        if (GainSpan_SPI_ReceiveByte(SPI_WIFI_CHANNEL, &c)) {
            Console_UART_SendByte(c);
            start = MSTimerGet();
        }
    }

    start = MSTimerGet();
    while (1) {
        /* Infitine loop */
        if (send) {
            /* Do AT commands as fast as possible. */
            GainSpan_SPI_SendData("AT\r\n", 4);
            send = false;
            pos = 0;
            start += 10;
        }
        if (GainSpan_SPI_ReceiveByte(SPI_WIFI_CHANNEL, &c)) {
            if (c != GAINSPAN_SPI_CHAR_IDLE) {
                in[pos++] = c;
                Console_UART_SendByte(c);
                if (pos == (sizeof(match) - 1)) {
                    if (memcmp(match, in, sizeof(match) - 1) != 0) {
                        Console_UART_SendByte('_');
                        ErrorCode(3);
                    }

                    send = true;
                }
            }
        }
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestGainSpan_ATCommands.c
 *-------------------------------------------------------------------------*/
