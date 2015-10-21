/*-------------------------------------------------------------------------*
 * File:  App_Menu.c
 *-------------------------------------------------------------------------*/
/** @addtogroup App_Menu
 * @{
 *     @brief Allows user to select which mode to run from a menu on the 
 *              Display
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <HostApp.h>
#include <system/platform.h>
#include <system/mstimer.h>
#include <system/switch.h>
#include <drv/Glyph/lcd.h>
#include <Apps/Apps.h>

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define NUM_DEMOS       7

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
typedef void (*T_appDemo)(void);

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
const uint8_t * const G_demos[NUM_DEMOS] = {
        "  COSM TCP  ",
        "  COSM HTTP ",
        " TCP Client ",
        " TCP Server ",
        "  UDP Demo  ",
        "HTTP OTA FWU",
        "Pass Through",
};

const T_appDemo G_demoApps[NUM_DEMOS] = {
        App_COSM,
        App_HTTPDemo,
        App_TCPClientDemo,
        App_TCPServerDemo,
        App_UDPDemo,
        App_HTTPOverTheAirProgramming,
#ifdef ATLIBGS_INTERFACE_UART
        App_PassThroughUART,
#endif
#ifdef ATLIBGS_INTERFACE_SPI
        App_PassThroughSPI,
#endif
};

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
 
 /*---------------------------------------------------------------------------*
 * Routine:  App_Menu
 *---------------------------------------------------------------------------*/
/** Allows user to select which mode to run from a menu on the 
 *              Display
 *      
 *  @return    void
 */
/*---------------------------------------------------------------------------*/
void App_Menu(void)
{
    uint8_t choice = 0;
    ClearLCD();

    DisplayLCD(LCD_LINE1, "GS Demo 2.00");
    DisplayLCD(LCD_LINE3, "Select Demo:");
    DisplayLCD(LCD_LINE5, G_demos[choice]);
    DisplayLCD(LCD_LINE7, "SW1 Next");
    DisplayLCD(LCD_LINE8, "SW2 Enter");

    while (1) {
        if (Switch1IsPressed()) {
            // Roll to the next choice
            choice++;
            if (choice >= NUM_DEMOS)
                choice = 0;
            DisplayLCD(LCD_LINE5, G_demos[choice]);

            // Debounce (after a delay to avoid potential noise on switch)
            MSTimerDelay(250);
            while (Switch1IsPressed())
                {}
        }

        if (Switch2IsPressed()) {
            // Choice is selected.  Run that app (after mostly clearing screen)
            DisplayLCD(LCD_LINE2, G_demos[choice]);
            DisplayLCD(LCD_LINE3, "");
            DisplayLCD(LCD_LINE5, "");
            DisplayLCD(LCD_LINE7, "");
            DisplayLCD(LCD_LINE8, "");
            
            // Call the function to run the selected demo
            G_demoApps[choice]();
        }
    }
}
/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  App_Menu.c
 *-------------------------------------------------------------------------*/
