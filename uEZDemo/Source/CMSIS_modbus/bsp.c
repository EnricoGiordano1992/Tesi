/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2010     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for Embedded Artists LPC1788 eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#define BSP_C
#include "bsp.h"

#include <CMSIS/LPC177x_8x.h>
#include "system_LPC177x_8x.h"
#include "lpc_types.h"

#ifdef __ICCARM__
  #pragma diag_suppress=Pe188  // Avoid "enumerated type mixed with another type" warning as we are not using enumerated values from CMSIS files for NVIC_* parameters
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// SDRAM settings
//
#define SDRAM_SIZE_MB    32
#define SDRAM_SIZE       (SDRAM_SIZE_MB * 1024 * 1024)
#define SDRAM_BASE_ADDR  0xA0000000

//
// LEDs
//
#define NUM_LEDS       2
#define LED0_BIT      26
#define LED1_BIT      27
#define LED_MASK_ALL  ((1 << LED0_BIT) | (1 << LED1_BIT))

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _DelayMs()
*
* Function description:
*   Starts a timer and waits for the given delay in ms.
*/
static void _DelayMs(U32 ms) {
  LPC_TIM0->TCR = 0x02;  // Reset timer
  LPC_TIM0->PR  = 0x00;  // Set prescaler to zero
  LPC_TIM0->MR0 = ms * (SystemCoreClock / (LPC_SC->PCLKSEL & 0x1F) / 1000 - 1);
  LPC_TIM0->IR  = 0xFF;  // Reset all interrrupts
  LPC_TIM0->MCR = 0x04;  // Stop timer on match
  LPC_TIM0->TCR = 0x01;  // Start timer
  //
  // Wait until delay time has elapsed
  //
  while (LPC_TIM0->TCR & 1);
}

/*********************************************************************
*
*       _TestSDRAM()
*
*  Function description:
*    Simple SDRAM test for testing timing setup
*
*  Return value:
*    1: Error
*    0: O.K.
*/
static int _TestSDRAM(void) {
  volatile U32 * pWriteLong;
  volatile U16 * pWriteShort;
           U32   Data;
           U32   i;
           U32   j;

  pWriteLong  = (U32*)SDRAM_BASE_ADDR;
  pWriteShort = (U16*)SDRAM_BASE_ADDR;
  //
  // Fill 16 bit wise
  //
  for (i = 0; i < (SDRAM_SIZE / 0x40000); i++) {
    for (j = 0; j < 0x100; j++) {
      *pWriteShort++ = (i + j);
      *pWriteShort++ = (i + j) + 1;
    }
  }
  //
  // Verifying
  //
  pWriteLong = (U32*)SDRAM_BASE_ADDR;
  for (i = 0; i < (SDRAM_SIZE / 0x40000); i++) {
    for (j = 0; j < 0x100; j++) {
      Data = *pWriteLong++;
      if (Data != (((((i + j) + 1) & 0xFFFF) << 16) | ((i + j) & 0xFFFF))) {
        return 1;  // Error
      }
    }
  }
  return 0;  // O.K.
}

/*********************************************************************
*
*       _FindDelay()
*
*  Function description:
*    Test for min./max. CMDDLY or FBCLKDLY values
*
*  Parameter:
*    DelayType: 0 = CMDDLY, 1 = FBCLKDLY
*/
static void _FindDelay(int DelayType) {
  U32 Delay;
  U32 Min;
  U32 Max;
  U32 v;

  //
  // Init start values
  //
  Delay = 0x00;
  Min   = 0xFF;
  Max   = 0xFF;
  //
  // Test for DLY min./max. values
  //
  while (Delay < 32) {
    //
    // Setup new DLY value to test
    //
    if (DelayType == 0) {
      v                 = LPC_SC->EMCDLYCTL & ~0x001Ful;
      LPC_SC->EMCDLYCTL = v | Delay;
    } else {
      v                 = LPC_SC->EMCDLYCTL & ~0x1F00ul;
      LPC_SC->EMCDLYCTL = v | (Delay << 8);
    }
    //
    // Test configured DLY value and find out min./max. values that will work
    //
    if (_TestSDRAM() == 0) {
      //
      // Test passed, remember min. DLY value if not done yet
      //
      if (Min == 0xFF) {
        Min = Delay;
      }
    } else {
      //
      // Test failed, if a min. value has been found before, remember the current value for max.
      //
      if (Min != 0xFF) {
        Max = Delay;
      }
    }
    Delay++;
  }
  //
  // Calc DLY value
  //
  if        (Max != 0xFF) {  // If we found a min. and max. value we use the average of the min. and max. values to get an optimal DQSIN delay
    Delay = (Min + Max) / 2;
  } else if (Min != 0xFF) {  // If we found only a min. value we use the average of the min. value and the longest DLY value to get an optimal DQSIN delay
    Delay = (Min + 0x1F) / 2;
  } else {                   // No working max. and/or min. values found
    while (1);  // Fatal error
  }
  //
  // Setup DLY value to work with
  //
  if (DelayType == 0) {
    v                 = LPC_SC->EMCDLYCTL & ~0x001Ful;
    LPC_SC->EMCDLYCTL = v | Delay;
  } else {
    v                 = LPC_SC->EMCDLYCTL & ~0x1F00ul;
    LPC_SC->EMCDLYCTL = v | (Delay << 8);
  }
}

/*********************************************************************
*
*       _CalibrateOsc()
*
*  Function description:
*    Calibrate ring oscillator.
*
*  Return value:
*    Current ring oscillator count
*/
static U32 _CalibrateOsc(void) {
  U32 Cnt;
  U32 v;
  U32 i;

  //
  // Init start values
  //
  Cnt = 0;
  //
  // Calibrate osc.
  //
  for (i = 0; i < 10; i++) {
    LPC_SC->EMCCAL = (1 << 14);     // Start calibration
    v = LPC_SC->EMCCAL;
    while ((v & (1 << 15)) == 0) {  // Wait for calibration done
      v = LPC_SC->EMCCAL;
    }
    Cnt += (v & 0xFF);
  }
  return (Cnt / 10);
}

/*********************************************************************
*
*       _AdjustEMCTiming()
*
*  Function description:
*    Adjust timings for EMC.
*/
static void _AdjustEMCTiming(U32 Delay) {
  U32 v;
  U32 CmdDly;
  U32 FBDelay;
  U32 FBClkDly;

  FBDelay = _CalibrateOsc();

  v = LPC_SC->EMCDLYCTL;
  CmdDly            = ((v &  0x001Ful) * Delay / FBDelay) & 0x1F;
  FBClkDly          = ((v &  0x1F00ul) * Delay / FBDelay) & 0x1F00;
  LPC_SC->EMCDLYCTL =  (v & ~0x1F1Ful) | FBClkDly | CmdDly;
}

/*********************************************************************
*
*       _EMC_Init()
*
*  Purpose:
*    Initializes external memory controller for SDRAM
*/
static void _EMC_Init(void) {
  volatile U32 i;
  volatile U32 Dummy;
  volatile U32 CmdDly;

  LPC_SC->PCONP     |= (1 << 11);   // Turn on EMC peripheral clock
  LPC_SC->EMCDLYCTL  = 0x00001010;
  LPC_EMC->Control   = 1;  // EMC enable
  LPC_EMC->Config    = 0;
  //
  // Port init
  //
  LPC_IOCON->P3_0  = 1;  // D0
  LPC_IOCON->P3_1  = 1;  // D1
  LPC_IOCON->P3_2  = 1;  // D2
  LPC_IOCON->P3_3  = 1;  // D3
  LPC_IOCON->P3_4  = 1;  // D4
  LPC_IOCON->P3_5  = 1;  // D5
  LPC_IOCON->P3_6  = 1;  // D6
  LPC_IOCON->P3_7  = 1;  // D7
  LPC_IOCON->P3_8  = 1;  // D8
  LPC_IOCON->P3_9  = 1;  // D9
  LPC_IOCON->P3_10 = 1;  // D10
  LPC_IOCON->P3_11 = 1;  // D11
  LPC_IOCON->P3_12 = 1;  // D12
  LPC_IOCON->P3_13 = 1;  // D13
  LPC_IOCON->P3_14 = 1;  // D14
  LPC_IOCON->P3_15 = 1;  // D15
  LPC_IOCON->P3_16 = 1;  // D16
  LPC_IOCON->P3_17 = 1;  // D17
  LPC_IOCON->P3_18 = 1;  // D18
  LPC_IOCON->P3_19 = 1;  // D19
  LPC_IOCON->P3_20 = 1;  // D20
  LPC_IOCON->P3_21 = 1;  // D21
  LPC_IOCON->P3_22 = 1;  // D22
  LPC_IOCON->P3_23 = 1;  // D23
  LPC_IOCON->P3_24 = 1;  // D24
  LPC_IOCON->P3_25 = 1;  // D25
  LPC_IOCON->P3_26 = 1;  // D26
  LPC_IOCON->P3_27 = 1;  // D27
  LPC_IOCON->P3_28 = 1;  // D28
  LPC_IOCON->P3_29 = 1;  // D29
  LPC_IOCON->P3_30 = 1;  // D30
  LPC_IOCON->P3_31 = 1;  // D31

  LPC_IOCON->P4_0  = 1;  // A0
  LPC_IOCON->P4_1  = 1;  // A1
  LPC_IOCON->P4_2  = 1;  // A2
  LPC_IOCON->P4_3  = 1;  // A3
  LPC_IOCON->P4_4  = 1;  // A4
  LPC_IOCON->P4_5  = 1;  // A5
  LPC_IOCON->P4_6  = 1;  // A6
  LPC_IOCON->P4_7  = 1;  // A7
  LPC_IOCON->P4_8  = 1;  // A8
  LPC_IOCON->P4_9  = 1;  // A9
  LPC_IOCON->P4_10 = 1;  // A10
  LPC_IOCON->P4_11 = 1;  // A11
  LPC_IOCON->P4_12 = 1;  // A12
  LPC_IOCON->P4_13 = 1;  // A13
  LPC_IOCON->P4_14 = 1;  // A14
  LPC_IOCON->P4_15 = 1;  // A15
  LPC_IOCON->P4_16 = 1;  // A16
  LPC_IOCON->P4_17 = 1;  // A17
  LPC_IOCON->P4_18 = 1;  // A18
  LPC_IOCON->P4_19 = 1;  // A19
  LPC_IOCON->P4_20 = 1;  // A20
  LPC_IOCON->P4_21 = 1;  // A21
  LPC_IOCON->P4_22 = 1;  // A22
  LPC_IOCON->P4_23 = 1;  // A23

  LPC_IOCON->P4_24 = 1;  // OE
  LPC_IOCON->P4_25 = 1;  // WE
  LPC_IOCON->P4_26 = 1;  // BLS0
  LPC_IOCON->P4_27 = 1;  // BLS1
  LPC_IOCON->P4_28 = 1;  // BLS2
  LPC_IOCON->P4_29 = 1;  // BLS3
  LPC_IOCON->P4_30 = 1;  // CS0
  LPC_IOCON->P4_31 = 1;  // CS1
  LPC_IOCON->P2_14 = 1;  // CS2
  LPC_IOCON->P2_15 = 1;  // CS3
  LPC_IOCON->P2_16 = 1;  // CAS
  LPC_IOCON->P2_17 = 1;  // RAS
  LPC_IOCON->P2_18 = 1;  // CLKOUT0
  LPC_IOCON->P2_19 = 1;  // CLKOUT1
  LPC_IOCON->P2_20 = 1;  // DYCS0
  LPC_IOCON->P2_21 = 1;  // DYCS1
  LPC_IOCON->P2_22 = 1;  // DYCS2
  LPC_IOCON->P2_23 = 1;  // DYCS3
  LPC_IOCON->P2_24 = 1;  // CKEOUT0
  LPC_IOCON->P2_25 = 1;  // CKEOUT1
  LPC_IOCON->P2_26 = 1;  // CKEOUT2
  LPC_IOCON->P2_27 = 1;  // CKEOUT3
  LPC_IOCON->P2_28 = 1;  // DQMOUT0
  LPC_IOCON->P2_29 = 1;  // DQMOUT1
  LPC_IOCON->P2_30 = 1;  // DQMOUT2
  LPC_IOCON->P2_31 = 1;  // DQMOUT3
  //
  // Setup EMC config for SDRAM, timings for 60MHz bus
  //
  LPC_EMC->DynamicConfig0    = 0x00004480;  // 256MB, 8Mx32, 4 banks, 12 rows, 9 columns, buffers disabled
  LPC_EMC->DynamicRasCas0    = 0x00000202;  // 2 RAS, 2 CAS latency */
  LPC_EMC->DynamicReadConfig = 0x00000001;  // Command delayed strategy, using EMCCLKDELAY
  LPC_EMC->DynamicRP         = 0x00000001;  // n + 1 clock cycles
  LPC_EMC->DynamicRAS        = 0x00000003;  // n + 1 clock cycles
  LPC_EMC->DynamicSREX       = 0x00000005;  // n + 1 clock cycles
  LPC_EMC->DynamicAPR        = 0x00000002;  // n + 1 clock cycles
  LPC_EMC->DynamicDAL        = 0x00000003;  // n     clock cycles
  LPC_EMC->DynamicWR         = 0x00000001;  // n + 1 clock cycles
  LPC_EMC->DynamicRC         = 0x00000004;  // n + 1 clock cycles
  LPC_EMC->DynamicRFC        = 0x00000004;  // n + 1 clock cycles
  LPC_EMC->DynamicXSR        = 0x00000005;  // n + 1 clock cycles
  LPC_EMC->DynamicRRD        = 0x00000001;  // n + 1 clock cycles
  LPC_EMC->DynamicMRD        = 0x00000001;  // n + 1 clock cycles
  _DelayMs(100);
  LPC_EMC->DynamicControl    = 0x00000183;  // Issue NOP command
  _DelayMs(200);
  LPC_EMC->DynamicControl    = 0x00000103;  // Issue PALL command
  LPC_EMC->DynamicRefresh    = 0x00000002;  // n * 16 clock cycles
  for (i = 0; i < 0x80; i++);               // Wait 128 AHB clock cycles
  LPC_EMC->DynamicRefresh    = 0x0000003A;  // n * 16 clock cycles
  //
  // Init SDRAM
  //
  LPC_EMC->DynamicControl = 0x00000083;                           // Issue MODE command
  Dummy = *((volatile U32*)(SDRAM_BASE_ADDR | (0x22 << (2+2+9))));  // 4 burst, 2 CAS latency
  LPC_EMC->DynamicControl = 0x00000000;                           // Issue NORMAL command
  LPC_EMC->DynamicConfig0 = 0x00084480;                           // 256MB, 8Mx32, 4 banks, 12 rows, 9 columns, buffers (re-)enabled
  //
  // Auto calibrate timings
  //
  CmdDly = _CalibrateOsc();
  //
  // Find best delay values
  //
  _FindDelay(0);  // EMCDLY
  _FindDelay(1);  // FBCLKDLY
  _AdjustEMCTiming(CmdDly);
}


/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  //
  // Init LEDs
  //
  LPC_IOCON->P2_26  = 0;             // GPIO (LED0)
  LPC_IOCON->P2_27  = 0;             // GPIO (LED1)
  LPC_GPIO2->DIR   |= LED_MASK_ALL;  // Set to output
  //
  // All LEDs off
  //
  LPC_GPIO2->SET    = LED_MASK_ALL;  // Initially clear leds (active low)  
}

/*********************************************************************
*
*       LED switching routines
*       LEDs are switched on with low level on port lines
*/
void BSP_SetLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  LPC_GPIO2->CLR = (1 << (LED0_BIT + Index));        // Active low
}

void BSP_ClrLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  LPC_GPIO2->SET = (1 << (LED0_BIT + Index));        // Active low
}

void BSP_ToggleLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  if (LPC_GPIO2->PIN & (1 << (LED0_BIT + Index))) {  // Active low
    BSP_SetLED(Index);
  } else {
    BSP_ClrLED(Index);
  }
}

/*********************************************************************
*
*       __low_level_init()
*
*       Initialize memory controller, clock generation and pll
*
*       Has to be modified, if another CPU clock frequency should be
*       used. This function is called during startup and
*       has to return 1 to perform segment initialization
*/
#ifdef __cplusplus
extern "C" {
#endif
int __low_level_init(void);  // Avoid "no prototype" warning
#ifdef __cplusplus
  }
#endif
int __low_level_init(void) {
  SystemCoreClockUpdate();  // Ensure, the SystemCoreClock is set
  //
  // Setup the systick timer with a 1ms delay
  // ISR located in GUI\GUI_X.c
  //
  SysTick_Config(SystemCoreClock/1000);  
  //
  // Init SDRAM, NAND- and NOR-flash
  //
  _EMC_Init();
  //
  //  Perform other initialization here, if required
  //
  return 1;
}

/****** End of File *************************************************/
