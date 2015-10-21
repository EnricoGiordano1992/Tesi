/*-------------------------------------------------------------------------*
 * File:  App_PassThroughUART.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_PassThroughUART 
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
#include <CmdLib/AtCmdLib.h>

/*---------------------------------------------------------------------------*
 * Routine:  App_PassThroughUART
 *---------------------------------------------------------------------------*/
/** Put the unit in a pass through mode where SCI2 bytes are sent to
 *      SCI6 and visa-versa.  Additionally, the baud rate of the module
 *      on SCI6 is raised to 115200 baud.
 *      
 *  @return    void
 */
/*---------------------------------------------------------------------------*/
void App_PassThroughUART(void)
{
    uint8_t c;
    const uint8_t msg[] = "PassThroughUART:\r\n";

    ClearLCD();
    DisplayLCD(LCD_LINE1, "UART PT");

    /* Try to get an AT command to respond */
    Console_UART_SendData(msg, sizeof(msg));
    GainSpan_UART_SendData("AT\r\n", 4);
    MSTimerDelay(100);
    while (GainSpan_UART_ReceiveByte(&c)) {
        Console_UART_SendByte(c);
    }
    AtLibGs_SetEcho(1);
    GainSpan_UART_SendData("AT\r\n", 4);
    MSTimerDelay(100);
    while (GainSpan_UART_ReceiveByte(&c)) {
        Console_UART_SendByte(c);
    }

    /* Change the baud rate from 9600 to 115200 baud */
    GainSpan_UART_SendData("ATB=115200,8,N,1\r\n", 18);
    MSTimerDelay(100);
    while (GainSpan_UART_ReceiveByte(&c)) {
        Console_UART_SendByte(c);
    }

    /* Change SCI6's baud rate */
    GainSpan_UART_Stop();
    GainSpan_UART_Start(115200);
    MSTimerDelay(100);

    /* Make sure an AT command response is received */
    GainSpan_UART_SendData("AT\r\n", 4);
    MSTimerDelay(100);
    while (GainSpan_UART_ReceiveByte(&c)) {
        Console_UART_SendByte(c);
    }
    GainSpan_UART_SendData("AT\r\n", 4);
    MSTimerDelay(100);
    while (GainSpan_UART_ReceiveByte(&c)) {
        Console_UART_SendByte(c);
    }

    /* Go into the pass through mode */
    while (1) {
        if (Console_UART_ReceiveByte(&c))
            GainSpan_UART_SendByte(c);
        if (GainSpan_UART_ReceiveByte(&c))
            Console_UART_SendByte(c);
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_PassThroughUART.c
 *-------------------------------------------------------------------------*/
