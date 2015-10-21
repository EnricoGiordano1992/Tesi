/*-------------------------------------------------------------------------*
 * File:  App_TCPClientDemo.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_TCPClientDemo 
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
#define TCP_DEMO_UPDATE_INTERVAL            1000 // ms
/* IP Address of the remote TCP Server */
#define TCP_DEMO_REMOTE_TCP_SRVR_IP         "192.168.0.9"
#define TCP_DEMO_REMOTE_TCP_SRVR_PORT       8010

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
void AppTCPSetIPMenu(ATLIBGS_NetworkStatus *pNetStatus);

/*---------------------------------------------------------------------------*
 * Routine:  App_TCPClientDemo
 *---------------------------------------------------------------------------*/
/** Connects to TCP server, then sends temperature and ADC data 
 *               to it.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_TCPClientDemo(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512];
    uint8_t cid = 0;
    static int16_t G_adc_int[2] = { 0, 0 };
    static char G_temp_int[2] = { 0, 0 };
    uint8_t remoteTcpSrvIp[20];
    
    bool connected = false; // when connected to TCP server this is true
    uint32_t time = MSTimerGet();
    ATLIBGS_TCPMessage msg;
    ATLIBGS_NetworkStatus networkStatus;
    
    AtLibGs_GetNetworkStatus(&networkStatus);
    
    AppTCPSetIPMenu(&networkStatus);

    sprintf((char*)remoteTcpSrvIp, "%d.%d.%d.%d",
            networkStatus.addr.ipv4[0],
            networkStatus.addr.ipv4[1],
            networkStatus.addr.ipv4[2],
            G_nvsettings.webprov.tcpIPClientHostIP);
    
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
                // send temp and ADC to last received TCP client every X seconds

                DisplayLCD(LCD_LINE7, " Sending");
                sprintf(content, "Temp: %d.%d, Pot: %d.%d%%\r\n", G_temp_int[0],
                        G_temp_int[1], G_adc_int[0], G_adc_int[1]);
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
        } // end else
    } // end while
}

/*---------------------------------------------------------------------------*
 * Routine:  AppTCPSetIPMenu
 *---------------------------------------------------------------------------*/
/** Receive user input from buttons to change remote TCP Server
 *              IP address on display
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void AppTCPSetIPMenu(ATLIBGS_NetworkStatus *pNetStatus)
{
    uint8_t ipString[13] = "    001     ";
    uint8_t ipAddressStr[15];
    uint8_t ipByte;
    uint8_t ipNums[3] = {0, 0, 1};
    uint8_t placeSel = 0;
    uint8_t blink = 0;
    
    // Build string to display for first three IP bytes (etc. "192.168.0.")
    sprintf((char*)ipAddressStr, "%d.%d.%d.",
            pNetStatus->addr.ipv4[0],
            pNetStatus->addr.ipv4[1],
            pNetStatus->addr.ipv4[2]);
    
    // Make sure SW2 has been released from AppMenu
    while(Switch2IsPressed())
        {}
    
    // Get previous fourth IP byte from NVsettings and format it into ipNums array
    ipByte = G_nvsettings.webprov.tcpIPClientHostIP;
    ipNums[0] = (ipByte/100);
    ipNums[1] = (ipByte%100)/10;
    ipNums[2] = (ipByte%10);
    
    // Display Remote IP Settings Menu
    DisplayLCD(LCD_LINE3, "Remote IP:  ");
    DisplayLCD(LCD_LINE4, ipAddressStr);
    DisplayLCD(LCD_LINE6, "SW1: Add 1  ");
    DisplayLCD(LCD_LINE7, "SW2: Next # ");
    DisplayLCD(LCD_LINE8, "SW3: Accept ");
    
    while(1)
    {
        // Update IP string based on ipNums
        ipString[4] = ipNums[0]+48;
        ipString[5] = ipNums[1]+48;
        ipString[6] = ipNums[2]+48;
        
        // Monitor Switches
        if(Switch1IsPressed()){
            // SW1 is pressed, increment the selected digit.
            ipNums[placeSel]++;
            if(ipNums[0]>2)
                ipNums[0] = 0;
            if(ipNums[1]>9)
                ipNums[1] = 0;
            if(ipNums[2]>9)
                ipNums[2] = 0;
            if(ipNums[0] == 2){
                if(ipNums[1] > 5)
                    ipNums[1] = 0;
                if((ipNums[1] == 5) && (ipNums[2] > 5))
                    ipNums[2] = 0;
            }
            while(Switch1IsPressed())
                {}
        }
        else if(Switch2IsPressed()){
            // SW2 is pressed, change the selected digit.
            placeSel++;
            if(placeSel > 2)
                placeSel = 0;
            while(Switch2IsPressed())
                {}
        }
        else if(Switch3IsPressed()){
            // SW3 is pressed. We're done, break out of loop.
            break;
        }
        
        if(blink > 5){
            
            ipString[placeSel+4] = ' ';
            if(blink > 10)
              blink = 0;
        }
        
        DisplayLCD(LCD_LINE5, ipString);
        MSTimerDelay(50);
        
        blink++;
    }
    
    // Clear menu and display ip address.
    DisplayLCD(LCD_LINE3, "");
    DisplayLCD(LCD_LINE4, "");
    DisplayLCD(LCD_LINE5, ipAddressStr);
    DisplayLCD(LCD_LINE6, ipString);
    DisplayLCD(LCD_LINE7, "");
    DisplayLCD(LCD_LINE8, "");
    
    // Save remote ip (fourth byte)
    G_nvsettings.webprov.tcpIPClientHostIP = (ipNums[0]*100) + (ipNums[1]*10) + ipNums[2];
    NVSettingsSave(&G_nvsettings);
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_TCPClientDemo.c
 *-------------------------------------------------------------------------*/
