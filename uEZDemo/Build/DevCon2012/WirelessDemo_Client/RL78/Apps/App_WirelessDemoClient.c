/*-------------------------------------------------------------------------*
 * File:  App_WirelessDemoClient.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_WirelessDemoClient
 * @{
 *     @brief Connects to TCP server, then sends temperature and potentiometer
 *              data to it.
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
#include <system/Switch.h>
#include <system/mstimer.h>
#include "Apps.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define TCP_DEMO_UPDATE_INTERVAL            100 // ms
#define TCP_DEMO_RSSI_INTERVAL              1000 // ms
/* IP Address of the remote TCP Server */
#define TCP_DEMO_REMOTE_TCP_SRVR_IP         "192.168.10.20"
#define TCP_DEMO_REMOTE_TCP_SRVR_PORT       23

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
void AppTCPSetIPMenu(ATLIBGS_NetworkStatus *pNetStatus);

/*---------------------------------------------------------------------------*
 * Routine:  App_WirelessDemoClient
 *---------------------------------------------------------------------------*/
/** Connects to TCP server, then sends temperature and ADC data 
 *               to it.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_WirelessDemoClient(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512];
    uint8_t cid = 0;
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    uint8_t remoteTcpSrvIp[20];
    int16_t rssi;
    
    bool connected = false; // when connected to TCP server this is true
    uint32_t time = MSTimerGet();
    uint32_t timeRSSI = time;
    ATLIBGS_TCPMessage msg;
    ATLIBGS_NetworkStatus networkStatus;
    
    AtLibGs_GetNetworkStatus(&networkStatus);
    
//    AppTCPSetIPMenu(&networkStatus);

//    sprintf((char*)remoteTcpSrvIp, "%d.%d.%d.%d",
//            networkStatus.addr.ipv4[0],
//            networkStatus.addr.ipv4[1],
//            networkStatus.addr.ipv4[2],
//            G_nvsettings.webprov.tcpIPClientHostIP);
    strcpy((char *)remoteTcpSrvIp, "192.168.10.1");
    
    App_PrepareIncomingData();
    while (1) {
        if (!AtLibGs_IsNodeAssociated()) {
            App_Connect(&G_nvsettings.webprov);
            connected = false;
        } else if (!connected) {
            DisplayLCD(LCD_LINE7, "Connecting");
            // Start a TCP client
            rxMsgId = AtLibGs_TCPClientStart((char *)remoteTcpSrvIp,
                    TCP_DEMO_REMOTE_TCP_SRVR_PORT, &cid);
            if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                DisplayLCD(LCD_LINE7, "No Connect!");
                MSTimerDelay(2000);
                DisplayLCD(LCD_LINE7, "");
                continue;
            }
            if (cid == ATLIBGS_INVALID_CID) {
                DisplayLCD(LCD_LINE7, "No CID!");
                MSTimerDelay(2000);
                DisplayLCD(LCD_LINE7, "");
                continue;
            }
            DisplayLCD(LCD_LINE7, "");
            App_PrepareIncomingData();
            connected = true;
        } else {
            App_TemperatureReadingUpdate(G_temp_int, true);
            App_PotentiometerUpdate(G_adc_int, true);

            // Look to see if there is a message
            if ((G_receivedCount) || (AtLibGs_WaitForTCPMessage(250)
                    == ATLIBGS_MSG_ID_DATA_RX)) {
                // Got data!  Its sitting in G_received, but in a <CID> <data> format
                // We need to send it back
                AtLibGs_ParseTCPData(G_received, G_receivedCount, &msg);

                // Prepare for the next batch of incoming data
                App_PrepareIncomingData();

                // Copy the data out of the receive message (its sitting in G_recieved)
                memcpy(content, msg.message, msg.numBytes);

                // Now send this back over the TCP/IP connection
#if 0
                rxMsgId = AtLibGs_SendTCPData(cid, (uint8_t *)content,
                        msg.numBytes);
                if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, " Send Fail!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    continue;
                }
#endif
            } else if (MSTimerDelta(time) >= TCP_DEMO_UPDATE_INTERVAL) {
                time = MSTimerGet();
                // send temp and ADC to last received TCP client every X seconds

                DisplayLCD(LCD_LINE7, " Sending");
                sprintf(content, ":%d%d0 %d\r\n", G_temp_int[0],
                        G_temp_int[1], G_adc_int[0]);
                ConsolePrintf("%s", content);
                // send data to server
                rxMsgId = AtLibGs_SendTCPData(cid, (uint8_t *)content, strlen(
                        content));
                if (rxMsgId != ATLIBGS_MSG_ID_OK) {
                    DisplayLCD(LCD_LINE7, " Send Fail!");
                    MSTimerDelay(2000);
                    DisplayLCD(LCD_LINE7, "");
                    connected = false;
                    continue;
                } // end if
                DisplayLCD(LCD_LINE7, "");
            } // end else if

            if (MSTimerDelta(timeRSSI) >= TCP_DEMO_RSSI_INTERVAL) {
                if (AtLibGs_GetRssi() == ATLIBGS_MSG_ID_OK) {
                    AtLibGs_ParseRssiResponse(&rssi);
                    ConsolePrintf("RSSI=%d\n", rssi);
                    P5_bit.no5 = (rssi >= -40)?0:1; // RLED1
                    P6_bit.no2 = (rssi >= -50)?0:1; // RLED2
                    P6_bit.no3 = (rssi >= -60)?0:1; // RLED3
                    P5_bit.no2 = (rssi >= -70)?0:1; // GLED1
                    P5_bit.no3 = (rssi >= -80)?0:1; // GLED2
                    P5_bit.no4 = (rssi >= -90)?0:1; // GLED3
                }
                timeRSSI = MSTimerGet();
            }
        } // end else
    } // end while
}


/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_WirelessDemoClient.c
 *-------------------------------------------------------------------------*/
