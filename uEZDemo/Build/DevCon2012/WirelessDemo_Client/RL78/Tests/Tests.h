/*-------------------------------------------------------------------------*
 * File:  Tests.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Variety of tests to perform.
 *-------------------------------------------------------------------------*/
#ifndef TESTS_H_
#define TESTS_H_

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
extern uint8_t G_testBuffer[23];
extern uint8_t G_testBuffer2[23];

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
void TestADCPotentiometer(void);
void TestConsole_Loopback(void);
void TestGainSpan_ATCommands(void);
void TestGainSpan_ATCommands(void);
void TestI2CTemperature(void);
void TestSPI_ATCommands(void);
void TestSPI_IdleChars(void);
void TestSPI_Speed(void);
void TestUART_Loopback(void);
void TestConnectionCommands(void);
void TestLastCommands(void);

void ErrorCode(uint8_t blinks);

#endif // TESTS_H_
/*-------------------------------------------------------------------------*
 * End of File:  Tests.h
 *-------------------------------------------------------------------------*/
