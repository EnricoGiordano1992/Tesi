/*-------------------------------------------------------------------------*
 * File:  main.c
 *-------------------------------------------------------------------------*/
/** @addtogroup  main
 * @{
 *     @brief Sets up one of the hardware and drivers and then calls one of the
 *     tests or demos.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdio.h>
#include <system/platform.h>
#include <HostApp.h>
#include <init/hwsetup.h>
#include <drv\Glyph\lcd.h>
#include <system\mstimer.h>
#include <system/Switch.h>
#include <sensors\Potentiometer.h>
#include <sensors\Temperature.h>
#include <Tests\Tests.h>
#include <system\console.h>
#include <drv\UART0.h>
#include <drv\UART2.h>
#include <drv\SPI.h>
#include <CmdLib\GainSpan_SPI.h>
#include <Apps/NVSettings.h>
#include <Apps/Apps.h>

/*-------------------------------------------------------------------------*
 * Macros:
 *-------------------------------------------------------------------------*/
/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xE9U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x04U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
/* Application Modes */
typedef enum {
    PROGRAM_MODE,
    UART_PT_MODE,
    SPI_PT_MODE,
    GAINSPAN_DEMO
}AppMode_T;

/*---------------------------------------------------------------------------*
 * Routine:  main
 *---------------------------------------------------------------------------*/
/** Setup the hardware, setup the peripherals, show the startup banner,
 *      wait for the module to power up, run a selected test or app.
 *      
 *  @param [in] void
 *  @return     int -- always 0.
 */
/*---------------------------------------------------------------------------*/
int  main(void)
{
    AppMode_T AppMode;
    char LCDString[20];
    
    /* Configure the hardware */
    HardwareSetup();
    
    /* Default app mode */
    AppMode = GAINSPAN_DEMO;
    
    /* Determine if SW1 & SW3 is pressed at power up to enter programming mode */
    if (Switch1IsPressed() && Switch3IsPressed()) {
        //App_ProgramMode();
      AppMode = PROGRAM_MODE;
    }
    
    /* Initialize millisecond timer */
    MSTimerInit();
    
    /* Initialize SPI */
    SPI_Init(GAINSPAN_SPI_RATE);
    
    /* Setup LCD SPI channel for Chip Select P10, active low, active per byte  */
    SPI_ChannelSetup(SPI_LCD_CHANNEL, false, true);

    #ifdef ATLIBGS_INTERFACE_SPI
        /* Setup WIFI SPI channel for Chip Select P7x, active low, active per byte  */
        SPI_ChannelSetup(GAINSPAN_SPI_CHANNEL, false, true);
        GainSpan_SPI_Start();
    #endif
        
    /* Send LCD configuration */
    InitialiseLCD();
    
    if (AppMode == PROGRAM_MODE) {
        App_ProgramMode();
    }
    else {
    
        UART0_Start(GAINSPAN_CONSOLE_BAUD);
        UART2_Start(GAINSPAN_UART_BAUD);

        Temperature_Init();
        Potentiometer_Init();
    
        sprintf(LCDString, "GS Demo %s", VERSION_TEXT);
        DisplayLCD(LCD_LINE1, (const uint8_t *)LCDString);
        
        /* Before doing any tests or apps, startup the module */
        /* and nonvolatile stettings */
        App_Startup();
    
        // Now connect to the system
        App_Connect(&G_nvsettings.webprov);
        
        /* Run one of the demo modes */ 
        //App_COSM();
        //App_UDPDemo();
        //App_TCPServerDemo();
        //App_TCPClientDemo();
        //App_HTTPDemo();
        
        //App_PassThroughUART();
        //App_PassThroughSPI();
        App_WirelessDemoClient();
    }
    
    return 0;
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  main.c
 *-------------------------------------------------------------------------*/

