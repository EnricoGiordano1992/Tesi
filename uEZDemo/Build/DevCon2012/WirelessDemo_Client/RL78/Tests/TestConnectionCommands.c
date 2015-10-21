/*-------------------------------------------------------------------------*
 * File:  TestConnectionCommands.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestConnectionCommands
 * @{
 *     @brief Test Connection commands
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <HostApp.h>
#include <system/platform.h>
#include <CmdLib/AtCmdLib.h>
#include <sensors/Temperature.h>
#include <sensors/Potentiometer.h>
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <system/console.h>
#include <CmdLib/GainSpan_SPI.h>
#include "Tests.h"

/*---------------------------------------------------------------------------*
 * Routine:  TestConnectionCommands
 *---------------------------------------------------------------------------*/
/** Test Connection Commands
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestConnectionCommands(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512]; // this seems too large just to get a MAC address
   
    DisplayLCD(LCD_LINE1, "Test All Cmd");
    ConsolePrintf("\fTEST: Testing All Commands\r\n");

    // Give the unit a little time to start up
    // (300 ms for GS1011 and 1000 ms for GS1500)
    MSTimerDelay(1000);

    // Check the link
    ConsolePrintf("TEST: Checking link\r\n");

    // Wait for the banner
    MSTimerDelay(500);

    // Clear out the buffers
    AtLibGs_FlushRxBuffer();

    DisplayLCD(LCD_LINE7, " Preparing");
    /* Send command to check */
    do {
        AtLibGs_FlushIncomingMessage();
        DisplayLCD(LCD_LINE8, "Checking...");
        rxMsgId = AtLibGs_Check(GAINSPAN_AT_DEFAULT_TIMEOUT);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);

    /* Send command to DISABLE echo */
    do {
        DisplayLCD(LCD_LINE8, "Echo Off...");
        rxMsgId = AtLibGs_SetEcho(ATLIBGS_DISABLE);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
    ConsolePrintf("TEST: Echo off\n");

    do {
        AtLibGs_GetMAC(content);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
    ConsolePrintf("TEST: MAC Addr: %s\n", content);

    // Enable DHCP
    do {
        DisplayLCD(LCD_LINE8, "DHCP On...");
        rxMsgId = AtLibGs_DHCPSet(1);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
    ConsolePrintf("TEST: DHCP on\n");

#ifdef ATLIBGS_SEC_PSK
    /* Store the PSK value. This call takes might take few seconds to return */
    do {
        DisplayLCD(LCD_LINE8, "Setting PSK");
        rxMsgId = AtLibGs_CalcNStorePSK(ATLIBGS_AP_SSID, ATLIBGS_AP_SEC_PSK);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
    ConsolePrintf("TEST: Set PSK\n");
#endif

    DisplayLCD(LCD_LINE8, "");

    while (1) {
        // Do we need to connect to the AP?
        DisplayLCD(LCD_LINE7, " Connecting ");
        ConsolePrintf("TEST: Connecting ... \n");
        /* Associate to a particular AP specified by SSID  */
        rxMsgId
                = AtLibGs_Assoc(ATLIBGS_AP_SSID, NULL, ATLIBGS_AP_CHANNEL);
        if (ATLIBGS_MSG_ID_OK != rxMsgId) {
            /* Association error - we can retry */
    #ifdef ATLIBGS_DEBUG_ENABLE
            ConsolePrintf("\n Association error - retry now \n");
    #endif
            DisplayLCD(LCD_LINE7, "** Failed **");
            MSTimerDelay(2000);
            DisplayLCD(LCD_LINE7, "");
            ConsolePrintf("TEST: Failed to connect\n");
        } else {
            /* Association success */
            AtLibGs_SetNodeAssociationFlag();
            DisplayLCD(LCD_LINE7, " Connected");
            MSTimerDelay(2000);
            DisplayLCD(LCD_LINE7, "");
            ConsolePrintf("TEST: Connected!\n");
            break;
        }
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestConnectionCommands.c
 *-------------------------------------------------------------------------*/
