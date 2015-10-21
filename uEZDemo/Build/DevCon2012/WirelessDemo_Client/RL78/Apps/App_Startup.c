/*-------------------------------------------------------------------------*
 * File:  App_Startup.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_Startup 
 * @{
 *     @brief Startup routine for Gainspan demo. Launches other subroutines 
 * such as Firmware update mode, TCP.UDP demos, etc.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <system/platform.h>
#include <Apps/NVSettings.h>
#include <system/Switch.h>
#include <drv/Glyph/lcd.h>
#include <system/mstimer.h>
#include <system/console.h>
#include "Apps.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
NVSettings_t G_nvsettings;

/*---------------------------------------------------------------------------*
 * Routine:  App_Startup
 *---------------------------------------------------------------------------*/
/** User selects which demo to run based on button input at startup
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_Startup(void)
{
    bool sw1;
    bool sw2;
    bool sw3;

    /* At power up, load up the default settings */
    if (NVSettingsLoad(&G_nvsettings))
        NVSettingsSave(&G_nvsettings);

    /* Grab switch 1 state at startup before we init the module (which */
    /* can take longer than people want to hold the button). */
    sw1 = Switch1IsPressed();
    sw2 = Switch2IsPressed();
    sw3 = Switch3IsPressed();

    /* Show the mode immediately before initialization */
    if (sw1) {
        DisplayLCD(LCD_LINE3, "Limited AP");
    } else if (sw2) {
        DisplayLCD(LCD_LINE3, "WPS Mode");
    } else if (sw3) {
        DisplayLCD(LCD_LINE3, "Over the Air");
        DisplayLCD(LCD_LINE4, "Programming");
    }

    /* Initialize the module now that a mode is chosen (above) */
    App_InitModule();

    /* Was switch1 held? */
    if (sw1) {
        /* Yes, then go into Limited AP point */
        App_StartupLimitedAP();

        /* Now go into web provisioning mode */
        App_WebProvisioning();
    } else if (sw2) {
        App_StartWPS();

        /* Now go into web provisioning mode */
        App_WebProvisioning();
    } else if (sw3) {
        /* User wants to do over the air programming */
        App_OverTheAirProgramming();
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  App_InitModule
 *---------------------------------------------------------------------------*/
/** Setup the mode by first checking if there is a link and either
 *      report or continue to the rest of the program.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_InitModule(void)
{
    ATLIBGS_MSG_ID_E r = ATLIBGS_MSG_ID_NONE;

    DisplayLCD(LCD_LINE7, " Preparing");
    DisplayLCD(LCD_LINE8, "  Init Mod");

    /* Give the unit a little time to start up */
    /* (300 ms for GS1011 and 1000 ms for GS1500) */
    MSTimerDelay(1000);

    /* Check the link */
#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Checking link\r\n");
#endif

    /* Wait for the banner (if any) */
    MSTimerDelay(500);

    /* Clear out the buffers */
    AtLibGs_FlushRxBuffer();

    /* Send command to check */
    do {
        AtLibGs_FlushIncomingMessage();
        DisplayLCD(LCD_LINE8, "Checking...");
        r = AtLibGs_Check(GAINSPAN_AT_DEFAULT_TIMEOUT);
    } while (ATLIBGS_MSG_ID_OK != r);

    /* Send command to DISABLE echo */
    do {
        DisplayLCD(LCD_LINE8, "Echo Off...");
        r = AtLibGs_SetEcho(ATLIBGS_DISABLE);
    } while (ATLIBGS_MSG_ID_OK != r);

    /* Done */
    DisplayLCD(LCD_LINE7, "");
    DisplayLCD(LCD_LINE8, "");
}

/*---------------------------------------------------------------------------*
 * Routine:  App_StartupLimitedAP
 *---------------------------------------------------------------------------*/
/** Put the unit into a limited AP mode using the configuration in the
 *      default LimitedAP settings. Units will then be able to connect to it 
 *      as an access point and change it's settings using web provisioning
 *     (if web provisioning is then enabled)
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_StartupLimitedAP(void)
{
    ATLIBGS_MSG_ID_E r;

    DisplayLCD(LCD_LINE3, "Limited AP");
    DisplayLCD(LCD_LINE4, ATLIBGS_LIMITED_AP_SSID);

#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Starting Limited AP: %s\n", ATLIBGS_LIMITED_AP_SSID);
#endif

    /* Try to disassociate if not already associated */
    AtLibGs_DisAssoc();
    while (1) {
        DisplayLCD(LCD_LINE6, " Setting up");
        r = AtLibGs_Mode(ATLIBGS_STATIONMODE_LIMITED_AP);
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "Bad Mode!");
            MSTimerDelay(2000);
            continue;
        }
        r = AtLibGs_IPSet(ATLIBGS_LIMITED_AP_IP, ATLIBGS_LIMITED_AP_MASK,
                ATLIBGS_LIMITED_AP_GATEWAY);
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "Bad IP!");
            MSTimerDelay(2000);
            continue;
        }
        AtLibGs_DisableDHCPServer();
        r = AtLibGs_EnableDHCPServer();
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "Bad DHCPSrv!");
            MSTimerDelay(2000);
            continue;
        }
        r = AtLibGs_Assoc(ATLIBGS_LIMITED_AP_SSID, 0,
                ATLIBGS_LIMITED_AP_CHANNEL);
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "AP Failed!");
            MSTimerDelay(2000);
            continue;
        }
        break;
    }
    DisplayLCD(LCD_LINE6, "");
#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Limited AP Started\n");
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  App_WebProvisioning
 *---------------------------------------------------------------------------*/
/** Put the unit into web provisioning mode and wait for the user to
 *      connect with a web browser, change the settings, and click Save.
 *      The settings will then be parsed by the AtLibGs library and
 *      get saved into the nv settings.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_WebProvisioning(void)
{
    ATLIBGS_MSG_ID_E r;
    ATLIBGS_NetworkStatus network_status;
    char text[32];
    
    /* Before going into web provisioning, provide DNS to give a link. */
    /* The user can then go to http://webprov.gainspan.com/gsclient.html to get */
    /* access to the web provisioning screen. */
    while (1) {
        AtLibGs_DisableDNSServer();
        r = AtLibGs_EnableDNSServer("webprov.gainspan.com");
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "Bad DNS!");
            MSTimerDelay(2000);
            continue;
        }
        // Program GainSpan logo, CAN ONLY BE DONE ONCE!!!!!!!!!!!!!!
        // AtLibGs_WebLogoAdd(1776, logoForProgramming);
        
        r = AtLibGs_WebProv("admin", "admin");
        if (r != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE6, "Bad WebProv!");
            MSTimerDelay(2000);
            continue;
        }
        break;
    }
    DisplayLCD(LCD_LINE6, "WebProv ON");

    do {
        DisplayLCD(LCD_LINE7, "IP: ???.???.");
        DisplayLCD(LCD_LINE8, "    ???.???");
        r = AtLibGs_GetNetworkStatus(&network_status);
    } while (ATLIBGS_MSG_ID_OK != r);

    sprintf(text, "IP: " _F8_ "." _F8_ ".",
            network_status.addr.ipv4[0], network_status.addr.ipv4[1]);
    DisplayLCD(LCD_LINE7, (uint8_t *)text);
    sprintf(text, "    " _F8_ "." _F8_, network_status.addr.ipv4[2],
            network_status.addr.ipv4[3]);
    DisplayLCD(LCD_LINE8, (uint8_t *)text);

#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Web Provisioning ON\n");
#endif

    /* Now wait for a list of responses until we get a blank line */
#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Waiting for web provisioning response...\n");
#endif
    AtLibGs_GetWebProvSettings(&G_nvsettings.webprov, 0);

    /* Save the above settings */
    NVSettingsSave(&G_nvsettings);
#ifdef ATLIBGS_DEBUG_ENABLE
    ConsolePrintf("Web provisioning complete.\n");
#endif

    DisplayLCD(LCD_LINE6, "WebProv Done");
    DisplayLCD(LCD_LINE7, "");
    DisplayLCD(LCD_LINE8, "Press RESET");
    while (1)
        {}
}

/*---------------------------------------------------------------------------*
 * Routine:  App_StartWPS
 *---------------------------------------------------------------------------*/
/** Put the unit into WPS pushbutton mode. After pushing the button on the 
 *      AP the unit will retrieve its SSID and pass phrase, then connect to it
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_StartWPS(void)
{
    ATLIBGS_MSG_ID_E rxMsgId;
    ATLIBGS_NetworkStatus network;
    AtLibGs_WPSResult result;
    char text[20];
    
    while (1) {
        // Ensure we are not connected to any network (from previous runs)
        AtLibGs_DisAssoc();

        /* Pushbutton WPS demo */
        /* Use Wi-Fi Protected Setup (WPS) FW */
        /* turn on DHCP client */
        AtLibGs_DHCPSet(1);

        /* set to connect to AP mode */
        AtLibGs_Mode(ATLIBGS_STATIONMODE_INFRASTRUCTURE);
        DisplayLCD(LCD_LINE5, "  Push the  ");
        DisplayLCD(LCD_LINE6, "button on AP");

        /* push the button on the AP so the GS module can connect */
        while (AtLibGs_StartWPSPUSH(&result) != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE8, " Not found!");
            MSTimerDelay(1000);
            DisplayLCD(LCD_LINE8, " Retrying...");
            MSTimerDelay(1000);
            DisplayLCD(LCD_LINE8, "");
        }

        /* Connect to AP (found from pushbutton) after setting pass phrase */
        AtLibGs_SetPassPhrase(result.password);
        AtLibGs_Assoc(result.ssid, "", result.channel);

        rxMsgId = AtLibGs_GetNetworkStatus(&network);
        if (rxMsgId != ATLIBGS_MSG_ID_OK) {
            DisplayLCD(LCD_LINE8, "Bad Network!");
            MSTimerDelay(2000);
            DisplayLCD(LCD_LINE8, "");
            continue;
        } else {
            strncpy(text, network.ssid, 12);
            DisplayLCD(LCD_LINE4, (const uint8_t *)text);
            
            // set the settings obtained from WPS
            strcpy(G_nvsettings.webprov.ssid, result.ssid);
            strcpy(G_nvsettings.webprov.password, result.password);
            G_nvsettings.webprov.channel = result.channel;
            
            // Save the above settings
            NVSettingsSave(&G_nvsettings);
        }
        break;
    }

    DisplayLCD(LCD_LINE5, "");
    DisplayLCD(LCD_LINE6, "");
}

/*---------------------------------------------------------------------------*
 * Routine:  App_OverTheAirProgramming
 *---------------------------------------------------------------------------*/
/** Put the unit into over the air programming mode after connecting to an
 *      access point in infrastructure mode.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_OverTheAirProgramming(void)
{
    char buf[12] = "           ";
    int i;

    /* OTA Firmware update demo */
    /* Connect to AP first, then do FW upgrade with PC application */
    AtLibGs_Mode(ATLIBGS_STATIONMODE_INFRASTRUCTURE); /* set to connect to AP mode */
    AtLibGs_DHCPSet(1); /* turn on DHCP client */
    AtLibGs_SetPassPhrase(ATLIBGS_AP_SEC_PSK);
    AtLibGs_Assoc(ATLIBGS_AP_SSID, 0, 0);
    AtLibGs_EnableRadio(1); /* radio always on */

    DisplayLCD(LCD_LINE6, "Start Update");
    DisplayLCD(LCD_LINE7, "on server IP");
    /* print last 12 (of 15) char of server IP address */

    for (i = 0; i < 12; i++) {
        buf[i] = (char)ATLIBGS_FWUPGRADE_SERVER[i + 3];
    }
    DisplayLCD(LCD_LINE8, (uint8_t *)buf);

    /* start FW update on remote port 8010 with 100 retries */
    AtLibGs_FWUpgrade(ATLIBGS_FWUPGRADE_SERVER, 8010, 3000, "100");
}

/*---------------------------------------------------------------------------*
 * Routine:  App_HTTPOverTheAirProgramming
 *---------------------------------------------------------------------------*/
/** After connecting to an access point in infrastructure mode, update the
 *      firmware. This uses the HTTP method to download the firmware files 
 *      from an HTTP server.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void App_HTTPOverTheAirProgramming(void)
{
  int urlStringLength;
  char fwUpdateUrl[60] = "http://"; // url of FW update files
  char fwUpdate_WIRELESS[] = "/WFW-REL-2_0_44.bin"; // wireless firmware
  // Strings for each application firmware type file name
  // Note the actual firmware has 2 parts: filename1.bin and filename2.bin
  const char *appFirmwares_str[]={ "/s2w-wps-2_3_5-app",
                                   "/s2w-web-2_3_5-app",
                                   "/s2w-webspi-2_3_5-app",
                                   "/s2w-secureweb-2_3_5-app",
                                   "/s2w-securewebspi-2_3_5-app",
                                   "/s2w-eap-2_3_5-app",
                                   "/s2w-rftest-2_3_5-app"};
  // enumerated names for each firmware type
  enum appFirmwares { fwUpdate_WPS,        // Wi-Fi Protected Setup (WPS) UART
                      fwUpdate_WSPUART,    // Web Server Provisioning UART
                      fwUpdate_WSPSPI,     // Web Server Provisioning SPI Mode 0
                      fwUpdate_SECWSPUART, // Secure Web Server Provisioning UART
                      fwUpdate_SECWSPSPI,  // Secure Web Server Provisioning SPI Mode 0
                      fwUpdate_EAP,        // Enterprise Security (EAP) UART
                      fwUpdate_RFTest};    // RF Test UART 
 
  // Change this to change which FW is updated to.
  const char *appFWToUse = appFirmwares_str[fwUpdate_WPS]; 
 
  strcat(fwUpdateUrl,ATLIBGS_FWUPGRADE_SERVER); // add IP address to URL
  urlStringLength = strlen(fwUpdateUrl); // length of string before filename
  
  DisplayLCD(LCD_LINE2, "    HTTP    ");
  DisplayLCD(LCD_LINE3, "  Firmware  ");
  DisplayLCD(LCD_LINE4, "   Update   ");
  
  AtLibGs_SetEcho(1); // turn echo on
  AtLibGs_Version(); // display current software version
  AtLibGs_DHCPSet(1); // turn on DHCP client
  AtLibGs_EnableRadio(1); // radio always on
  
  // Connect to AP
  AtLibGs_Mode(ATLIBGS_STATIONMODE_INFRASTRUCTURE); // infrastructure mode
  AtLibGs_SetPassPhrase(ATLIBGS_AP_SEC_PSK); // passphrase
  AtLibGs_Assoc(ATLIBGS_AP_SSID, 0, 0); // connect to SSID
  
  // http settings for FW upgrade
  AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_USER_A,"Mozilla/4.0");
  AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_TYPE,"application/x-www-form-urlencoded");
  AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_HOST,ATLIBGS_FWUPGRADE_SERVER);
  AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CONN,"Keep-Alive");
  // server and port settings for FW upgrade
  AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_SERVERIP,ATLIBGS_FWUPGRADE_SERVER); // http server url
  AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_SERVERPORT,"80"); // server port number
  
  // FW update strings look like: http://192.168.0.1/WFW-REL-2_0_44.bin
  // prepare WLAN FWUpdate string
  strcat(fwUpdateUrl,fwUpdate_WIRELESS); // add filename to string
  AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_WLAN,fwUpdateUrl); // set filename for WLAN firmware
  
  // prepare app1 FWUpdate string
  fwUpdateUrl[urlStringLength] = '\0'; // remove filename from string
  strcat(fwUpdateUrl,appFWToUse); // add new filename to string
  urlStringLength = strlen(fwUpdateUrl); // length of string before 1.bin
  strcat(fwUpdateUrl,"1.bin"); // add file extension
  AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_APP0,fwUpdateUrl); // set filename for APP1 firmware
  
  // prepare app2 FWUpdate string
  fwUpdateUrl[urlStringLength] = '\0'; // remove 1.bin from string
  strcat(fwUpdateUrl,"2.bin"); // add file extension
  AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_APP1,fwUpdateUrl); // set filename for APP2 firmware

  // start HTTP firmware update, will take about 2 minutes to finish
  AtLibGs_StartFirmwareUpdate(ATLIBGS_ALL_BINS);
  // should return "APP Reset External Flash FW-UP-SUCCESS"
  // After 2 minutes it will accept new commands.
  AtLibGs_Version(); // display new software version
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_Startup.c
 *-------------------------------------------------------------------------*/
