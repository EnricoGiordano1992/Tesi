/*-------------------------------------------------------------------------*
 * File:  TestLastCommands.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  TestLastCommands
 * @{
 *     @brief Tests that don't fall into the other catagories
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
#include <system/flash.h>
#include <CmdLib/GainSpan_SPI.h>
#include "Tests.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#if 0
    #define TEST_START(name) \
        DisplayLCD(LCD_LINE2, name)
#else
    #define TEST_START(name) \
        DisplayLCD(LCD_LINE2, name); \
        ConsolePrintf("Testing: %s\n", name)
#endif

#define TEST_END()
#define TEST_ASSERT(cond, msg)  \
    if (!(cond)) \
        { ConsolePrintf("Test Failed! : " msg "\r\n"); G_numFailures++; }
#define TEST_ASSERT_STRING(v, string)  \
    if (strcmp((const char *)v, (const char *)string)!=0) \
      { ConsolePrintf("Test Failed! : \"%s\" != \"" string "\"\r\n", v); G_numFailures++; }
#define TEST_ASSERT_STRINGS_EQUAL(string1, string2, msg)  \
    if (strcmp((const char *)string1, (const char *)string2)!=0) \
      { ConsolePrintf("Test Failed! (%s:%d): %s\n  \"%s\" != \"%s\"\r\n", __FILE__, __LINE__, msg, string1, string2); G_numFailures++; }
#define TEST_ASSERT_STRINGS_NOT_EQUAL(string1, string2, msg)  \
    if (strcmp((const char *)string1, (const char *)string2)==0) \
      { ConsolePrintf("Test Failed! (%s:%d): %s\n  \"%s\" == \"%s\"\r\n", __FILE__, __LINE__, msg, string1, string2); G_numFailures++; }
#define TEST_ASSERT_VALUE(v, number) \
    if (v != number) \
      { ConsolePrintf("Test Failed! : " #v " (%d) != " #number "\r\n", v); G_numFailures++; }
#define TEST_ASSERT_OK(r, msg) \
        if (r != ATLIBGS_MSG_ID_OK) \
          { ConsolePrintf("Test Failed! : %s\n", msg); G_numFailures++; }
#define TEST_ASSERT_NOT_OK(r, msg) \
        if (r == ATLIBGS_MSG_ID_OK) \
          { ConsolePrintf("Test Failed! : %s\n", msg); G_numFailures++; }

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
int G_numFailures = 0;

/*---------------------------------------------------------------------------*
 * Routine:  TestEchoOff
 *---------------------------------------------------------------------------*/
/** Test turning echo off
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestEchoOff(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;

    /* Send command to DISABLE echo */
    do {
        DisplayLCD(LCD_LINE8, "Echo Off...");
        rxMsgId = AtLibGs_SetEcho(ATLIBGS_DISABLE);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
}

/*---------------------------------------------------------------------------*
 * Routine:  TestEchoOn
 *---------------------------------------------------------------------------*/
/** Test turning echo on
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestEchoOn(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;

    /* Send command to DISABLE echo */
    do {
        DisplayLCD(LCD_LINE8, "Echo On...");
        rxMsgId = AtLibGs_SetEcho(ATLIBGS_ENABLE);
    } while (ATLIBGS_MSG_ID_OK != rxMsgId);
}

/*---------------------------------------------------------------------------*
 * Routine:  TestGetInfo
 *---------------------------------------------------------------------------*/
/** test getting various info from GainSpan module
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestGetInfo(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    static char content[512];

    TEST_START("GetInfo");

    rxMsgId = AtLibGs_GetInfo(ATLIBGS_ID_INFO_OEM, content, sizeof(content));
    TEST_ASSERT(rxMsgId==ATLIBGS_MSG_ID_OK, "ATI0 did not respond with OK");
    TEST_ASSERT_STRING(content, "GainSpan");

    rxMsgId = AtLibGs_GetInfo(ATLIBGS_ID_INFO_HARDWARE_VERSION, content, sizeof(content));
    TEST_ASSERT(rxMsgId==ATLIBGS_MSG_ID_OK, "ATI0 did not respond with OK");
    TEST_ASSERT_STRING(content, "GS1011A1");

    rxMsgId = AtLibGs_GetInfo(ATLIBGS_ID_INFO_SOFTWARE_VERSION, content, sizeof(content));
    TEST_ASSERT(rxMsgId==ATLIBGS_MSG_ID_OK, "ATI0 did not respond with OK");
    TEST_ASSERT_STRING(content, "2.3.5");

    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestScanTimes
 *---------------------------------------------------------------------------*/
/** test setting scan times
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestScanTimes(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    uint16_t min, max;

    TEST_START("ScanTimes");
    rxMsgId = AtLibGs_SetScanTimes(100, 100);
    TEST_ASSERT_OK(rxMsgId, "Failed to set scan times");

    rxMsgId = AtLibGs_GetScanTimes(&min, &max);
    TEST_ASSERT_OK(rxMsgId, "Failed to get scan times");
    TEST_ASSERT_VALUE(min, 100);
    TEST_ASSERT_VALUE(max, 100);

    rxMsgId = AtLibGs_SetScanTimes(150, 150);
    TEST_ASSERT_OK(rxMsgId, "Failed to set scan times");

    rxMsgId = AtLibGs_GetScanTimes(&min, &max);
    TEST_ASSERT_OK(rxMsgId, "Failed to get scan times");
    TEST_ASSERT_VALUE(min, 150);
    TEST_ASSERT_VALUE(max, 150);
    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestSetDefaultProfile
 *---------------------------------------------------------------------------*/
/** test setting a default profile
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestSetDefaultProfile(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    TEST_START("SetDefProf");

    rxMsgId = AtLibGs_SetDefaultProfile(1);
    TEST_ASSERT_OK(rxMsgId, "Could not set profile!");
    rxMsgId = AtLibGs_SetDefaultProfile(2);
    TEST_ASSERT_NOT_OK(rxMsgId, "Illegal profile set?");
    rxMsgId = AtLibGs_SetDefaultProfile(0);
    TEST_ASSERT_OK(rxMsgId, "Could not set profile!");

    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestGetProfile
 *---------------------------------------------------------------------------*/
/** Test retrieving stored profile
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestGetProfile(void)
{
    ATLIBGS_MSG_ID_E rxMsgId = ATLIBGS_MSG_ID_NONE;
    char content[512];

    TEST_START("GetProfile");
    memset(content, 0, sizeof(content));
    rxMsgId = AtLibGs_GetProfile(ATLIBGS_PROFILE_STORED_1, content,
            sizeof(content) - 1);
    TEST_ASSERT_OK(rxMsgId, "Could not get profile!");
    TEST_ASSERT(strlen(content)>128, "Missing profile data!");
    TEST_ASSERT(strstr(content, "+NAUTO"), "Missing NAUTO!");
    TEST_ASSERT(strstr(content, "+WRXPS"), "Missing WRXPS!");
    TEST_ASSERT(strstr(content, "+SSID"), "Missing SSID!");

    memset(content, 0, sizeof(content));
    rxMsgId = AtLibGs_GetProfile(ATLIBGS_PROFILE_STORED_0, content,
            sizeof(content) - 1);
    TEST_ASSERT_OK(rxMsgId, "Could not get profile!");
    TEST_ASSERT(strlen(content)>128, "Missing profile data!");

    memset(content, 0, sizeof(content));
    rxMsgId = AtLibGs_GetProfile(ATLIBGS_PROFILE_ACTIVE, content,
            sizeof(content) - 1);
    TEST_ASSERT_OK(rxMsgId, "Could not get profile!");
    TEST_ASSERT(strlen(content)>128, "Missing profile data!");

//    ConsolePrintf("Profile: %s\n", content);
    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestGetSetMAC
 *---------------------------------------------------------------------------*/
/** Test retrieving and setting the MAC address
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestGetSetMAC(void)
{
    char mac1[30];
    char mac2[30];
    char mac3[30];
    char mac4[30];

    TEST_START("GetSetMAC");

    // Reset MAC address (only uncomment if needing to reset to a specific value)
    //AtLibGs_SetMAC("00:1D:C9:10:3D:7C");
    //AtLibGs_SetMAC2("00:1D:C9:10:3D:7C");

    memset(mac1, 0, sizeof(mac1));
    memset(mac2, 0, sizeof(mac2));

    // Store the original mac addresses
    TEST_ASSERT_OK(AtLibGs_GetMAC(mac1), "Could not get MAC1");
    TEST_ASSERT_OK(AtLibGs_GetMAC2(mac2), "Could not get MAC2");

    //ConsolePrintf("mac1=%s, mac2=%s\n", mac1, mac2);
    strcpy(mac3, mac1);
    mac3[0] ^= 1;
    TEST_ASSERT_OK(AtLibGs_SetMAC(mac3), "Could not set MAC1");
    TEST_ASSERT_OK(AtLibGs_GetMAC(mac4), "Could not get MAC1");
    TEST_ASSERT_STRINGS_EQUAL(mac3, mac4, "MAC1 does not equal last stored MAC1");
    TEST_ASSERT_OK(AtLibGs_GetMAC2(mac4), "Could not get MAC2");
    //TEST_ASSERT_STRINGS_EQUAL(mac2, mac4, "MAC2 changed!"); // mac1 and mac2 are tied together!

    TEST_ASSERT_OK(AtLibGs_SetMAC(mac1), "Could not set MAC1");
    TEST_ASSERT_OK(AtLibGs_SetMAC2(mac2), "Could not set MAC2");

    strcpy(mac3, mac2);
    mac3[0] ^= 1;
    TEST_ASSERT_OK(AtLibGs_SetMAC2(mac3), "Could not set MAC2");
    TEST_ASSERT_OK(AtLibGs_GetMAC2(mac4), "Could not get MAC2");
    TEST_ASSERT_STRINGS_EQUAL(mac3, mac4, "MAC2 does not equal last stored MAC2");
    TEST_ASSERT_OK(AtLibGs_GetMAC(mac4), "Could not get MAC1");
    // TEST_ASSERT_STRINGS_EQUAL(mac1, mac4, "MAC1 changed!"); // mac1 and mac2 are tied together!

    // Put the MACs back
    TEST_ASSERT_OK(AtLibGs_SetMAC(mac1), "Could not set MAC1");
    TEST_ASSERT_OK(AtLibGs_SetMAC2(mac2), "Could not set MAC2");
    TEST_ASSERT_OK(AtLibGs_GetMAC(mac3), "Could not get MAC1");
    TEST_ASSERT_STRINGS_EQUAL(mac1, mac3, "MAC1 changed!");
    TEST_ASSERT_OK(AtLibGs_GetMAC2(mac3), "Could not get MAC2");
    TEST_ASSERT_STRINGS_EQUAL(mac2, mac3, "MAC2 changed!");

    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestRegulatoryDomain
 *---------------------------------------------------------------------------*/
/** Test the regulatory domain change feature.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestRegulatoryDomain(void)
{
    ATLIBGS_REGDOMAIN_E domain=ATLIBGS_REGDOMAIN_UNKNOWN;

    TEST_START("RegDomain");

    // Store the original mac addresses
    TEST_ASSERT_OK(AtLibGs_SetRegulatoryDomain(ATLIBGS_REGDOMAIN_TELEC), "Could not set regulatory domain");
    TEST_ASSERT_OK(AtLibGs_GetRegulatoryDomain(&domain), "Could not get regulatory domain");
    TEST_ASSERT_VALUE(domain, ATLIBGS_REGDOMAIN_TELEC);
    TEST_ASSERT_OK(AtLibGs_SetRegulatoryDomain(ATLIBGS_REGDOMAIN_ETSI), "Could not set regulatory domain");
    TEST_ASSERT_OK(AtLibGs_GetRegulatoryDomain(&domain), "Could not get regulatory domain");
    TEST_ASSERT_VALUE(domain, ATLIBGS_REGDOMAIN_ETSI);
    TEST_ASSERT_OK(AtLibGs_SetRegulatoryDomain(ATLIBGS_REGDOMAIN_FCC), "Could not set regulatory domain");
    TEST_ASSERT_OK(AtLibGs_GetRegulatoryDomain(&domain), "Could not get regulatory domain");
    TEST_ASSERT_VALUE(domain, ATLIBGS_REGDOMAIN_FCC);

    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestNetworkScan
 *---------------------------------------------------------------------------*/
/** Test the Network scan feature of the GainSpan module.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestNetworkScan(void)
{
    ATLIBGS_NetworkScanEntry entries[10];
    ATLIBGS_NetworkScanEntry first;
    uint8_t numEntries;

    TEST_START("Network scan");
    TEST_ASSERT_OK(AtLibGs_NetworkScan(0, 0, 0, entries, 10, &numEntries), "Could not scan for networks!");
    ConsolePrintf("Num entries: %d\n", numEntries);
    TEST_ASSERT(numEntries > 0, "No networks found!");
    first = entries[0];
    memset(entries, 0, sizeof(entries));
    TEST_ASSERT(AtLibGs_NetworkScan(first.ssid, 0, 0, entries, 10, &numEntries), "Could not scan for networks!");
    TEST_ASSERT(numEntries > 0, "No networks found!");
    TEST_ASSERT_STRINGS_EQUAL(first.bssid, entries[0].bssid, "Mismatch BSSID");
    TEST_ASSERT_STRINGS_EQUAL(first.ssid, entries[0].ssid, "Mismatch BSSID");
    TEST_ASSERT_VALUE(first.channel, entries[0].channel);
    TEST_ASSERT_VALUE(first.security, entries[0].security);
    TEST_ASSERT(first.signal < 0, "Signal should be negative number!");
    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestNewCommands
 *---------------------------------------------------------------------------*/
/** Commands that haven't been put into their own category yet.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestNewCommands(void)
{
    // Disassociate from the network
    TEST_START("Disassociate from AP");
   // TEST_ASSERT_OK(AtLibGs_DisAssoc2(), "Could not disassociate.");
    TEST_END();
    
    // Gets the transmit rate as ASCII    
    TEST_START("Get Transmit Rate");
    TEST_ASSERT_OK(AtLibGs_GetTransmitRate(), "Could not get transmit rate");
    TEST_END();
    
    // Get Client Info
    TEST_START("Get Client Info");  
    //TEST_ASSERT_OK(AtLibGs_GetClientInfo(), "Could not get client info");
    TEST_END();

    // Set the Authentication Mode    
    TEST_START("Set Authentication mode");
    //TEST_ASSERT_OK(AtLibGs_SetAuthentictionMode(ATLIBGS_AUTHMODE_OPEN_WEP), "Could not set authentication mode");
    //TEST_ASSERT_OK(AtLibGs_SetAuthentictionMode(ATLIBGS_AUTHMODE_SHARED_WEP), "Could not set authentication mode");
    TEST_ASSERT_OK(AtLibGs_SetAuthentictionMode(ATLIBGS_AUTHMODE_NONE_WPA), "Could not set authentication mode");
    TEST_END();
    
    // Set the WEP Keys
    char shorkey[11] = "AB235476FE\0";    
    char longkey[27] = "43975943275349BDF49327543F\0";
    
    TEST_START("Set WEP Keys 1-4");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY1,shorkey), "Could not set short WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY1,longkey), "Could not set long WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY2,shorkey), "Could not set short WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY2,longkey), "Could not set long WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY3,shorkey), "Could not set short WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY3,longkey), "Could not set long WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY4,shorkey), "Could not set short WEP key");
    TEST_ASSERT_OK(AtLibGs_SetWEPKey(ATLIBGS_WEPKEY4,longkey), "Could not set long WEP key");
    TEST_END();
    
    // Set the WPA Key
    char wpakey[66] = "0A010B03040506070C80900010203E4F5060708090D01020304050D0A08090DF\0";
    
    TEST_START("Set WPA Key");
    TEST_ASSERT_OK(AtLibGs_SetWPAKey(wpakey), "Could not set WPA key");
    TEST_END();
    
    TEST_START("Set Listen Beacon Interval");
//    TEST_ASSERT_OK(AtLibGs_SetListenBeaconInterval(false,0), "Could not set Listen Beacon Interval");
//    TEST_ASSERT_OK(AtLibGs_SetListenBeaconInterval(true,65535), "Could not set Listen Beacon Interval");
//    TEST_ASSERT_OK(AtLibGs_SetListenBeaconInterval(true,10), "Could not set Listen Beacon Interval");
    TEST_END();
    
    
    TEST_START("Set Transmit Rate");
//    TEST_ASSERT_OK(AtLibGs_SetTransmitRate(ATLIBGS_TR1MBPS), "Could not set transmit rate");
//    TEST_ASSERT_OK(AtLibGs_SetTransmitRate(ATLIBGS_TR2MBPS), "Could not set transmit rate");
//   TEST_ASSERT_OK(AtLibGs_SetTransmitRate(ATLIBGS_TR5p5MBPS), "Could not set transmit rate");
//    TEST_ASSERT_OK(AtLibGs_SetTransmitRate(ATLIBGS_TR11MBPS), "Could not set transmit rate");
    TEST_ASSERT_OK(AtLibGs_SetTransmitRate(ATLIBGS_TRAUTO), "Could not set transmit rate");
    TEST_END();

    TEST_START("Testing DHCP Server");
//    TEST_ASSERT_OK(AtLibGs_EnableDHCPServer(), "Could not enable DHCP Server");
//    TEST_ASSERT_OK(AtLibGs_DisableDHCPServer(), "Could not disable DHCP Server");
    TEST_END();
 
    // Set the DNS server
    // Will fail if try to set same server twice or disable twice in a row
  //  char server[9] = "4.4.2.2\0";
    TEST_START("Testing DNS Server");
//    TEST_ASSERT_OK(AtLibGs_EnableDNSServer(server), "Could not enable DNS Server");
//    TEST_ASSERT_OK(AtLibGs_DisableDNSServer(), "Could not disable DNS Server");
    TEST_END();
    
      
    // EAP Security
//    char user[10] = "username\0";
//    char pass[10] = "password\0";
    TEST_START("Testing EAP Security");
//    TEST_ASSERT_OK(AtLibGs_SetEAPConfiguration(ATLIBGS_OAEAPTTLS,ATLIBGS_IAEAPGTC,user,pass), "Could not set EAP Sec");
    TEST_END();
    
    
    // Test setting various security modes
    TEST_START("Testing Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMOPEN), "Could not set Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMWEP), "Could not set Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMWPAPSK), "Could not set Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMWPA2PSK), "Could not set Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMWPAE), "Could not set Security");
//    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMWPA2E), "Could not set Security");
    TEST_ASSERT_OK(AtLibGs_SetSecurity(ATLIBGS_SMAUTO), "Could not set Security");
    TEST_END(); 
    
    
    // SSL Cert
//    char name[14] = "certificate1\0";
    TEST_START("Testing SSL Cert");
//    TEST_ASSERT_OK(AtLibGs_AddSSLCertificate(name, 1, 256, 1), "Could not set SSL Cert");
//    TEST_ASSERT_OK(AtLibGs_DeleteSSLCertificate(name), "Could not clear SSL cert");
    TEST_END();
    
    // SSL Cert Open/Close
    uint16_t cid = 321;
    TEST_START("Testing SSL Con");
//    TEST_ASSERT_OK(AtLibGs_AddSSLCertificate(name, 1, 256, 1), "Could not set SSL Cert");
//   TEST_ASSERT_OK(AtLibGs_SSLOpen(cid, name), "Could not open SSL con");
//    TEST_ASSERT_OK(AtLibGs_SSLClose(cid), "Could not close SSL con");
    TEST_END();
    
    TEST_START("Testing HTTP");
/*  TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_AUTH,name), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_AUTH), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CONN,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_CONN), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_ENC,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_CON_ENC), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_LENGTH,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_CON_RANGE), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_CON_TYPE,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_DATE), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_EXPIRES,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_EXPIRES), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_FROM,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_FROM), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_HOST,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_HOST), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_IF_MODIF_SIN,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_IF_MODIF_SIN), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_LAST_MODIF,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_LAST_MODIF), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_LOCATION,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_LOCATION), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_PRAGMA,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_PRAGMA), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_RANGE,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_RANGE), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_REF,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_REF), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_SERVER,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_SERVER), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_TRANS_ENC,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_TRANS_ENC), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_USER_A,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_USER_A), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_HE_WWW_AUTH,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_HE_WWW_AUTH), "Could not delete HTTP config");
    TEST_ASSERT_OK(AtLibGs_HTTPConf(ATLIBGS_HTTP_REQ_URL,), "Could not conf HTTP client");
    TEST_ASSERT_OK(AtLibGs_HTTPConfDel(ATLIBGS_HTTP_REQ_URL), "Could not delete HTTP config");
 */ 
    TEST_ASSERT_OK(AtLibGs_HTTPClose(cid), "Could not close HTTP con");
    TEST_END();
    
    // Raw ethernet frame test
    TEST_START("Testing Raw Eth Frame Conf");
//    TEST_ASSERT_OK(AtLibGs_RawETHFrameConf(ATLIBGS_RAWETHNONSNAP), "Could not set raw eth frame conf");
//    TEST_ASSERT_OK(AtLibGs_RawETHFrameConf(ATLIBGS_RAWETHENABLEALL), "Could not set raw eth frame conf");
//    TEST_ASSERT_OK(AtLibGs_RawETHFrameConf(ATLIBGS_RAWETHDIS), "Could not set raw eth frame conf");
    TEST_END();
    
    TEST_START("Testing bulk trans mode");
//   TEST_ASSERT_OK(AtLibGs_BulkDataTrans(true), "Could not enable bulk trans mode");
//   TEST_ASSERT_OK(AtLibGs_BulkDataTrans(false), "Could not disable bulk trans mode");
    TEST_END();
    
    TEST_START("Testing battery");
    TEST_ASSERT_OK(AtLibGs_SetBatteryCheckFreq(1), "Could not set batt check freq");
    TEST_ASSERT_OK(AtLibGs_SetBatteryCheckFreq(100), "Could not set batt check freq");
    TEST_ASSERT_OK(AtLibGs_GetBatteryValue(), "Could not get battery level");
    TEST_ASSERT_OK(AtLibGs_BatterySetWarnLevel(2000,100,1500), "Could not set battery warn level");
    TEST_END();
    
    /*
    TEST_START("Deep Sleep Test");
    TEST_ASSERT_OK(AtLibGs_EnterDeepSleep(5000), "Could not set batt check freq");
    TEST_END();
    */
    
    TEST_START("Association Power Save Mode test");
    TEST_ASSERT_OK(AtLibGs_AssocPowerSaveMode(ATLIBGS_ASPWMD_ACTIVE_ON), "Could not set Assoc PW mode");
//  TEST_ASSERT_OK(AtLibGs_AssocPowerSaveMode(ATLIBGS_ASPWMD_MODE_1), "Could not set Assoc PW mode");
//  TEST_ASSERT_OK(AtLibGs_AssocPowerSaveMode(ATLIBGS_ASPWMD_MODE_2), "Could not set Assoc PW mode");
//  TEST_ASSERT_OK(AtLibGs_AssocPowerSaveMode(ATLIBGS_ASPWMD_MODE_3), "Could not set Assoc PW mode");
    TEST_END();
    
    TEST_START("Transmit power level test");
//   TEST_ASSERT_OK(AtLibGs_TransmitPower(15), "Could not set transmit power 15");
//   TEST_ASSERT_OK(AtLibGs_TransmitPower(7), "Could not set transmit power 7");
//    TEST_ASSERT_OK(AtLibGs_TransmitPower(0), "Could not set transmit power 0");
//    TEST_ASSERT_OK(AtLibGs_TransmitPower(2), "Could not set transmit power 2");
    TEST_END();
    
    TEST_START("Auto connect test");
//  TEST_ASSERT_OK(AtLibGs_ReturnAutoConnect(), "Could not return to auto con");
//  TEST_ASSERT_OK(AtLibGs_StartAutoConnect(), "Could not start auto con");
//  TEST_ASSERT_OK(AtLibGs_ConfigAPAutoConnect(), "Could not auto connect to config AP");
    TEST_END();
    
    TEST_START("Web Provisioning test");
//  TEST_ASSERT_OK(AtLibGs_WebProv(user,pass), "Could not set web prov");
//  TEST_ASSERT_OK(AtLibGs_WebLogoAdd(1788), "Could not add web logo");
    TEST_END();
    
    
    TEST_START("RF tests");
//  TEST_ASSERT_OK(AtLibGs_RFAsyncFrameRX(1), "Could not enable async frame RX");
//  TEST_ASSERT_OK(AtLibGs_RFStop(), "Could not stop RX");
    TEST_END();
    
     
    TEST_START("SPI tests");
//  TEST_ASSERT_OK(AtLibGs_SPIConf(ATLIBGS_SPIPOLARITYHIGH,ATLIBGS_SPIPHASE180), "Could not set SPI");
//  TEST_ASSERT_OK(AtLibGs_SPIConf(ATLIBGS_SPIPOLARITYLOW,ATLIBGS_SPIPHASE180), "Could not set SPI");
//  TEST_ASSERT_OK(AtLibGs_SPIConf(ATLIBGS_SPIPOLARITYHIGH,ATLIBGS_SPIPHASE0), "Could not set SPI");
//  TEST_ASSERT_OK(AtLibGs_SPIConf(ATLIBGS_SPIPOLARITYLOW,ATLIBGS_SPIPHASE0), "Could not set SPI");
    TEST_END();
    
    TEST_START("MISC tests");
//  TEST_ASSERT_OK(AtLibGs_Reset(), "Could not reset");
//  TEST_ASSERT_OK(AtLibGs_MemoryTrace(), "Could not get memory trace");
//  TEST_ASSERT_OK(AtLibGs_StartFirmWareUpdate(ATLIBGS_WLANBIN), "Could not start firmware update");
    TEST_ASSERT_OK(AtLibGs_GetTime(), "Could not get system time");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO10,ATLIBGS_LOW), "Could not set GPIO10 low");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO10,ATLIBGS_HIGH), "Could not set GPIO10 high");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO11,ATLIBGS_LOW), "Could not set GPIO11 low");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO11,ATLIBGS_HIGH), "Could not set GPIO11 high");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO30,ATLIBGS_LOW), "Could not set GPIO30 low");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO30,ATLIBGS_HIGH), "Could not set GPIO30 high");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO31,ATLIBGS_LOW), "Could not set GPIO31 low");
    TEST_ASSERT_OK(AtLibGs_SetGPIO(ATLIBGS_GPIO31,ATLIBGS_HIGH), "Could not set GPIO31 high");
//  TEST_ASSERT_OK(AtLibGs_EnableEnhancedAsync(true), "Could not enable enhanced async");
//  TEST_ASSERT_OK(AtLibGs_EnableEnhancedAsync(false), "Could not disable enhanced async");
//  TEST_ASSERT_OK(AtLibGs_EnableVerbose(false), "Could not disable verbose");
//  TEST_ASSERT_OK(AtLibGs_EnableVerbose(true), "Could not enable verbose");
    TEST_ASSERT_OK(AtLibGs_GetNetworkConfiguration(), "Could not get network conf");
//  TEST_ASSERT_OK(AtLibGs_FWUpgradeConfig(ATLIBGS_FWUPPARAM_SERVERIP,"127.0.0.1\0"), "Could not set FWUP conf");
    TEST_ASSERT_OK(AtLibGs_GetCIDInfo(), "Could not get CID info");
//  TEST_ASSERT_OK(AtLibGs_SetEAPCertificate(ATLIBGS_EAPTLS_CLIC,ATLIBGS_EAPTLS_HEX,256,ATLIBGS_EAPTLS_RAM), "Could not conf EAP-TLS Cert");
//  TEST_ASSERT_OK(AtLibGs_Ping("192.168.1.1\0", 5, 3000, 56, 0, 30, "\0"), "Could not ping");
//  TEST_ASSERT_OK(AtLibGs_TraceRoute("192.168.1.1\0", 1000, 30, 1, 0), "Could not start trace route");
    TEST_ASSERT_OK(AtLibGs_SNTPsync(true,"192.43.244.18\0", 15, true, 86400), "Could not sync time");
    TEST_ASSERT_OK(AtLibGs_SetSocketOptions(0, ATLIBGS_SOCKET_OPTION_TYPE_TCP, ATLIBGS_SOCKET_OPTION_PARAM_TCP_MAXRT, 30), "Could not set socket");
//    TEST_ASSERT_OK(AtLibGs_HTTPOpen("www.gainspan.com\0", 80, false, "\0", "\0", 30), "Could not open HTTP connection");
//    TEST_ASSERT_OK(AtLibGs_HTTPSend(0, ATLIBGS_HTTPSEND_GET,"index.html\0",30, 4,"Test"), "Could not get/post HTTP data on client conn");
//  TEST_ASSERT_OK(AtLibGs_UnsolicitedTXRate(uint32_t frame,uint16_t seq,uint8_t chan,ATLIBGS_UNSOLICITEDTX_E rate,uint32_t wmm,char mac[],char bssid[],uint16_t len), "Could not ");
//  TEST_ASSERT_OK(AtLibGs_RFAsyncFrameTX(1, 2, ATLIBGS_RFFRAMETXSTART_RATE11, 2, 200, 0, 11, 0, 30, false, true, "00:00:00:00:00:00\0", "00:00:00:00:00:00\0"), "Could not enable asynchronous frame sync ");
//  TEST_ASSERT_OK(AtLibGs_RFWaveTXStart(true, 4,ATLIBGS_RFWAVETXSTART_RATE11, true, true, true, 2, "ssid\0"), "Could not change modulation for wave transmission");
    
    TEST_END();
    
    
}

/*---------------------------------------------------------------------------*
 * Routine:  TestFlash
 *---------------------------------------------------------------------------*/
/** Test reading/writing to flash
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestFlash(void)
{
    uint8_t test[8];
    
    TEST_START("Flash");
    Flash_Open();
    TEST_ASSERT(Flash_Read(0, test, 8)==0, "Could not read flash!");
    TEST_ASSERT(Flash_ChipErase() == 0, "Could not erase!");
    TEST_ASSERT(Flash_Write(0, "Hello", 6)==0, "Could not write!");
    TEST_ASSERT(Flash_Read(0, test, 8)==0, "Could not read flash!");
    TEST_ASSERT_STRINGS_EQUAL(test, "Hello", "Did not save data correctly!");
    TEST_END();
}

/*---------------------------------------------------------------------------*
 * Routine:  TestLastCommands
 *---------------------------------------------------------------------------*/
/** Run various tests...
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void TestLastCommands(void)
{
    DisplayLCD(LCD_LINE1, "Test Last Cmd");
    ConsolePrintf("\fTEST: Testing Last Commands\r\n");

    TestEchoOff();
    TestGetInfo();
//    TestScanTimes();
//    TestSetDefaultProfile();
//    TestGetProfile();
//    TestGetSetMAC();
//    TestRegulatoryDomain();
//    TestNewCommands();
//    TestNetworkScan();
    TestFlash();

    TestEchoOn();

    ConsolePrintf("Test ended! ");
    ConsolePrintf("Num failures: %d\n", G_numFailures);
    if (G_numFailures == 0) {
        ConsolePrintf("\n\n** PASS **\n\n");
        DisplayLCD(LCD_LINE8, "** PASS **");
    } else {
        ConsolePrintf("\n\n** FAIL **\n\n");
        DisplayLCD(LCD_LINE8, "** FAIL **");
    }

}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  TestLastCommands.c
 *-------------------------------------------------------------------------*/
