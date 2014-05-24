#ifndef _MODBUS_PORT_H_
#define _MODBUS_PORT_H_

void delay_us(uint32_t delayInMs);
void TimersEnable(  );
void TimersDisable(  );
void RCV_ON(void);
void RCV_OFF(void);
void modbus_enable_timeout(BOOL enable);
void timer_interrupt(void);
void modbus_serial_putc(int8_t c);
void uart_rx_interrupt(void);









#endif