/*-------------------------------------------------------------------------*
 * File:  TestSPI_IdleChars.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestSPI_IdleChars
 * @{
 *     @brief Send a group of idle characters to the WiFi module in a loop.
 *     Verify that it returns idle characters from the module.
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

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static bool G_done;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
static void IMarkDone(void);

/*---------------------------------------------------------------------------*
 * Routine:  TestSPI_IdleChars
 *---------------------------------------------------------------------------*/
/** Test the SPI module by sending a group of idle characters and
 *      check the returned characters and that they are all IDLE characters.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestSPI_IdleChars(void)
{
    uint32_t i;
    static volatile uint8_t c;

    DisplayLCD(LCD_LINE2, "SPI Idle");
    for (i = 0; i < 256; i++)
        G_testBuffer[i] = 0xF5;
    while (1) {
        G_done = false;
        for (i=0; i<16; i++)
            G_testBuffer2[i] = 0x00;
        SPI_Transfer(SPI_WIFI_CHANNEL, 16, G_testBuffer,
                G_testBuffer2, IMarkDone);
        while (!G_done) {
        }
        for (i = 0; i < 16; i++) {
            if (G_testBuffer2[i] != 0xF5) {
                ErrorCode(3);
            }
        }
        MSTimerDelay(10);
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  IMarkDone
 *---------------------------------------------------------------------------*/
/** Callback from SPI routine to mark when SPI transaction is done.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
static void IMarkDone(void)
{
    G_done = true;
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestSPI_IdleChars.c
 *-------------------------------------------------------------------------*/
