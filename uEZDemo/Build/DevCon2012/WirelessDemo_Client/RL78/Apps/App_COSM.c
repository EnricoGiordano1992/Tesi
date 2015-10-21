/*-------------------------------------------------------------------------*
 * File:  App_COSM.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_COSM 
 * @{
 *     @brief This demo reads the temperature, potentiometer, and RSSI and sends the
 *     data to the COSM.com website using an HTTP connection.
 *     Data is sent every 20 seconds.
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
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <system/console.h>
#include <CmdLib/GainSpan_SPI.h>
#include "Apps.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define COSM_DEMO_UPDATE_INTERVAL            15000 // ms
#define DATA_TO_SEND_TO_SERVER_ALL_LINES \
        "POST /v2/feeds/11366.csv?_method=put&key=103338a658c84debc9d4d0609362056882b6ccaa312d3de7fbde57e592630007 HTTP/1.1\r\n" \
        "User-Agent: curl/7.19.5 (i486-pc-linux-gnu) libcurl/7.19.5 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.15\r\n" \
        "Host: api.cosm.com\r\n" \
        "Accept: *" "/" "*\r\n" \
        "Content-Length: %d\r\n" \
        "Content-Type: application/x-www-form-urlencoded\r\n" \
        "\r\n"

/* IP Address of the remote TCP Server */
#define COSM_DEMO_REMOTE_TCP_SRVR_IP     "216.52.233.121" // api.cosm.com, must use ip address
#define COSM_DEMO_REMOTE_TCP_SRVR_PORT   80

/*---------------------------------------------------------------------------*
 * Routine:  App_COSM
 *---------------------------------------------------------------------------*/
/** Take a reading of temperature and potentiometer, and RSSI and send to 
 *      the COSM.com website using a TCP connection.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_COSM(void)
{
    static char command[512];
    uint8_t cid = 0;
    uint32_t start;
    uint32_t end;
    uint32_t count;
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[256];
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    static int16_t rssi = 0;
    ATLIBGS_TCPMessage msg;
    char *lines[10];
    uint8_t numLines;
    char *tokens[10];
    uint8_t numTokens;

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
             */
            rxMsgId = AtLibGs_TCPClientStart(COSM_DEMO_REMOTE_TCP_SRVR_IP,
                    COSM_DEMO_REMOTE_TCP_SRVR_PORT, &cid);
            if (ATLIBGS_MSG_ID_OK != rxMsgId) {
                /* TCP connection error */
#ifdef ATLIBGS_DEBUG_ENABLE
                ConsolePrintf("\n TCP Connection ERROR !\n");
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

             POST /v2/feeds/11366.csv?_method=put&key=103338a658c84debc9d4d0609362056882b6ccaa312d3de7fbde57e592630007 HTTP/1.1
             User-Agent: curl/7.19.5 (i486-pc-linux-gnu) libcurl/7.19.5 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.15
             Host: api.cosm.com
             Accept: "*"/"*"  <------- Note: Do not send the "" quotes, I just placed there to make sure this line is still a valid comment
             Content-Length: ? <------ Note: Determined by the data below
             Content-Type: application/x-www-form-urlencoded

             0,X
             1,Y
             2,Z

             where:

             X = Temperature reading
             Y = A/D reading
             Z = RSSI reading
             */

            // prints 0,X;1,Y;2,Z
            sprintf(content, "0,%d.%d\r\n1,%d.%d\r\n2,%d\r\n\r\n",
                    G_temp_int[0], G_temp_int[1], G_adc_int[0], G_adc_int[1],
                    rssi);

            // prints http command with Content-Length filled in
            sprintf(command, DATA_TO_SEND_TO_SERVER_ALL_LINES, strlen(content));
            strcat(command, content); // combines into 1 command

            App_PrepareIncomingData();
#ifdef ATLIBGS_DEBUG_ENABLE
            ConsolePrintf("(Send)\r\n");
#endif
            if (AtLibGs_SendTCPData(cid, (uint8_t *)command, strlen(command))
                    == ATLIBGS_MSG_ID_OK) {
                rxMsgId = AtLibGs_WaitForTCPMessage(5000);
                if ((rxMsgId == ATLIBGS_MSG_ID_DATA_RX) && (G_receivedCount)) {
                    AtLibGs_ParseTCPData(G_received, G_receivedCount, &msg);

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
                                (const char *)tokens[0], "HTTP/1.1") == 0)) {
                            if (strcmp((const char *)tokens[1], "200") == 0) {
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
            }

            /* Send command to GainSpan Client to close client/server connection from the
             www.COSM.com website

             Server IP address:  173.203.98.28
             Server port number: 80

             attempt disconnecting to the above server an infinite number of times until
             a successful disconnection is accomplished and the message for it is received
             from the GainSpan client
             */
            AtLibGs_Close(cid);
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
                // update temp and ADC readings and display on LCD
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
 * End of File:  App_COSM.c
 *-------------------------------------------------------------------------*/
