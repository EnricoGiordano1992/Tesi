/*-------------------------------------------------------------------------*
 * File:  TestSPI_ATCommands.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestSPI_ATCommands
 * @{
 *     @brief Test the RX62N SPI driver by sending AT commands and looking
 *     for proper response commands.
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
#include <drv/ADC.h>
#include <system/console.h>
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <sensors/Potentiometer.h>
#include "Tests.h"
#include <HostApp.h>
#include <CmdLib/GainSpan_SPI.h>

/*---------------------------------------------------------------------------*
 * Routine:  TestSPI_ATCommands
 *---------------------------------------------------------------------------*/
/** In an infinite loop, send AT commands and look for the resulting
 *      OK response using the low level RX62N SPI functions.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestSPI_ATCommands(void)
{
    bool send = true;
    const uint8_t match[] = "AT\r\r\nOK\r\n\n\r\n";
    uint8_t transfer[20];
    uint8_t in[sizeof(match)];
    uint8_t pos;
    uint8_t c;
    uint32_t start;
    uint8_t r;
    uint8_t i;

    /* Reset the SPI connection by flushing any waiting data */
    DisplayLCD(LCD_LINE2, "SPI ATCmd");
    start = MSTimerGet();
    while (MSTimerDelta(start) < 500) {
        c = 0xF5;
        r = 0x00;
        SPI_Transfer(SPI_WIFI_CHANNEL, 1, &c, &r, 0);
        while (SPI_IsBusy(SPI_WIFI_CHANNEL)) {
        }
        if ((r) && (r != 0xF5)) {
            Console_UART_SendByte(r);
            start = MSTimerGet();
        }
    }

    while (1) {
        /* Infitine loop */
        /* Send AT's and looking for OK responses */
        if (send) {
            /* Send AT */
            strcpy((char *)transfer, "AT\r\n");
            SPI_Transfer(SPI_WIFI_CHANNEL, 4, transfer, transfer, 0);
            send = false;
            pos = 0;
        }

        if (!SPI_IsBusy(SPI_WIFI_CHANNEL)) {
            /* Look for OK */
            transfer[0] = 0xF5;
            transfer[1] = 0xF5;
            transfer[2] = 0xF5;
            transfer[3] = 0xF5;
            transfer[4] = 0xF5;
            SPI_Transfer(SPI_WIFI_CHANNEL, 5, transfer, transfer, 0);
            while (SPI_IsBusy(SPI_WIFI_CHANNEL)) {
            }
            for (i = 0; i < 5; i++) {
                c = transfer[i];
                if (c != GAINSPAN_SPI_CHAR_IDLE) {
                    in[pos++] = c;
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
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestSPI_ATCommands.c
 *-------------------------------------------------------------------------*/
