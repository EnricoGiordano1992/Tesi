/*-------------------------------------------------------------------------*
 * File:  App_TCPServerDemo.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_TCPServerDemo 
 * @{
 *     @brief Put the unit into TCP Server mode, then take a reading of temperature 
 *      and potentiometer and send to the last connected TCP client every 10 
 *      seconds
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
#define TCP_DEMO_UPDATE_INTERVAL            1000 // ms
#define TCP_DEMO_REMOTE_TCP_SRVR_PORT   23

/*---------------------------------------------------------------------------*
 * Routine:  App_TCPServerDemo
 *---------------------------------------------------------------------------*/
/** Put the unit into TCP Server mode, then take a reading of temperature 
 *      and potentiometer and send to the last connected TCP client every 10 
 *      seconds
 *      
 *  @return        void
 */
/*---------------------------------------------------------------------------*/
void App_TCPServerDemo(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512];
    uint8_t cid = 0;
    uint8_t server_cid = 0;
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    bool serving = false;
    uint32_t time = MSTimerGet();
    ATLIBGS_TCPMessage msg;
    ATLIBGS_TCPConnection connection;
    ATLIBGS_NetworkStatus network_status;
    bool connected = false;

#if 0
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512];
    uint8_t * cid = 0;
    static int16_t G_adc_int[2] = {0, 0};
    static char G_temp_int[2] = {0, 0};

    AtLibGs_TCPServer_Start(TCP_DEMO_REMOTE_TCP_SRVR_PORT);

    if (!AtLibGs_ParseTCPServerStartResponse(cid)) {
        ConsolePrintf("TCP Server could not start!\n");
    }

    ATLIBGS_UDPMessage rxm;

    while (1) {
        AtLibGs_WaitForTCPMessage(0); // block till message received
        DisplayLCD(LCD_LINE7, "PC Connected");

        ConsolePrintf("CID: %c\r\n", rxm.cid);
        ConsolePrintf("Message: %c\r\n", rxm.message);
        AtLibGs_SendTCPData((uint8_t)rxm.cid, &rxm.message, 1); //resend message to pc
        if (rxm.message[0] == 0x51) // if 'Q' received, go to next part
        {
            break;
        }
    }

    while (1) // enter here AFTER client sends data
    {
        // send temp and ADC to last received TCP client every 10 seconds
        MSTimerDelay(10000);

        App_TemperatureReadingUpdate(G_temp_int, true);
        App_PotentiometerUpdate(G_adc_int, true);

        static char content[512];
        sprintf(content, "0,%d.%d\r\n1,%d.%d\r\n", G_temp_int[0],
                G_temp_int[1], G_adc_int[0], G_adc_int[1]);
        // send data to client
        AtLibGs_SendTCPData((uint8_t)rxm.cid, (uint8_t *)content, strlen(
                        content));
    }
#endif

    while (1) {
        if (!AtLibGs_IsNodeAssociated()) {
            App_Connect(&G_nvsettings.webprov);
            serving = false;
        } else if (!serving) {
            DisplayLCD(LCD_LINE7, "Starting");
            // Start a TCP client
            rxMsgId = AtLibGs_TCPServer_Start(TCP_DEMO_REMOTE_TCP_SRVR_PORT,
                    &server_cid);
            if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                DisplayLCD(LCD_LINE7, "No Connect!");
                MSTimerDelay(2000);
                DisplayLCD(LCD_LINE7, "");
                continue;
            }
            if (server_cid == ATLIBGS_INVALID_CID) {
                DisplayLCD(LCD_LINE7, "No CID!");
                MSTimerDelay(2000);
                DisplayLCD(LCD_LINE7, "");
                continue;
            }
            DisplayLCD(LCD_LINE7, "");
            App_PrepareIncomingData();
            serving = true;

            do {
                DisplayLCD(LCD_LINE5, "IP: ???.???.");
                DisplayLCD(LCD_LINE6, "    ???.???");
                rxMsgId = AtLibGs_GetNetworkStatus(&network_status);
            } while (ATLIBGS_MSG_ID_OK != rxMsgId);

            sprintf(content, "IP: " _F8_ "." _F8_ ".",
                    network_status.addr.ipv4[0], network_status.addr.ipv4[1]);
            DisplayLCD(LCD_LINE5, (uint8_t *)content);
            sprintf(content, "    " _F8_ "." _F8_, network_status.addr.ipv4[2],
                    network_status.addr.ipv4[3]);
            DisplayLCD(LCD_LINE6, (uint8_t *)content);
            DisplayLCD(LCD_LINE8, "");
        } else if (!connected) {
            // Need to get a connection, wait for someone to connect
            DisplayLCD(LCD_LINE7, "Waiting...");
            if (AtLibGs_WaitForTCPConnection(&connection, 250)
                    == ATLIBGS_MSG_ID_TCP_SERVER_CONNECT) {
                // Got a connection!  What cid?
                cid = connection.cid;
                connected = true;
            }
        } else if (connected) {
            App_TemperatureReadingUpdate(G_temp_int, true);
            App_PotentiometerUpdate(G_adc_int, true);

            // Look to see if there is a message
            if ((G_receivedCount) || (AtLibGs_WaitForTCPMessage(250)
                    == ATLIBGS_MSG_ID_DATA_RX)) {
                // Got data!  Its sitting in G_received, but in a <CID> <data> format
                // We need to send it back
                AtLibGs_ParseTCPData(G_received, G_receivedCount, &msg);

                // Prepare for the next back of incoming data
                App_PrepareIncomingData();

                // Copy the data out of the receive message (its sitting in G_recieved)
                memcpy(content, msg.message, msg.numBytes);

                // Now send this back over the TCP/IP connection
                rxMsgId = AtLibGs_SendTCPData(cid, (uint8_t *)content,
                        msg.numBytes);
                if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, " Send Fail!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    continue;
                }
            } else if (MSTimerDelta(time) >= TCP_DEMO_UPDATE_INTERVAL) {
                time = MSTimerGet();
                // send temp and ADC to last received UDP client every X seconds

                DisplayLCD(LCD_LINE7, " Sending");
                sprintf(content, "Temp: %d.%d, Pot: %d.%d%%\r\n",
                        G_temp_int[0], G_temp_int[1], G_adc_int[0],
                        G_adc_int[1]);
                // send data to client
                rxMsgId = AtLibGs_SendTCPData(cid, (uint8_t *)content, strlen(
                        content));
                if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, " Send Fail!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    AtLibGs_Close(cid);
                    connected = false;
                    continue;
                } else {
                    DisplayLCD(LCD_LINE7, " ** Sent **");
                    MSTimerDelay(500);
                    DisplayLCD(LCD_LINE7, "");
                }
                DisplayLCD(LCD_LINE7, "");
            }
        }
    }
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_TCPServerDemo.c
 *-------------------------------------------------------------------------*/
