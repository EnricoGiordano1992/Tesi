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





