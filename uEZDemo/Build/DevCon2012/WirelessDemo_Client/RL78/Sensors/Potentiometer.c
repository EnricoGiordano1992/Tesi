/*-------------------------------------------------------------------------*
 * File:  Potentiometer.c
 *-------------------------------------------------------------------------*/
/** @addtogroup Potentiometer 
 * @{
 *     @brief Potentiometer driver that uses the RX62N's S12AD A/D input.
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <system/platform.h>
#include <drv/ADC.h>
#include "Potentiometer.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Routine:  Potentiometer_Init
 *---------------------------------------------------------------------------*/
/** Initialize the potentiometer driver.
 *      
 *  @return     void
 */
/*---------------------------------------------------------------------------*/
void Potentiometer_Init(void)
{
    ADC_Start();
    ADC_EnableChannel(POTENTIOMETER_CHANNEL);
}

/*---------------------------------------------------------------------------*
 * Routine:  Potentiometer_Get
 *---------------------------------------------------------------------------*/
/** Read the potentiometer and get the percent it is turned.
 *      
 *  @return     uint32_t -- value of 0 to 1000 for 0% to 100%
 */
/*---------------------------------------------------------------------------*/
uint32_t Potentiometer_Get(void)
{
    int32_t adc;

    // ADC sensor reading
    adc = ADC_GetReading(POTENTIOMETER_CHANNEL);

    // Get the temperature and show it on the LCD
    adc *= 1000; // scale it from 0 to 1000
    adc /= (1 << 12); // 12-bit reading

    return adc;
}

/** @} */
/*-------------------------------------------------------------------------*
 * End of File:  Potentiometer.c
 *-------------------------------------------------------------------------*/
