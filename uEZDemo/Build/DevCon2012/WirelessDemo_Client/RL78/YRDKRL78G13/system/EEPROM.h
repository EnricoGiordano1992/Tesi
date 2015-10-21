/*-------------------------------------------------------------------------*
 * File:  EEPROM.h
 *-------------------------------------------------------------------------*
 * Description:
 *     
 *-------------------------------------------------------------------------*/
#ifndef EEPROM_H_
#define EEPROM_H_

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
/* Initialise EEPROM function prototype declaration */
void EEPROM_Open(void);
/* EEPROM Erase function prototype declaration */
uint8_t EEPROM_Erase(uint16_t offset, uint16_t aSize);
/* EEPROM erase function prototype declaration */
uint8_t EEPROM_ChipErase(void);
/* EEPROM write function prototype declaration */
uint8_t EEPROM_Write(uint16_t offset, uint8_t *aData, uint16_t aSize);
/* EEPROM read function prototype declaration */
uint8_t EEPROM_Read(uint16_t offset, uint8_t *aData, uint16_t aSize);

bool EEPROM_Test();

#endif // EEPROM_H_
/*-------------------------------------------------------------------------*
 * End of File:  EEPROM.h
 *-------------------------------------------------------------------------*/
