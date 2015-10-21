/*-------------------------------------------------------------------------*
 * File:  TestSPI_Speed.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestSPI_Speed
 * @{
 *     @brief Test the RX62N SPI driver by sending out bytes in a loop.  When used
 *     with a scope, the raw speed of the SPI driver can be measured.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <system/platform.h>
#include <drv/ADC.h>
#include <system/console.h>
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <sensors/Potentiometer.h>
#include <HostApp.h>
#include "Tests.h"
#include <drv/SPI.h>

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static bool G_done;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
static void MarkDone(void);

/*---------------------------------------------------------------------------*
 * Routine:  TestSPI_Speed
 *---------------------------------------------------------------------------*/
/** Test the RX62N SPI driver by repeatedly sending out an array of 256
 *      bytes of 0 .. 255.  A digital scope can then be used to measure
 *      the net throughput speed between CPU and WiFi module.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestSPI_Speed(void)
{
#if 0
    uint8_t send[7] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x07};
    uint8_t receive[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    while (1) {
        G_done = false;
        SPI_Transfer(SPI_WIFI_CHANNEL, sizeof(send), send, receive, MarkDone);
        while (!G_done)
        {}
    }
#else
    uint32_t i;

    DisplayLCD(LCD_LINE2, "SPI Speed");

    for (i = 0; i < 256; i++)
        G_testBuffer[i] = (uint8_t)i;

    while (1) {
        G_done = false;
        SPI_Transfer(SPI_WIFI_CHANNEL, 255, G_testBuffer,
                G_testBuffer2, MarkDone);
        //PORTE.DR.BIT.B0 = 1;
        while (!G_done) {
        }
        //PORTE.DR.BIT.B0 = 0;
    }
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  MarkDone
 *---------------------------------------------------------------------------*/
/** Callback from SPI routine to mark when SPI transaction is done.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
static void MarkDone(void)
{
    G_done = true;
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestSPI_Speed.c
 *-------------------------------------------------------------------------*/
