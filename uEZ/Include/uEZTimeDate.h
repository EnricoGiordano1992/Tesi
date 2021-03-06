/**
 *	@file 	uEZTimeDate.h
 *  @brief 	uEZ Time and Date Interface
 *
 *	The uEZ interface which maps to the low level TimeDate drivers.
 */
#ifndef _UEZ_TimeDate_H_
#define _UEZ_TimeDate_H_

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

#include <uEZTypes.h>
#include <uEZErrors.h>
#include <Types/TimeDate.h>

/**
 *	Get the time date structure from the system.
 *
 *	@param [out] 	*aTimeDate		Time and date structure returned
 *
 *	@return		T_uezError
 */
T_uezError UEZTimeDateGet(T_uezTimeDate *aTimeDate);

/**
 *	Set the time and date on the system clock.
 *
 *	@param [out] 	*aTimeDate		Time and date structure to use
 *
 *	@return		T_uezError
 */
T_uezError UEZTimeDateSet(T_uezTimeDate *aTimeDate);

void UEZTimeInit(T_uezTime *aTime);
void UEZTimeCalcDuration(T_uezTime *aT1, T_uezTime *aT2, T_uezTimeDuration *aDuration);
char UEZTimeCompare(T_uezTime *aT1, T_uezTime *aT2);
void UEZTimeParse(T_uezTime *aTime, char *aString);
TUInt16 UEZTimeAddDuration(T_uezTime *aTime, T_uezTimeDuration *aDuration);
TUInt16 UEZTimeSubtractDuration(T_uezTime *aTime, T_uezTimeDuration *aDuration);

void UEZDateInit(T_uezDate *aDate);
TInt8 UEZDateCompare(T_uezDate *aDate1, T_uezDate *aDate2);
TBool UEZDateIsLeapYear(T_uezDate *aDate);
TUInt16 UEZDateGetDayOfYear(T_uezDate *aDate);
TUInt8 UEZDateGetNumDaysInMonth(T_uezDate *aDate);
void UEZDateNextDay(T_uezDate *aDate);
void UEZDateNextWeek(T_uezDate *aDate);
void UEZDateNextMonth(T_uezDate *aDate);
void UEZDateNextYear(T_uezDate *aDate);
void UEZDatePreviousDay(T_uezDate *aDate);
void UEZDatePreviousWeek(T_uezDate *aDate);
void UEZDatePreviousMonth(T_uezDate *aDate);
void UEZDatePreviousYear(T_uezDate *aDate);
TUInt16 UEZDateGetNumDaysInYear(T_uezDate *aDate);
TUInt32 UEZDateCalcDuration(T_uezDate *aD1, T_uezDate *aD2);
void UEZDateParse(T_uezDate *aDate, char *aString);
TUInt8 UEZDateGetDayOfWeek(T_uezDate *aDate);
void UEZDateDayOfYearToDate(TUInt16 aDayOfYear, T_uezDate *aDate);
TInt8 UEZTimeDateCompare(T_uezTimeDate *aTD1, T_uezTimeDate *aTD2);
void UEZTimeDateCalcDuration(
        T_uezTimeDate *aTD1,
        T_uezTimeDate *aTD2,
        T_uezTimeDuration *aDuration);
void UEZTimeDateParse(
        T_uezTimeDate *aTD,
        char *aDateString,
        char *aTimeString);

void UEZTimeDateDurationInit(T_uezTimeDuration *aDuration);
void UEZTimeDateSumDuration(T_uezTimeDuration *aSum, T_uezTimeDuration *aAdded);
double UEZTimeDateDurationToDouble(T_uezTimeDuration *aDuration);
void UEZTimeDateAddDuration(T_uezTimeDate *aTD, T_uezTimeDuration *aAdded);
void UEZTimeDateSubtractDuration(
        T_uezTimeDate *aTD,
        T_uezTimeDuration *aDuration);

/**
 *  Determine if the system RTC is valid.  If not, reset with the
 *  given time and date.
 *
 *  @param [in]    *aResetTimeDate      Time and date structure to use on reset
 *
 *  @return     T_uezError.  Returns UEZ_ERROR_INVALID if invalid.
 */
T_uezError UEZTimeDateIsValid(const T_uezTimeDate *aResetTimeDate);

#endif // _UEZ_TimeDate_H_

/*-------------------------------------------------------------------------*
 * End of File:  uEZTimeDate.h
 *-------------------------------------------------------------------------*/
