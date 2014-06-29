/*
 * init.h
 *
 *  Created on: 01/ott/2013
 *      Author: enrico
 */

#ifndef INIT_H_
#define INIT_H_



#define UART_TEST_NUM		2

#if (UART_TEST_NUM == 0)
#define	_LPC_UART			(LPC_UART_TypeDef *)LPC_UART0
#define _UART_IRQ			UART0_IRQn
#define _UART_IRQHander		UART0_IRQHandler
#elif (UART_TEST_NUM == 1)
#define _LPC_UART			(LPC_UART_TypeDef *)LPC_UART1
#define _UART_IRQ			UART1_IRQn
#define _UART_IRQHander		UART1_IRQHandler
#elif (UART_TEST_NUM == 2)
#define _LPC_UART			LPC_UART2
#define _UART_IRQ			UART2_IRQn
#define _UART_IRQHander		UART2_IRQHandler
#endif

void timer_init();
void uart_init();

#endif /* INIT_H_ */
