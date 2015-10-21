/*-------------------------------------------------------------------------*
 * File:  UEZGainSpan.h
 *-------------------------------------------------------------------------*/
#ifndef UEZGAINSPAN_H_
#define UEZGAINSPAN_H_

/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2012 Future Designs, Inc.
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

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <uEZTypes.h>
#include <string.h>
#include <uEZ.h>
#include <uEZStream.h>
#include <uEZSPI.h>

#define TEMPERATURE_NO_READING      -10000 // reading of -10000 means no reading
typedef struct {
     TInt32 iTemperature; // temperature in 100 per degree C
     TInt32 iPotentiometer; // Reading
} T_GainSpanTemperatureStatus;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
int UEZGUIGainSpan(void *aWorkspace, int argc, char *argv[], char *rxBuf);
void UEZGUIGainSpanProgram(void);
void UEZGUIGainSpanStartTask(void);
void UEZGUIGainSpanGetStatus(T_GainSpanTemperatureStatus *aStatus);

#endif // UEZGAINSPAN_H_
/*-------------------------------------------------------------------------*
 * End of File:  UEZGainSpan.h
 *-------------------------------------------------------------------------*/
