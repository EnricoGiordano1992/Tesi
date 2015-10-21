/*-------------------------------------------------------------------------*
 * File:  EEPROM.c
 *-------------------------------------------------------------------------*
 * Description:
 *     RX62N Flash Driver.  Provides functions to read, write, and erase
 *     the internal EEPROM.  Used to store non-volatile information.
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <system/mstimer.h>
#include "drv/I2C.h"

#define EEPROM_SIZE     64000   // 64KB (512Kb)
#define EEPROM_ADDR     0xA0u   // Slave Address (Write)
#define EEPROM_A16      0x02    // A16 of memory address

#define EEPROM_TIMEOUT  20      // Millisecond Timeout on I2C bus

#define EEPROM_BYTES_PER_WRITE  32

/*******************************************************************************
 * Outline      : EEPROM_Open
 * Description  : 
 * Argument     : none
 * Return value : none
 *******************************************************************************/
void EEPROM_Open(void)
{
    /* Do nothing for now */
}

/*******************************************************************************
 * Outline      : EEPROM_ChipErase
 * Description  : This function enters a for loop, and erases blocks of data
 *                 EEPROM memory each iteration until all blocks have been erased.
 * Argument     : none
 * Return value : none
 *******************************************************************************/
uint8_t EEPROM_ChipErase(void)
{
    return 0;
}

/*******************************************************************************
 * Outline      : EEPROM_Write
 * Description  : This function writes the given contents to the
 *                 EEPROM, at the given location.
 * Argument     : offset -- Offset byte from start of EEPROM
 *                aData -- Pointer to bytes to write to EEPROM
 *                aSize -- number of bytes to write to EEPROM
 * Return value : 0 = success, else failure
 *******************************************************************************/
uint8_t EEPROM_Write(uint16_t offset, uint8_t *aData, uint16_t aSize)
{
    I2C_Request r;
    uint32_t timeout = MSTimerGet();
    uint8_t writeData[EEPROM_BYTES_PER_WRITE+2];
    uint16_t dataAddr, byteIndex;

    r.iAddr = EEPROM_ADDR>>1;
    r.iSpeed = 100; /* kHz */
    
    // Write Data in groups of size defined by EEPROM_BYTES_PER_WRITE
    for(dataAddr=offset; dataAddr<(aSize+offset); dataAddr+=EEPROM_BYTES_PER_WRITE) {
      
        // Data Address in the EEPROM to write to
        writeData[0] = (uint8_t)dataAddr<<8;
        writeData[1] = (uint8_t)dataAddr;
        
        byteIndex = 0;
        while(byteIndex < EEPROM_BYTES_PER_WRITE) {
            writeData[2+byteIndex] = aData[dataAddr+byteIndex];
            byteIndex++;
        }
        
        r.iWriteData = writeData;
        r.iWriteLength = 2+byteIndex;
        r.iReadData = 0;
        r.iReadLength = 0;
    
        I2C_Start();
        I2C_Write(&r, 0);
        MSTimerDelay(10); // Part requires a 5ms to process a data write
        while ((I2C_IsBusy()) && (MSTimerDelta(timeout) < EEPROM_TIMEOUT))
            {}
    }
    
    return 0;
}


/*******************************************************************************
 * Outline      : EEPROM_Read
 * Description  : This function writes the given contents to the
 *                 EEPROM, at the given location.
 * Argument     : offset -- Offset byte from start of EEPROM
 *                aData -- Pointer to bytes to write to EEPROM
 *                aSize -- number of bytes to write to EEPROM
 * Return value : 0 = success, else failure
 *******************************************************************************/
uint8_t EEPROM_Read(uint16_t offset, uint8_t *aData, uint16_t aSize)
{
    uint8_t writeData[2];
    uint32_t timeout = MSTimerGet();
    I2C_Request r;

    writeData[0] = (uint8_t)offset<<8;
    writeData[1] = (uint8_t)offset;
    
    r.iAddr = EEPROM_ADDR>>1;
    r.iSpeed = 100;
    r.iWriteData = writeData;
    r.iWriteLength = 2;
    r.iReadData = aData;
    r.iReadLength = aSize;
    
    I2C_Start();
    I2C_Write(&r, 0);
    while ((I2C_IsBusy()) && (MSTimerDelta(timeout) < EEPROM_TIMEOUT))
        {}
    I2C_Read(&r, 0);
    while ((I2C_IsBusy()) && (MSTimerDelta(timeout) < EEPROM_TIMEOUT))
        {}

    return 0;
}

/*******************************************************************************
 * Outline      : EEPROM_Erase
 * Description  : This function enters a for loop, and erases blocks of data
 *                 EEPROM memory that cover the given address range.  This
 *                 routine is not smart enough to keep the existing data
 *                 in a block that is erased.
 * Argument     : none
 * Return value : none
 *******************************************************************************/
uint8_t EEPROM_Erase(uint16_t offset, uint16_t aSize)
{
    I2C_Request r;
    uint32_t timeout = MSTimerGet();
    uint8_t writeData[EEPROM_BYTES_PER_WRITE+2];
    uint16_t dataAddr, byteIndex;

    r.iAddr = EEPROM_ADDR>>1;
    r.iSpeed = 100; /* kHz */
    
    // Write Data in groups of size defined by EEPROM_BYTES_PER_WRITE
    for(dataAddr=offset; dataAddr<(aSize+offset); dataAddr+=EEPROM_BYTES_PER_WRITE) {
      
        // Data Address in the EEPROM to write to
        writeData[0] = (uint8_t)dataAddr<<8;
        writeData[1] = (uint8_t)dataAddr;
        
        byteIndex = 0;
        while(byteIndex < EEPROM_BYTES_PER_WRITE) {
            writeData[2+byteIndex] = 0x00;
            byteIndex++;
        }
        
        r.iWriteData = writeData;
        r.iWriteLength = 2+byteIndex;
        r.iReadData = 0;
        r.iReadLength = 0;
    
        I2C_Start();
        I2C_Write(&r, 0);
        MSTimerDelay(5); // Part requires a 5ms to process a data write
        while ((I2C_IsBusy()) && (MSTimerDelta(timeout) < 10))
            {}
    }
    
    return 0;
}

#define EEPROM_TEST_DATA_ADDR       0x00
#define EEPROM_TEST_BUFFER_SIZE     128
bool EEPROM_Test()
{
    uint8_t writeData[EEPROM_TEST_BUFFER_SIZE];
    uint8_t readData[EEPROM_TEST_BUFFER_SIZE];
    uint16_t i;
    
    for(i=0; i<EEPROM_TEST_BUFFER_SIZE; i++){
        writeData[i] = i;
    }
    
    EEPROM_Write(EEPROM_TEST_DATA_ADDR, writeData, EEPROM_TEST_BUFFER_SIZE);
    EEPROM_Read(EEPROM_TEST_DATA_ADDR, readData, EEPROM_TEST_BUFFER_SIZE);
    
    for(i=0; i<EEPROM_TEST_BUFFER_SIZE; i++){
        if(writeData[i] != readData[i]){
            return false;
        }
    }
    
    return true;
}

/*-------------------------------------------------------------------------*
 * End of File:  EEPROM.c
 *-------------------------------------------------------------------------*/
