/*-------------------------------------------------------------------------*
 * File:  App_HTTPDemo.c
 *-------------------------------------------------------------------------*/
 /** @addtogroup App_HTTPDemo
 * @{
 *     @brief This demo reads the temperature, potentiometer, and RSSI then sends it to
 *     a COSM server using the gainspan module's HTTP POST command
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
#include <system/mstimer.h>
#include <system/console.h>
#include <CmdLib/GainSpan_SPI.h>
#include <drv/Glyph/lcd.h>
#include "Apps.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define COSM_DEMO_UPDATE_INTERVAL            15000 // ms
/* IP Address of the remote TCP Server */
#define COSM_DEMO_REMOTE_HTTP_SRVR_IP     "api.cosm.com" // COSM.com (TODO: End with 28 or 29?)
#define COSM_DEMO_REMOTE_HTTP_SRVR_PORT   80

/*---------------------------------------------------------------------------*
 * Routine:  App_HTTPDemo
 *---------------------------------------------------------------------------*/
 /** Take a reading of temperature and potentiometer and send to the
 *      COSM.com website using the GainSpan's HTTP connection.
 * 
 * @return     void
 */
/*---------------------------------------------------------------------------*/
void App_HTTPDemo(void)
{
    static char command[512];
    uint8_t cid = 0;
    unsigned int start;
    unsigned int end;
    unsigned int count;
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    static int16_t rssi = 0;
    char lengthBuf[2];
    ATLIBGS_HTTPMessage msg;
    char *lines[10];
    uint8_t numLines;
    char *tokens[10];
    uint8_t numTokens;

    AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_TYPE,
            "application/x-www-form-urlencoded"); // content type header
    AtLibGs_HTTPConf(
            ATLIBGS_HTTP_HE_USER_A,
            "curl/7.19.5 (i486-pc-linux-gnu) libcurl/7.19.5 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.15");
    AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_HOST, "api.cosm.com");

    while (1) {
        start = MSTimerGet();

        // update temp and ADC readings and display on LCD
        App_TemperatureReadingUpdate(G_temp_int, true);
        App_PotentiometerUpdate(G_adc_int, true);
        DisplayLCD(LCD_LINE7, "");
        DisplayLCD(LCD_LINE8, "");

        // Do we need to connect to the AP?
        if (!AtLibGs_IsNodeAssociated())
            App_Connect(&G_nvsettings.webprov);

        if (AtLibGs_IsNodeAssociated()) {
            rssi = App_RSSIReading(true);

            DisplayLCD(LCD_LINE7, " Sending...");
            DisplayLCD(LCD_LINE8, "");

            /*  Set parameters to send command to GainSpan Client to create a TCP client/server connection to
             the www.COSM.com website at the following server IP address and port number:

             Server IP address:  173.203.98.28
             Server port number: 80

             attempt connecting to the above server an infinite number of times until a successful
             client / server connection to it is accomplished and the message for it  is received from
             GainSpan client

             Now use HTTP post/get functions to send data instead of manual TCP socket
             */

            rxMsgId = AtLibGs_HTTPOpen(COSM_DEMO_REMOTE_HTTP_SRVR_IP,
                    COSM_DEMO_REMOTE_HTTP_SRVR_PORT, false, "\0", "\0",
                    15, &cid);
            if (ATLIBGS_MSG_ID_OK != rxMsgId) {
                /* HTTP Open error */
#ifdef ATLIBGS_DEBUG_ENABLE
                ConsolePrintf("\n HTTP Connection ERROR !\n");
#endif
                AtLibGs_CloseAll();
                DisplayLCD(LCD_LINE7, " Open Fail");
                MSTimerDelay(2000);
                DisplayLCD(LCD_LINE7, "");
                continue;
            }

            /*  After indicating GainSpan module to start transmitting data over connection to
             COSM.com server, send a data set to the COSM.com server by sending the
             following ASCII data:

             0,X
             1,Y
             2,Z

             where:

             X = Temperature reading
             Y = A/D reading
             Z = RSSI reading
             */

            sprintf(command, "\r\n0,%d.%d\r\n1,%d.%d\r\n2,%d\r\n",
                    G_temp_int[0], G_temp_int[1], G_adc_int[0], G_adc_int[1],
                    rssi);

            App_PrepareIncomingData();
#ifdef ATLIBGS_DEBUG_ENABLE
            ConsolePrintf("(Send)\r\n");
#endif
            // configure HTTP content length Header
            sprintf(lengthBuf, "%d", strlen(command));
            AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_LENGTH, lengthBuf);

            AtLibGs_HTTPSend(cid,
                    ATLIBGS_HTTPSEND_POST, //send data using POSt method
                    5, // timeout in seconds to send data to GS module
                    "/v2/feeds/11366.csv?_method=put&key=103338a658c84debc9d4d0609362056882b6ccaa312d3de7fbde57e592630007\0", // correct page on server?
                    strlen(command), // length of content
                    command); //content to send
            rxMsgId = AtLibGs_WaitForHTTPMessage(5000); // needed to trigger command send

            if (rxMsgId == ATLIBGS_MSG_ID_HTTP_RESPONSE_DATA_RX) {
                AtLibGs_ParseHTTPData(G_received, G_receivedCount, &msg);

#ifdef ATLIBGS_DEBUG_ENABLE
                ConsoleSendString("Response:\r\n");
                ConsoleSendString((const char *)msg.message);
                ConsoleSendString("\r\n");
#endif
                numLines = AtLibGs_ParseIntoLines((char *)msg.message,
                        lines, 1);
                if (numLines >= 1) {
                    numTokens = AtLibGs_ParseIntoTokens(lines[0], ' ',
                            tokens, 10);
                    if ((numTokens >= 2) && (strcmp(
                            (const char *)tokens[0], "200") == 0)) {
                        if (strcmp((const char *)tokens[1], "OK") == 0) {
                            DisplayLCD(LCD_LINE7, "** Sent! **");
                        } else {
                            DisplayLCD(LCD_LINE7, "** FAIL **");
                        }
                    } else {
                        DisplayLCD(LCD_LINE7, "** FAIL **");
                    }
                } else {
                    DisplayLCD(LCD_LINE7, "** FAIL **");
                }
            }
            rxMsgId = AtLibGs_ResponseHandle();
            AtLibGs_HTTPClose(cid);
            MSTimerDelay(1000);
            DisplayLCD(LCD_LINE7, "");
        }

        /* Wait a little bit after server disconnecting client before connecting again*/
        /* Really cannot go faster than 15 seconds per sample per COSM's server limits */
        while (1) {
            end = MSTimerGet();
            if ((start + COSM_DEMO_UPDATE_INTERVAL - end)
                    < COSM_DEMO_UPDATE_INTERVAL) {
                count = start + COSM_DEMO_UPDATE_INTERVAL - end;
                if (count > 250)
                    count = 250;
                MSTimerDelay(count);
                // update temp and ADC readings and display them on LCD
                App_TemperatureReadingUpdate(G_temp_int, true);
                App_PotentiometerUpdate(G_adc_int, true);
                DisplayLCD(LCD_LINE7, "");
                DisplayLCD(LCD_LINE8, "");
            } else {
                break;
            }
        }
    }
}
/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_HTTPDemo.c
 *-------------------------------------------------------------------------*/
