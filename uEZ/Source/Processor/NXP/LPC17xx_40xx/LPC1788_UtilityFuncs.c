/*-------------------------------------------------------------------------*
 * File:  LPC1768_UtilityFuncs.c
 *-------------------------------------------------------------------------*
 * Description:
 *      Utilty functions that are specific to the LPC1768 processor.
 *-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2010 Future Designs, Inc.
 *--------------------------------------------------------------------------
 * This file is part of the uEZ(R) distribution.  See the included
 * uEZLicense.txt or visit http://www.teamfdi.com/uez for details.
 *
 *    *===============================================================*
 *    |  Future Designs, Inc. can port uEZ(tm) to your own hardware!  |
 *    |             We can get you up and running fast!               |
 *    |      See http://www.teamfdi.com/uez for more details.         |
 *    *===============================================================*
 *
 *-------------------------------------------------------------------------*/
#include <Config.h>
#include <uEZTypes.h>
#include <uEZ.h>
#include <uEZPlatform.h>
#include <uEZProcessor.h>
#include <Source/Processor/NXP/LPC17xx_40xx/LPC1788_UtilityFuncs.h>
#include <uEZBSP.h>
#include <uEZGPIO.h>
#include <uEZLCD.h>


void LPC1788PowerOn(TUInt32 bits)
{
    extern unsigned int G_lpc1788_powerSetting;
    G_lpc1788_powerSetting |= (bits);
    LPC_SC->PCONP = G_lpc1788_powerSetting;
}

void LPC1788PowerOff(TUInt32 bits)
{
    extern unsigned int G_lpc1788_powerSetting;
    G_lpc1788_powerSetting &= ~(bits);
    LPC_SC->PCONP = G_lpc1788_powerSetting;
}

void LPC1788_IOCON_ConfigPin(
        T_uezGPIOPortPin aPortPin,
        const T_LPC1788_IOCON_ConfigList *aList,
        TUInt8 aCount)
{
    while (aCount--) {
        if (aList->iPortPin == aPortPin) {
            UEZGPIOLock(aPortPin);
            UEZGPIOControl(aPortPin, GPIO_CONTROL_SET_CONFIG_BITS,
                    aList->iSetting);
            return;
        }
        aList++;
    }
    UEZFailureMsg("Bad Pin");
}

void LPC1788_IOCON_ConfigPinOrNone(
        T_uezGPIOPortPin aPortPin,
        const T_LPC1788_IOCON_ConfigList *aList,
        TUInt8 aCount)
{
    if (aPortPin == GPIO_NONE)
        return;
    while (aCount--) {
        if (aList->iPortPin == aPortPin) {
            UEZGPIOLock(aPortPin);
            UEZGPIOControl(aPortPin, GPIO_CONTROL_SET_CONFIG_BITS,
                    aList->iSetting);
            return;
        }
        aList++;
    }
    UEZFailureMsg("Bad Pin");
}

/*-------------------------------------------------------------------------*
 * End of File:  LPC1768_UtilityFuncs.c
 *-------------------------------------------------------------------------*/

