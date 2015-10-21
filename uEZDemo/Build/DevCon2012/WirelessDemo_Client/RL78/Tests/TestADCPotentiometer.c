/*-------------------------------------------------------------------------*
 * File:  TestADCPotentiometer.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestADCPotentiometer
 * @{
 *     @brief Test the potentiometer by reading it every 100 ms and reporting
 *     it on the console.
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
#include "Tests.h"

/*---------------------------------------------------------------------------*
 * Routine:  TestADCPotentiometer
 *---------------------------------------------------------------------------*/
/** Read the ADC Potentiometer every 100 ms.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestADCPotentiometer(void)
{
    int32_t adc;
    char lcd_buffer[20];
    uint32_t start;
    int16_t G_adc_int[2] = { 0, 0 };

    start = 0;
    DisplayLCD(LCD_LINE2, "ADC Loop");
    while (1) {
        while (MSTimerDelta(start) < 100) {
        }
        start += 100;

        adc = Potentiometer_Get();
        G_adc_int[0] = (int16_t)(adc / 10);
        G_adc_int[1] = (int16_t)(adc % 10);

        sprintf((char *)lcd_buffer, " POT:%3d.%d %%", G_adc_int[0], G_adc_int[1]);
        /* Display the contents of lcd_buffer onto the debug LCD */
        DisplayLCD(LCD_LINE4, (const uint8_t *)lcd_buffer);
        ConsolePrintf("%5d POT : %3d.%d %%\n", MSTimerGet(), G_adc_int[0], G_adc_int[1]);
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestADCPotentiometer.c
 *-------------------------------------------------------------------------*/
