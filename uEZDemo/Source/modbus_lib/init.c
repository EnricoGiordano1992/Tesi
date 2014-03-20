/*
 * init.c
 *
 *  Created on: 01/ott/2013
 *      Author: enrico
 */

#include <CMSIS/LPC177x_8x.h>
#include <CMSIS_modbus/lpc177x_8x_uart.h>
#include <CMSIS_modbus/lpc177x_8x_timer.h>
#include <CMSIS_modbus/lpc177x_8x_pinsel.h>
#include <CMSIS_modbus/bsp.h>

#include "port.h"

#include "init.h"


//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;
FunctionalState LEDStatus = ENABLE;

// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;




void timer_init()
{

	// Conifg P1.28 as MAT0.0
	PINSEL_ConfigPin(BRD_TIM_CAP_LINKED_PORT, BRD_TIM_CAP_LINKED_PIN, 3);

	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 100;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz)
	TIM_MatchConfigStruct.MatchValue   = 4000;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(BRD_TIMER_USED, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(BRD_TIMER_USED, &TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(BRD_TIM_INTR_USED, ((0x01<<3)|0x01));

	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(BRD_TIM_INTR_USED);


	// To start timer
//	TIM_Cmd(BRD_TIMER_USED, ENABLE);

}


void uart_init()
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;


#if (UART_TEST_NUM == 0)
	/*
	 * Initialize UART0 pin connect
	 * P0.2: U0_TXD
	 * P0.3: U0_RXD
	 */
	PINSEL_ConfigPin(0,2,1);
	PINSEL_ConfigPin(0,3,1);
#elif (UART_TEST_NUM == 1)
	/*
	 * Initialize UART1 pin connect
	 * P0.15: U1_TXD
	 * P0.16: U1_RXD
	 */
	PINSEL_ConfigPin(0,15,1);
	PINSEL_ConfigPin(0,16,1);
#elif (UART_TEST_NUM == 2)
	/*
	 * Initialize UART2 pin connect
	 * P0.10: U2_TXD
	 * P0.11: U2_RXD
	 */
	PINSEL_ConfigPin(0,10,1);
	PINSEL_ConfigPin(0,11,1);

#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 38400bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	UARTConfigStruct.Baud_rate = 38400;

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)_LPC_UART, &UARTConfigStruct);


	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig((LPC_UART_TypeDef *)_LPC_UART, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)_LPC_UART, ENABLE);

    /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)_LPC_UART, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)_LPC_UART, UART_INTCFG_RLS, ENABLE);
	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
	TxIntStat = RESET;


    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(_UART_IRQ, ((0x01<<3)|0x01));

	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(_UART_IRQ);

}





