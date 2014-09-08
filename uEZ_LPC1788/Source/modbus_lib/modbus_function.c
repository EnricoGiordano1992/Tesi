
#include "modbus.h"
#include "CMSIS/LPC1788.h"
#include "CMSIS_modbus/lpc177x_8x_uart.h"
#include "CMSIS_modbus/lpc177x_8x_timer.h"
#include "CMSIS_modbus/bsp.h"
#include "port.h"
#include "modbus.h"
#include "init.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <uEZ.h>
/****************************************************************************************************************/

_modbus_rx modbus_rx;

char Message[256] = {0};
int indix = 0;

BOOL char_received = FALSE;

/* Table of CRC values for high�order byte */
const unsigned char modbus_auchCRCHi[] = {
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,
0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,
0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,
0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,
0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,
0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,
0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,
0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,
0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,
0x40
};

/* Table of CRC values for low�order byte */
const char modbus_auchCRCLo[] = {
0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,0x07,0xC7,0x05,0xC5,0xC4,
0x04,0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,0x1E,0xDE,0xDF,0x1F,0xDD,
0x1D,0x1C,0xDC,0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,0xF2,0x32,0x36,0xF6,0xF7,
0x37,0xF5,0x35,0x34,0xF4,0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,0xEB,0x2B,0x2A,0xEA,0xEE,
0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,
0x62,0x66,0xA6,0xA7,0x67,0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,0x78,0xB8,0xB9,0x79,0xBB,
0x7B,0x7A,0xBA,0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,0x50,0x90,0x91,
0x51,0x93,0x53,0x52,0x92,0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,0x88,
0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,0x43,0x83,0x41,0x81,0x80,
0x40
};

/*Stages of MODBUS reception.  Used to keep our ISR fast enough.*/
enum {MODBUS_GETADDY=0, MODBUS_GETFUNC=1, MODBUS_GETDATA=2} modbus_serial_state = 0;

int32_t modbus_serial_wait=MODBUS_SERIAL_TIMEOUT;

BOOL modbus_serial_new= FALSE;

BOOL frame_received = FALSE;

int i = 0;

volatile uint32_t timer0_counter = 0;

/*Global value holding our current CRC value.*/
union
{
   int8_t b[2];
   int16_t d;
} modbus_serial_crc;






/**************************************************************************************************************************/
/***********************************
*
* UTILITY SECTION
*
************************************/

void delay_us(uint32_t delayInMs)
{
		UEZTaskDelay(delayInMs);

}

unsigned int make8(unsigned int var,unsigned int offset)
{
	return ((unsigned int)var >> (offset * 8)) & 0x00FF;
}


void MODBUS_SERIAL_WAIT_FOR_RESPONSE()
{
        modbus_serial_state=MODBUS_GETADDY;
        modbus_serial_new = TRUE;

        while(--modbus_serial_wait >= 0)
          //se non ci sono caratteri disponibili, aspetto o vado in timeout
          if(!modbus_kbhit())
            delay_us(1);
          else {
          //resetto il timer per 30 ms e mi preparo per aspettare di nuovo
            modbus_serial_wait=30;
					}
        //se non ho letto nulla
        if(frame_received == FALSE)
           //errore!
           modbus_rx.error=TIMEOUT;

        else
           modbus_rx.error = 0;

				modbus_calc_crc('0');

				//crc_high=modbus_serial_crc.b[1];
				//crc_low=modbus_serial_crc.b[0];

				if(modbus_serial_crc.b[1] != Message[indix] || modbus_serial_crc.b[0] != Message[indix-1])
					modbus_rx.error = TIMEOUT;

        modbus_serial_wait = MODBUS_SERIAL_TIMEOUT;
        modbus_serial_new = TRUE;
        frame_received = FALSE;

}




// Purpose:    Initialize RS232 communication. Call this before
//             using any other RS232 functions.
// Inputs:     None
// Outputs:    None
void modbus_init()
{

   uart_init();
   modbus_rx.error=TIMEOUT;

}




// Purpose:    Calculate crc of data and updates global crc
// Inputs:     Character
// Outputs:    None
void modbus_calc_crc(char data)
{


	  uint16_t crc = 0xFFFF;
	  int pos;
	  int i;

	  for (pos = 0; pos < indix; pos++) {
	    crc ^= Message[pos];          // XOR byte into least sig. byte of crc

	    for (i = 8; i != 0; i--) {    // Loop over each bit
	      if ((crc & 0x0001) != 0) {      // If the LSB is set
	        crc >>= 1;                    // Shift right and XOR 0xA001
	        crc ^= 0xA001;
	      }
	      else                            // Else LSB is not set
	        crc >>= 1;                    // Just shift right
	    }
	  }

	  modbus_serial_crc.b[0] = crc / 256;
	  modbus_serial_crc.b[1] = crc % 256;


}



// Purpose:    Puts a character onto the serial line
// Inputs:     Character
// Outputs:    None
void modbus_serial_putc(int8_t c)
{
	Message[indix] = c;
	indix++;

	UART_SendByte(_LPC_UART, c);

}



void uart_rx_interrupt(void)
{

   char c;

   c = UART_ReceiveByte(_LPC_UART);


      if(modbus_serial_state == MODBUS_GETADDY)
      {
				 indix = 0;
         modbus_serial_crc.d = 0xFFFF;
         modbus_rx.address = c;
         modbus_serial_state++;
         modbus_rx.len = 0;
         modbus_rx.error=0;
      }
      else if(modbus_serial_state == MODBUS_GETFUNC)
      {
         modbus_rx.func = c;
         modbus_serial_state++;
      }
      else if(modbus_serial_state == MODBUS_GETDATA)
      {
         if (modbus_rx.len>=MODBUS_SERIAL_RX_BUFFER_SIZE) {modbus_rx.len=MODBUS_SERIAL_RX_BUFFER_SIZE-1;}
         modbus_rx.data[modbus_rx.len]=c;
         modbus_rx.len++;
      }

      Message[indix++] = c;

  modbus_serial_new = FALSE;
  frame_received = TRUE;

}


// Purpose:   Interrupt service routine for handling incoming serial data
// Inputs:    None
// Outputs:   None
void _UART_IRQHander(void)
{
	uint32_t intsrc, tmp, tmp1;

	/* Determine the interrupt source */
	intsrc = UART_GetIntId(_LPC_UART);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(_LPC_UART);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
			  modbus_rx.error = tmp1;
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
		  uart_rx_interrupt();
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
	}

}


// Purpose:    Send a message over the RS485 bus
// Inputs:     1) The destination address
//             2) The number of bytes of data to send
//             3) A pointer to the data to send
//             4) The length of the data
// Outputs:    TRUE if successful
//             FALSE if failed
// Note:       Format:  source | destination | data-length | data | checksum
void modbus_serial_send_start(int8_t to, int8_t func)
{

   indix = 0;

   modbus_serial_crc.d=0xFFFF;
   modbus_serial_new=TRUE;

   
   modbus_serial_putc(to);
   modbus_serial_putc(func);
}

void modbus_serial_send_stop()
{

   int8_t crc_low, crc_high;

   modbus_calc_crc('0');

   crc_high=modbus_serial_crc.b[1];
   crc_low=modbus_serial_crc.b[0];

   modbus_serial_putc(crc_high);
   modbus_serial_putc(crc_low);
    
   indix = 0;

   modbus_serial_crc.d=0xFFFF;


}




// Purpose:    Get a message from the RS485 bus and store it in a buffer
// Inputs:     None
// Outputs:    TRUE if a message was received
//             FALSE if no message is available
// Note:       Data will be filled in at the modbus_rx struct:
BOOL modbus_kbhit()
{

	BOOL ret;
  if(!modbus_serial_new)
      ret = FALSE;

	else{
		   modbus_serial_new=FALSE;
			ret = TRUE;
	}

   if(modbus_rx.func & 0x80)           //did we receive an error?
   {
      modbus_rx.error = modbus_rx.data[0];  //if so grab the error and return true
      modbus_rx.len = 1;
   }

		return ret;
}





/***********************************
*
* MODBUS SECTION
*
************************************/
/*MODBUS Master Functions*/



/********************************************************************
The following functions are defined in the MODBUS protocol.  Please
refer to http://www.modbus.org for the purpose of each of these.
All functions take the slaves address as their first parameter.
Each function returns the exception code received from the response.
The function will return 0 if there were no errors in transmission.
********************************************************************/

/*
read_coils
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start reading from
           int16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_coils(int8_t address, int16_t start_address, int16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_COILS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}


/*
read_discrete_input
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start reading from
           int16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_discrete_input(int8_t address, int16_t start_address, int16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_DISCRETE_INPUT);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();
      
   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_holding_registers
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start reading from
           int16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_holding_registers(int8_t address, int16_t start_address, int16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_HOLDING_REGISTERS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();
   
   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_input_registers
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start reading from
           int16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_input_registers(int8_t address, int16_t start_address, int16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_INPUT_REGISTERS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();
   
   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
write_single_coil
Input:     int8_t       address            Slave Address
           int16_t      output_address     Address to write into
           int1       on                 true for on, false for off
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_single_coil(int8_t address, int16_t output_address, BOOL on)
{
   modbus_serial_send_start(address, FUNC_WRITE_SINGLE_COIL);

   modbus_serial_putc(make8(output_address,1));
   modbus_serial_putc(make8(output_address,0));

   if(on)
       modbus_serial_putc(0xFF);
   else
       modbus_serial_putc(0x00);
   
   modbus_serial_putc(0x00);

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
write_single_register
Input:     int8_t       address            Slave Address
           int16_t      reg_address        Address to write into
           int16_t      reg_value          Value to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_single_register(int8_t address, int16_t reg_address, int16_t reg_value)
{
   modbus_serial_send_start(address, FUNC_WRITE_SINGLE_REGISTER);

   modbus_serial_putc(make8(reg_address,1));
   modbus_serial_putc(make8(reg_address,0));

   modbus_serial_putc(make8(reg_value,1));
   modbus_serial_putc(make8(reg_value,0));

   modbus_serial_send_stop();
   
   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_exception_status
Input:     int8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_exception_status(int8_t address)
{
   modbus_serial_send_start(address, FUNC_READ_EXCEPTION_STATUS);
   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
diagnostics
Input:     int8_t       address            Slave Address
           int16_t      sub_func           Subfunction to send
           int16_t      data               Data to send, changes based on subfunction
Output:    exception                     0 if no error, else the exception
*/
exception modbus_diagnostics(int8_t address, int16_t sub_func, int16_t data)
{
   modbus_serial_send_start(address, FUNC_DIAGNOSTICS);

   modbus_serial_putc(make8(sub_func,1));
   modbus_serial_putc(make8(sub_func,0));

   modbus_serial_putc(make8(data,1));
   modbus_serial_putc(make8(data,0));

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
get_comm_event_couter
Input:     int8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_get_comm_event_counter(int8_t address)
{
   modbus_serial_send_start(address, FUNC_GET_COMM_EVENT_COUNTER);
   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
get_comm_event_log
Input:     int8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_get_comm_event_log(int8_t address)
{
   modbus_serial_send_start(address, FUNC_GET_COMM_EVENT_LOG);
   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
write_multiple_coils
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start at
           int16_t      quantity           Amount of coils to write to
           int1*      values             A pointer to an array holding the values to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_multiple_coils(int8_t address, int16_t start_address, int16_t quantity,
                           int8_t *values)
{
   int8_t i,count;
   
   count = (int8_t)((quantity/8));
   
   if(quantity%8)
      count++;      

   modbus_serial_send_start(address, FUNC_WRITE_MULTIPLE_COILS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_putc(count);

   for(i=0; i < count; ++i) 
      modbus_serial_putc(values[i]);

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
write_multiple_registers
Input:     int8_t       address            Slave Address
           int16_t      start_address      Address to start at
           int16_t      quantity           Amount of coils to write to
           int16_t*     values             A pointer to an array holding the data to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_multiple_registers(int8_t address, int16_t start_address, int16_t quantity,
                           int16_t *values)
{
   int8_t i,count;
   
   count = quantity*2;

   modbus_serial_send_start(address, FUNC_WRITE_MULTIPLE_REGISTERS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));
   
   modbus_serial_putc(count);

   for(i=0; i < quantity; ++i)
   {
      modbus_serial_putc(make8(values[i],1));
      modbus_serial_putc(make8(values[i],0));
   }

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
report_slave_id
Input:     int8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_report_slave_id(int8_t address)
{
   modbus_serial_send_start(address, FUNC_REPORT_SLAVE_ID);
   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_file_record
Input:     int8_t                address            Slave Address
           int8_t                byte_count         Number of bytes to read
           read_sub_request*   request            Structure holding record information
Output:    exception                              0 if no error, else the exception
*/
exception modbus_read_file_record(int8_t address, int8_t byte_count,
                            modbus_read_sub_request *request)
{
   int8_t i;

   modbus_serial_send_start(address, FUNC_READ_FILE_RECORD);

   modbus_serial_putc(byte_count);

   for(i=0; i < (byte_count/7); i+=7)
   {
      modbus_serial_putc(request->reference_type);
      modbus_serial_putc(make8(request->file_number, 1));
      modbus_serial_putc(make8(request->file_number, 0));
      modbus_serial_putc(make8(request->record_number, 1));
      modbus_serial_putc(make8(request->record_number, 0));
      modbus_serial_putc(make8(request->record_length, 1));
      modbus_serial_putc(make8(request->record_length, 0));
      request++;
   }

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
write_file_record
Input:     int8_t                address            Slave Address
           int8_t                byte_count         Number of bytes to read
           read_sub_request*   request            Structure holding record/data information
Output:    exception                              0 if no error, else the exception
*/
exception modbus_write_file_record(int8_t address, int8_t byte_count,
                            modbus_write_sub_request *request)
{
   int8_t i, j=0;

   modbus_serial_send_start(address, FUNC_WRITE_FILE_RECORD);

   modbus_serial_putc(byte_count);

   for(i=0; i < byte_count; i+=(7+(j*2)))
   {
      modbus_serial_putc(request->reference_type);
      modbus_serial_putc(make8(request->file_number, 1));
      modbus_serial_putc(make8(request->file_number, 0));
      modbus_serial_putc(make8(request->record_number, 1));
      modbus_serial_putc(make8(request->record_number, 0));
      modbus_serial_putc(make8(request->record_length, 1));
      modbus_serial_putc(make8(request->record_length, 0));

      for(j=0; (j < request->record_length) && 
            (j < MODBUS_SERIAL_RX_BUFFER_SIZE-8); j+=2)
      {
         modbus_serial_putc(make8(request->data[j], 1));
         modbus_serial_putc(make8(request->data[j], 0));
      }
      request++;
   }

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
mask_write_register
Input:     int8_t       address            Slave Address
           int16_t      reference_address  Address to mask
           int16_t      AND_mask           A mask to AND with the data at reference_address
           int16_t      OR_mask            A mask to OR with the data at reference_address
Output:    exception                              0 if no error, else the exception
*/
exception modbus_mask_write_register(int8_t address, int16_t reference_address,
                           int16_t AND_mask, int16_t OR_mask)
{
   modbus_serial_send_start(address, FUNC_MASK_WRITE_REGISTER);

   modbus_serial_putc(make8(reference_address,1));
   modbus_serial_putc(make8(reference_address,0));

   modbus_serial_putc(make8(AND_mask,1));
   modbus_serial_putc(make8(AND_mask,0));

   modbus_serial_putc(make8(OR_mask, 1));
   modbus_serial_putc(make8(OR_mask, 0));

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_write_multiple_registers
Input:     int8_t       address                Slave Address
           int16_t      read_start             Address to start reading
           int16_t      read_quantity          Amount of registers to read
           int16_t      write_start            Address to start writing
           int16_t      write_quantity         Amount of registers to write
           int16_t*     write_registers_value  Pointer to an aray us to write
Output:    exception                         0 if no error, else the exception
*/
exception modbus_read_write_multiple_registers(int8_t address, int16_t read_start,
                                    int16_t read_quantity, int16_t write_start,
                                    int16_t write_quantity,
                                    int16_t *write_registers_value)
{
   int8_t i;

   modbus_serial_send_start(address, FUNC_READ_WRITE_MULTIPLE_REGISTERS);

   modbus_serial_putc(make8(read_start,1));
   modbus_serial_putc(make8(read_start,0));

   modbus_serial_putc(make8(read_quantity,1));
   modbus_serial_putc(make8(read_quantity,0));

   modbus_serial_putc(make8(write_start, 1));
   modbus_serial_putc(make8(write_start, 0));

   modbus_serial_putc(make8(write_quantity, 1));
   modbus_serial_putc(make8(write_quantity, 0));

   modbus_serial_putc((int8_t)(2*write_quantity));

   for(i=0; i < write_quantity ; i+=2)
   {
      modbus_serial_putc(make8(write_registers_value[i], 1));
      modbus_serial_putc(make8(write_registers_value[i+1], 0));
   }

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

/*
read_FIFO_queue
Input:     int8_t       address           Slave Address
           int16_t      FIFO_address      FIFO address
Output:    exception                    0 if no error, else the exception
*/
exception modbus_read_FIFO_queue(int8_t address, int16_t FIFO_address)
{
   modbus_serial_send_start(address, FUNC_READ_FIFO_QUEUE);

   modbus_serial_putc(make8(FIFO_address, 1));
   modbus_serial_putc(make8(FIFO_address, 0));

   modbus_serial_send_stop();

   MODBUS_SERIAL_WAIT_FOR_RESPONSE();

   return modbus_rx.error;
}

