/*-------------------------------------------------------------------------*
 * File:  TestI2CTemperature.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestI2CTemperature
 * @{
 *     @brief Temperature tests
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
#include <sensors/Temperature.h>
#include "Tests.h"
#include <HostApp.h>
#include <CmdLib/GainSpan_SPI.h>

/*---------------------------------------------------------------------------*
 * Routine:  TestI2CTemperature
 *---------------------------------------------------------------------------*/
/** Test the temperature sensor by making repeated temperatur readings
 *      and outputting to the console and screen.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestI2CTemperature(void)
{
    uint16_t temp;
    char temp_int[2] = { 0, 0 };
    char lcd_buffer[20];
    uint32_t start;

    start = 0;
    DisplayLCD(LCD_LINE2, "I2C Temp");
    while (1) {
        while (MSTimerDelta(start) < 100) {
        }
        start += 100;
        temp = Temperature_Get();

        // Get the temperature and show it on the LCD
        temp_int[0] = (int16_t)temp / 16;
        temp_int[1] = (int16_t)((temp & 0x000F) * 10) / 16;

        sprintf((char *)lcd_buffer, "TEMP: %2d.%d C", temp_int[0], temp_int[1]);
        /* Display the contents of lcd_buffer onto the debug LCD */
        DisplayLCD(LCD_LINE3, (const uint8_t *)lcd_buffer);
        ConsolePrintf("%5d TEMP: %d.%d C\n", MSTimerGet(), temp_int[0],
                temp_int[1]);
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestI2CTemperature.c
 *-------------------------------------------------------------------------*/
