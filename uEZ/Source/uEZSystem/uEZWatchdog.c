/*-------------------------------------------------------------------------*
 * File:  uEZWatchdog.c
 *-------------------------------------------------------------------------*
 * Description:
 *     Watchdog Interface
 *-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2011 Future Designs, Inc.
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
/**
 *    @addtogroup uEZWatchdog
 *  @{
 *  @brief     uEZ Watchdog Interface
 *  @see http://www.teamfdi.com/uez/
 *  @see http://www.teamfdi.com/uez/files/uEZLicense.txt
 *
 *    The uEZ Watchdog interface.
 *
 * @par Example code:
 * Example task to setup watchdog
 * @par
 * @code
 * #include <uEZ.h>
 * #include <uEZWatchdog.h>
 * 
 *  TUInt32 watchdogTest(T_uezTask aMyTask, void *aParams)
 *  {
 *      T_uezDevice WD;
 *      if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	  	
 *          UEZWatchdogSetMinTime(WD, 50);
 *          UEZWatchdogSetMaxTime(WD, 2000); // 2 sec time till reset
 *          UEZWatchdogStart(WD);
 *          if (UEZWatchdogClose(WD) != UEZ_ERROR_NONE) {
 *              // error closing the device
 *          }
 *
 *      } else {
 *          // an error occurred opening the device
 *      }
 *      return 0;
 *  }
 *  @endcode
 */
#include "Config.h"
#include "Device/Watchdog.h"
#include "uEZDevice.h"
#include <uEZDeviceTable.h>
#include <uEZWatchdog.h>

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogOpen
 *---------------------------------------------------------------------------*/
/**
 *  Open up access to the Watchdog bank.
 *
 *  @param [in]    *aName 		Pointer to name of Watchdog bank (usually
 *                                      "Watchdogx" where x is bank number)
 *  @param [in]    *aDevice		Pointer to device handle to be returned
 *
 *  @return        T_uezError 	If the device is opened, returns 
 *                                  UEZ_ERROR_NONE.  If the device cannot be 
 *                                  found, returns UEZ_ERROR_DEVICE_NOT_FOUND.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *    	// the device opened properly
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogOpen(
            const char *const aName, 
            T_uezDevice *aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableFind(aName, aDevice);
    if (error)
        return error;
    
    error = UEZDeviceTableGetWorkspace(*aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;
    
    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogClose
 *---------------------------------------------------------------------------*/
/**
 *  End access to the Watchdog bank.
 *
 *  @param [in]    aDevice 			Device handle of Watchdog to close
 *
 *  @return        T_uezError    	If the device is successfully closed, 
 *                                  returns UEZ_ERROR_NONE.  If the device 
 *                                  handle is bad, returns 
 *                                  UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      if (UEZWatchdogClose(WD) != UEZ_ERROR_NONE) {
 *          // error closing the device
 *      }
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogClose(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogSetMaxTime
 *---------------------------------------------------------------------------*/
/**
 *  Set the maximum amount of time that can go by before feeding the watchdog.
 *
 *  @param [in]    aDevice    	Device handle of Watchdog
 *
 *  @param [in]    aMaxTime  	Time til watchdog expires in milliseconds
 *
 *  @return        T_uezError  	If successful, returns UEZ_ERROR_NONE. If 
 *                              an invalid device handle, returns 
 *                              UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      UEZWatchdogSetMaxTime(WD, 2000);   // 2 sec time till reset
 *      UEZWatchdogStart(WD);
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogSetMaxTime(
            T_uezDevice aDevice,
            TUInt32 aMaxTime)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->SetMaximumTime((void *)p, aMaxTime);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogSetMinTime
 *---------------------------------------------------------------------------*/
/**
 *  Set the minimum amount of time that can go by before feeding the watchdog.
 *
 *  @param [in]    aDevice     Device handle of Watchdog
 *
 *  @param [in]    aMinTime    Time needed before feeding watchdog milliseconds
 *
 *  @return        T_uezError  If successful, returns UEZ_ERROR_NONE. If 
 *                             an invalid device handle, returns 
 *                             UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *   *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      UEZWatchdogSetMinTime(WD, 50);
 *      UEZWatchdogStart(WD);
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogSetMinTime(
            T_uezDevice aDevice,
            TUInt32 aMinTime)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->SetMinimumTime((void *)p, aMinTime);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZIsResetFromWatchdog
 *---------------------------------------------------------------------------*/
/**
 *  Check if the system has previously been reset due to the watchdog
 *
 *  @param [in]    aDevice    	Device handle of Watchdog
 *
 *  @return        T_uezError 	If successful, returns UEZ_ERROR_NONE.  If 
 *                              an invalid device handle, returns 
 *                              UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *
 *      if (UEZIsResetFromWatchdog(WD) == UEZ_ERROR_NONE) {
 *          UEZWatchdogClearResetFlag(WD); // clear reset flag
 *      }
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
TBool UEZIsResetFromWatchdog(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->IsResetFromWatchdog((void *)p);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogClearResetFlag
 *---------------------------------------------------------------------------*/
/**
 *  Clear the state of the reset flag that says the last reboot
 *      was from a watchdog reset.
 *
 *  @param [in]    aDevice    	Device handle of Watchdog
 *
 *  @return        T_uezError 	If successful, returns UEZ_ERROR_NONE. If 
 *                              an invalid device handle, returns 
 *                              UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *
 *      UEZWatchdogClearResetFlag(WD);
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void UEZWatchdogClearResetFlag(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);

    if(!error)
        (*p)->ClearResetFlag((void *)p);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogIsActive
 *---------------------------------------------------------------------------*/
/**
 *  Determine if the watchdog is actively running.
 *
 *  @param [in]    aDevice     Device handle of Watchdog
 *
 *  @return        T_uezError  If successful, returns UEZ_ERROR_NONE.  If 
 *                             an invalid device handle, returns 
 *                             UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      if (UEZWatchdogIsActive(WD) != UEZ_ERROR_NONE) {
 *          UEZWatchdogStart(WD); // start watchdog if not already started
 *      }
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
TBool UEZWatchdogIsActive(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->IsActive((void *)p);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogStart
 *---------------------------------------------------------------------------*/
/**
 *  Start the watchdog
 *
 *  @param [in]    aDevice    	Device handle of Watchdog
 *
 *  @return        T_uezError  	If successful, returns UEZ_ERROR_NONE. If 
 *                              an invalid device handle, returns 
 *                              UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *
 *      UEZWatchdogStart(WD);   // start watchdog after setting up timeout
 *                              // (or with default timeout)
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogStart(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->Start((void *)p);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogFeed
 *---------------------------------------------------------------------------*/
/**
 *  Start the watchdog.
 *
 *  @param [in]    aDevice     Device handle of Watchdog
 *
 *  @return        T_uezError  If successful, returns UEZ_ERROR_NONE. If 
 *                             an invalid device handle, returns 
 *                             UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      UEZWatchdogFeed(WD);
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogFeed(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->Feed((void *)p);
}

/*---------------------------------------------------------------------------*
 * Routine:  UEZWatchdogTrip
 *---------------------------------------------------------------------------*/
/**
 *  Purposely cause the watchdog to be tripped.
 *
 *  @param [in]    aDevice     	Device handle of Watchdog
 *
 *  @return        T_uezError 	If successful, returns UEZ_ERROR_NONE. If 
 *                              an invalid device handle, returns 
 *                              UEZ_ERROR_HANDLE_INVALID.
 *  @par Example Code:
 *  @code
 *  #include <uEZ.h>
 *  #include <uEZWatchdog.h>
 *
 *  T_uezDevice WD;
 *  if (UEZWatchdogOpen("Watchdog0", &WD) == UEZ_ERROR_NONE) {
 *	 
 *      UEZWatchdogTrip(WD); // trip the watchdog
 *
 *  } else {
 *      // an error occurred opening the device
 *  }
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
T_uezError UEZWatchdogTrip(T_uezDevice aDevice)
{
    T_uezError error;
    DEVICE_Watchdog **p;
    
    error = UEZDeviceTableGetWorkspace(aDevice, (T_uezDeviceWorkspace **)&p);
    if (error)
        return error;

    return (*p)->Trip((void *)p);
}
/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  uEZWatchdog.c
 *-------------------------------------------------------------------------*/
