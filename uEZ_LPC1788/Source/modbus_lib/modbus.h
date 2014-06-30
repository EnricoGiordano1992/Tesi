/*
 * modbus.h
 *
 *  Created on: 24/ago/2013
 *      Author: enrico
 */

#ifndef _MODBUS_H_
#define _MODBUS_H_

//#include "CMSIS/LPC177x_8x.h"
//#include "CMSIS_modbus/lpc177x_8x_uart.h"
//#include "CMSIS_modbus/lpc177x_8x_timer.h"
//#include "CMSIS_modbus/lpc177x_8x_pinsel.h"

#include "port.h"

/*Some defines so we can use identifiers to set things up*/
#define MODBUS_TYPE_MASTER 99999
#define MODBUS_TYPE_SLAVE 88888
#define MODBUS_INT_RDA 77777
#define MODBUS_INT_RDA2 66666
#define MODBUS_INT_EXT 55555

#ifndef MODBUS_TYPE
#define MODBUS_TYPE MODBUS_TYPE_MASTER
#endif

#ifndef MODBUS_SERIAL_INT_SOURCE
#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_EXT  // Select between external interrupt
#endif                                           // or asynchronous serial interrupt

#define MODBUS_SERIAL_BAUD 38400

#ifndef MODBUS_SERIAL_TIMEOUT
#define MODBUS_SERIAL_TIMEOUT      5000     //in us
#endif


#ifndef MODBUS_SERIAL_RX_BUFFER_SIZE
#define MODBUS_SERIAL_RX_BUFFER_SIZE  64      //size of send/rcv buffer
#endif

#if (MODBUS_TYPE == MODBUS_TYPE_MASTER)
#endif


#define bit_test(var,pos)       ((var) & (1<<(pos)))



#define MODBUS_TYPE MODBUS_TYPE_MASTER
#define MODBUS_SERIAL_TYPE MODBUS_RTU     //use MODBUS_ASCII for ASCII mode
#define MODBUS_SERIAL_RX_BUFFER_SIZE 64
#define MODBUS_SERIAL_BAUD 38400

#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_EXT

/********************************************************************
These exceptions are defined in the MODBUS protocol.  These can be
used by the slave to communicate problems with the transmission back
to the master who can also use these to easily check the exceptions.
The first exception is the only one that is not part of the protocol
specification.  The TIMEOUT exception is returned when no slave
responds to the master's request within the timeout period.
********************************************************************/
typedef enum _exception{ILLEGAL_FUNCTION=1,ILLEGAL_DATA_ADDRESS=2,
ILLEGAL_DATA_VALUE=3,SLAVE_DEVICE_FAILURE=4,ACKNOWLEDGE=5,SLAVE_DEVICE_BUSY=6,
MEMORY_PARITY_ERROR=8,GATEWAY_PATH_UNAVAILABLE=10,GATEWAY_TARGET_NO_RESPONSE=11,
TIMEOUT=12} exception;

/********************************************************************
These functions are defined in the MODBUS protocol.  These can be
used by the slave to check the incomming function.  See
ex_modbus_slave.c for example usage.
********************************************************************/
typedef enum _function{FUNC_READ_COILS=0x01,FUNC_READ_DISCRETE_INPUT=0x02,
FUNC_READ_HOLDING_REGISTERS=0x03,FUNC_READ_INPUT_REGISTERS=0x04,
FUNC_WRITE_SINGLE_COIL=0x05,FUNC_WRITE_SINGLE_REGISTER=0x06,
FUNC_READ_EXCEPTION_STATUS=0x07,FUNC_DIAGNOSTICS=0x08,
FUNC_GET_COMM_EVENT_COUNTER=0x0B,FUNC_GET_COMM_EVENT_LOG=0x0C,
FUNC_WRITE_MULTIPLE_COILS=0x0F,FUNC_WRITE_MULTIPLE_REGISTERS=0x10,
FUNC_REPORT_SLAVE_ID=0x11,FUNC_READ_FILE_RECORD=0x14,
FUNC_WRITE_FILE_RECORD=0x15,FUNC_MASK_WRITE_REGISTER=0x16,
FUNC_READ_WRITE_MULTIPLE_REGISTERS=0x17,FUNC_READ_FIFO_QUEUE=0x18} function;


/********************************************************************
The following structs are used for read/write_sub_request.  These
functions take in one of these structs.
Please refer to the MODBUS protocol specification if you do not
understand the members of the structure.
********************************************************************/
typedef struct _modbus_read_sub_request
{
   int8_t reference_type;
   int16_t file_number;
   int16_t record_number;
   int16_t record_length;
} modbus_read_sub_request;

typedef struct _modbus_write_sub_request
{
   int8_t reference_type;
   int16_t file_number;
   int16_t record_number;
   int16_t record_length;
   int16_t data[MODBUS_SERIAL_RX_BUFFER_SIZE-8];
} modbus_write_sub_request;


void modbus_calc_crc(char data);

  void modbus_init();

  void modbus_serial_send_start(int8_t to, int8_t func);

  void modbus_serial_send_stop();

  BOOL modbus_kbhit();

//  All master API functions return 0 on success.

  exception modbus_read_coils(int8_t address, int16_t start_address, int16_t quantity);

  exception modbus_read_discrete_input(int8_t address, int16_t start_address, int16_t quantity);

  exception modbus_read_holding_registers(int8_t address, int16_t start_address, int16_t quantity);

  exception modbus_read_input_registers(int8_t address, int16_t start_address, int16_t quantity);

  exception modbus_write_single_coil(int8_t address, int16_t output_address, BOOL on);

  exception modbus_write_single_register(int8_t address, int16_t reg_address, int16_t reg_value);

  exception modbus_read_exception_status(int8_t address);

  exception modbus_diagnostics(int8_t address, int16_t sub_func, int16_t data);

  exception modbus_get_comm_event_counter(int8_t address);

  exception modbus_get_comm_event_log(int8_t address);

  exception modbus_write_multiple_coils(int8_t address, int16_t start_address, int16_t quantity,
                             int8_t *values);

  exception modbus_write_multiple_registers(int8_t address, int16_t start_address, int16_t quantity,
                             int16_t *values);

  exception modbus_report_slave_id(int8_t address);

  exception modbus_read_file_record(int8_t address, int8_t byte_count,
                              modbus_read_sub_request *request);

  exception modbus_write_file_record(int8_t address, int8_t byte_count,
                              modbus_write_sub_request *request);

  exception modbus_mask_write_register(int8_t address, int16_t reference_address,
                             int16_t AND_mask, int16_t OR_mask);

  exception modbus_read_write_multiple_registers(int8_t address, int16_t read_start,
                                      int16_t read_quantity, int16_t write_start,
                                      int16_t write_quantity,
                                      int16_t *write_registers_value);

  exception modbus_read_FIFO_queue(int8_t address, int16_t FIFO_address);






   /********************************************************************
   Our receive struct.  This is used when receiving data as a master or
   slave.  Once a message is sent to you with your address, you should
   begin processing that message.  Refer to ex_modbus_slave.c to see
   how to properly use this structure.
   ********************************************************************/
typedef struct
   {
      int8_t address;
      int8_t len;                                //number of bytes in the message received
      function func;                           //the function of the message received
      exception error;                         //error recieved, if any
      int8_t data[MODBUS_SERIAL_RX_BUFFER_SIZE]; //data of the message received
      int16_t data_converted[125];
   }_modbus_rx;



  unsigned int make8(unsigned int var, unsigned int offset);
  void delay_us (uint32_t delayInMs);
  void reset_modbus_struct();


#endif /* MODBUS_H_ */
