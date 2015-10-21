#ifndef HOST_APP_H_
#define HOST_APP_H_

#include <drv\SPI.h>

#define VERSION_MAJOR       2
#define VERSION_MINOR       2
#define VERSION_TEXT        "2.02"

/* Set the UART rate to the Console: */
#define GAINSPAN_CONSOLE_BAUD        115200

#define GAINSPAN_AT_DEFAULT_TIMEOUT  2000

#define ATLIBGS_DEBUG_ENABLE       // output information on the serial port to PC

// Choose one of the following:  SPI or UART communications
// NOTE that the GainSpan module requires the correct firmware to be loaded.
#define  ATLIBGS_INTERFACE_SPI      /* SPI interface is used for GS1011 S2w App communication */
//#define  ATLIBGS_INTERFACE_UART   /* UART interface is used for GS1011 S2w App communication */

#define GAINSPAN_SPI_CHANNEL      SPI_APPHEADER_CHANNEL  /* Use this line if you connect to Application Header (J6) */
//#define GAINSPAN_SPI_CHANNEL    SPI_PMOD1_CHANNEL   /* Use this line if you connect to PMOD1 (J11) */
//#define GAINSPAN_SPI_CHANNEL    SPI_PMOD2_CHANNEL   /* Use this line if you connect to PMOD1 (J11) */

/* Set the SPI rate to the GainSpan module: */
#define GAINSPAN_SPI_RATE            312500   // Max 857142

/* Set the UART rate to the GainSpan module: */
#define GAINSPAN_UART_BAUD           9600

/* MAC Address of the S2W Node  */
#define ATLIBGS_GS_NODE_MAC_ID      "00:1D:C9:01:01:D0"

#if 0 // Demo configuration
#define ATLIBGS_AP_SSID             "GainSpanDemo"
/* Access Point (AP) channel here. For auto channel, AP channel is NULL  */
#define ATLIBGS_AP_CHANNEL          NULL //  "6"
/* Uncomment the following macro to enable WPA PSK security */
//#define  ATLIBGS_SEC_PSK
/* Specify the AP PSK here  */
//#define ATLIBGS_AP_SEC_PSK          ""
#endif

#if 1 // work configuration
#define ATLIBGS_AP_SSID             "FDI_10172012"
#define ATLIBGS_AP_CHANNEL          NULL
#define ATLIBGS_SEC_PSK
#define ATLIBGS_AP_SEC_PSK          "fdifdifdiFDI"
#endif

#if 0 // work2 configuration
#define ATLIBGS_AP_SSID             "TestRouterNoInternet"
#define ATLIBGS_AP_CHANNEL          NULL
#define ATLIBGS_SEC_PSK
#define ATLIBGS_AP_SEC_PSK          "fdifdifdi"
#endif

#if 0 // home configuration
#define ATLIBGS_AP_SSID             "lesinknet3"
#define ATLIBGS_AP_CHANNEL          NULL
#define ATLIBGS_SEC_PSK
#define ATLIBGS_AP_SEC_PSK          "CB6AYBWGED"
#endif


#define ATLIBGS_LIMITED_AP_SSID     "GainSpanLAP"
#define ATLIBGS_LIMITED_AP_CHANNEL  0  // use 0 for any
#define ATLIBGS_LIMITED_AP_IP       "192.168.5.1"
#define ATLIBGS_LIMITED_AP_MASK     "255.255.255.0"
#define ATLIBGS_LIMITED_AP_GATEWAY  "192.168.5.0"

#define ATLIBGS_FWUPGRADE_SERVER  "192.168.0.101"

#endif

