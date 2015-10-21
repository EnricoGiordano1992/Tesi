/*-------------------------------------------------------------------------*
 * File:  App_UDPDemo.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_UDPDemo 
 * @{
 *     @brief Put the unit into UDP mode, then listen for a message from a PC.
 *      Resend the message then take a reading of the temperature and 
 *      potentiometer and send to the last connected PC every 10 seconds.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
#define UDP_DEMO_UPDATE_INTERVAL            1000 // ms
#define UDP_DEMO_SERVER_PORT                23
#define UDP_DEMO_CLIENT_PORT                8010

/*---------------------------------------------------------------------------*
 * Routine:  App_UDP_Demo
 *---------------------------------------------------------------------------*/
/** Put the unit into UDP mode, then listen for a message from a PC.
 *      Resend the message then take a reading of the temperature and 
 *      potentiometer and send to the last connected PC every 10 seconds.
 *      
 *  @return    void
 */
/*---------------------------------------------------------------------------*/
void App_UDPDemo(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    uint8_t cid;
    ATLIBGS_UDPMessage rxm;
    ATLIBGS_NetworkStatus network_status;
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    bool server_up = false;
    bool haveClient = false;
    uint16_t clientPort;
    char clientIP[20];
    bool got_packet = false;
    static char content[512];
    uint32_t time;
    uint32_t update;

    update = time = MSTimerGet();
    while (1) {
        if (MSTimerDelta(update) >= 250) {
            App_TemperatureReadingUpdate(G_temp_int, true);
            App_PotentiometerUpdate(G_adc_int, true);
            update = MSTimerGet();
        }
        if (!AtLibGs_IsNodeAssociated()) {
            App_Connect(&G_nvsettings.webprov);
        } else {
            if (!server_up) {
                // wait for UDP on port 23
                DisplayLCD(LCD_LINE7, "Starting...");
                if (AtLibGs_UDPServer_Start(UDP_DEMO_SERVER_PORT, &cid)
                        != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, "Server Fail!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    continue;
                } else {
                    server_up = true;
                }
                
                do {
                    DisplayLCD(LCD_LINE4, "IP: ???.???.");
                    DisplayLCD(LCD_LINE5, "    ???.???");
                    rxMsgId = AtLibGs_GetNetworkStatus(&network_status);
                } while (ATLIBGS_MSG_ID_OK != rxMsgId);

                sprintf(content, "IP: " _F8_ "." _F8_ ".", network_status.addr.ipv4[0], network_status.addr.ipv4[1]);
                DisplayLCD(LCD_LINE4, (uint8_t *)content);
                sprintf(content, "    " _F8_ "." _F8_, network_status.addr.ipv4[2], network_status.addr.ipv4[3]);
                DisplayLCD(LCD_LINE5, (uint8_t *)content);
                DisplayLCD(LCD_LINE8, "");
            }

            // Wait as a server until we get a UDP packet
            while (!haveClient) {
                DisplayLCD(LCD_LINE7, "Wait for UDP");
                App_PrepareIncomingData();
                rxMsgId = AtLibGs_WaitForUDPMessage(0);
                if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, "No UDP!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    continue;
                }

                AtLibGs_ParseUDPData(G_received, G_receivedCount, &rxm);

                // Pull out the client information
                strcpy(clientIP, rxm.ip);
                clientPort = rxm.port;
                haveClient = true;
                got_packet = true;
                DisplayLCD(LCD_LINE5, "");
                DisplayLCD(LCD_LINE6, "");
                DisplayLCD(LCD_LINE7, "");
            } // end while

            // If we have a client, process packets to/from that client
            if (haveClient) {
                if (!got_packet) {
                    // Are we getting a packet from the server?
                    rxMsgId = AtLibGs_WaitForUDPMessage(250);
                    if (rxMsgId == ATLIBGS_MSG_ID_OK) {
                        AtLibGs_ParseUDPData(G_received, G_receivedCount, &rxm);
                        got_packet = true;
                    }
                }

                if (got_packet) {
                    // Echo back a packet
                    memcpy(content, rxm.message, rxm.numBytes);
                    App_PrepareIncomingData();
                    AtLibGs_SendUDPData(cid, content, rxm.numBytes,
                            ATLIBGS_CON_UDP_SERVER, clientIP, clientPort);
                    time = MSTimerGet();
                    got_packet = false;
                } else if (MSTimerDelta(time) >= UDP_DEMO_UPDATE_INTERVAL) {
                    time = MSTimerGet();
                    DisplayLCD(LCD_LINE7, " Sending");
                    sprintf(content, "Temp: %d.%d, Pot: %d.%d%%\r\n", G_temp_int[0],
                            G_temp_int[1], G_adc_int[0], G_adc_int[1]);

                    rxMsgId = AtLibGs_SendUDPData(cid, (uint8_t *)content, strlen(content),
                            ATLIBGS_CON_UDP_SERVER, clientIP, clientPort);
                    if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                        DisplayLCD(LCD_LINE7, " Send Fail!");
                        MSTimerDelay(2000);
                        DisplayLCD(LCD_LINE7, "");
                        continue;
                    }
                    DisplayLCD(LCD_LINE7, "");
                } // else if
            } // end if
        } // end else
    } // end while
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_UDPDemo.c
 *-------------------------------------------------------------------------*/