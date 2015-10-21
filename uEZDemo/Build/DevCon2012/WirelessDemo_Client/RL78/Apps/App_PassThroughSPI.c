/*-------------------------------------------------------------------------*
 * File:  App_PassThroughSPI.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_PassThroughSPI 
 * @{
 *     @brief Put the unit in a pass through mode where SCI2 bytes are sent to
 *      SCI6 and visa-versa.  Additionally, the baud rate of the module
 *      on SCI6 is raised to 115200 baud.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <HostApp.h>
#include <system/platform.h>
#include <system/mstimer.h>
#include <drv/Glyph/lcd.h>
#include <CmdLib/GainSpan_SPI.h>
#include <CmdLib/AtCmdLib.h>

/*---------------------------------------------------------------------------*
 * Routine:  App_PassThroughSPI
 *---------------------------------------------------------------------------*/
/** Put the unit in a pass through mode where SCI2 bytes are sent to
 *      SCI6 and visa-versa.  Additionally, the baud rate of the module
 *      on SCI6 is raised to 115200 baud.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_PassThroughSPI(void)
{
    uint8_t c;
    const uint8_t msg[] = "PassThroughSPI:\r\n";

    ClearLCD();
    DisplayLCD(LCD_LINE1, "SPI PT");

    AtLibGs_SetEcho(ATLIBGS_ENABLE);
    Console_UART_SendData(msg, sizeof(msg));
    while (1) {
        if (Console_UART_ReceiveByte(&c))
            while (!GainSpan_SPI_SendByte(c))
                GainSpan_SPI_Update(GAINSPAN_SPI_CHANNEL);
        if (GainSpan_SPI_ReceiveByte(GAINSPAN_SPI_CHANNEL, &c))
            if (c != GAINSPAN_SPI_CHAR_IDLE)
                while (!Console_UART_SendByte(c))
                    {}
        GainSpan_SPI_Update(GAINSPAN_SPI_CHANNEL);
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_PassThroughSPI.c
 *-------------------------------------------------------------------------*/
