/*-------------------------------------------------------------------------*
 * File:  I2C_Mux_PCA9546.h
 *-------------------------------------------------------------------------*
 * Description:
 *
 *-------------------------------------------------------------------------*/
#ifndef I2C_MUX_PCA9546_H_
#define I2C_MUX_PCA9546_H_

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
#include <Source/Devices/I2C/Generic/Generic_I2C.h>

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
extern const DEVICE_I2C_BUS I2CBus_Mux_PCA9546_Interface;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
void I2C_Mux_PCA9546_Create(
    const char *aName,
    const char *aI2CBus,
    TUInt8 aI2CMuxAddr,
    TUInt8 aI2CMuxLane,
    void **aGroupWorkspace);

#endif // I2C_MUX_PCA9546_H_
/*-------------------------------------------------------------------------*
 * End of File:  I2C_Mux_PCA9546.h
 *-------------------------------------------------------------------------*/
