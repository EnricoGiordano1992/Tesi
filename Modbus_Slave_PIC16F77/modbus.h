/*
 * modbus.h
 *
 *  Created on: 24/ago/2013
 *      Author: enrico
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include "driver/UART.h"
#include "driver/TIMER.h"
#include "driver/delay.h"
#include "port.h"
#include "Includes.h"


/*Some defines so we can use identifiers to set things up*/

#define MODBUS_ADDRESS 0x01

#define HOLDING_ADDR    2000

#define MODBUS_SERIAL_BAUD 38400

#ifndef MODBUS_SERIAL_TIMEOUT
#define MODBUS_SERIAL_TIMEOUT      3     //in us
#endif


#ifndef MODBUS_SERIAL_RX_BUFFER_SIZE
#define MODBUS_SERIAL_RX_BUFFER_SIZE  64      //size of send/rcv buffer
#endif


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

//  All slave API functions return 0 on success.

void modbus_read_coils_rsp(int8_t address, int8_t byte_count, int8_t* coil_data);
void modbus_read_discrete_input_rsp(int8_t address, int8_t byte_count,
                                    int8_t *input_data);

void modbus_read_holding_registers_rsp(int8_t address, int8_t byte_count,
                                        int8_t *reg_data);

void modbus_read_input_registers_rsp(int8_t address, int8_t byte_count,
                                        int8_t *input_data);

void modbus_write_single_coil_rsp(int8_t address, int16_t output_address,
                                    int16_t output_value);

void modbus_write_single_register_rsp(int8_t address, int16_t reg_address,
                                        int16_t reg_value);


void modbus_write_multiple_coils_rsp(int8_t address, int16_t start_address,
                                        int16_t quantity);

void modbus_write_multiple_registers_rsp(int8_t address, int16_t start_address,
                                            int16_t quantity);

void modbus_read_write_multiple_registers_rsp(int8_t address, int8_t data_len,
                                                int16_t *data);

void modbus_write_multiple_registers_rsp(int8_t address, int16_t start_address,
                                            int16_t quantity);

void modbus_exception_rsp(int8_t address, int16_t func, exception error);

  struct
   {
      int8_t address;
      int8_t len;                                //number of bytes in the message received
      function func;                           //the function of the message received
      exception error;                         //error recieved, if any
      char data[MODBUS_SERIAL_RX_BUFFER_SIZE]; //data of the message received
   } modbus_rx;




   /********************************************************************
   Our receive struct.  This is used when receiving data as a master or
   slave.  Once a message is sent to you with your address, you should
   begin processing that message.  Refer to ex_modbus_slave.c to see
   how to properly use this structure.
   ********************************************************************/

    /*Stages of MODBUS reception.  Used to keep our ISR fast enough.*/
typedef enum {MODBUS_GETADDY=0, MODBUS_GETFUNC=1, MODBUS_GETDATA=2} Modbus_serial_state;

/*Global value holding our current CRC value.*/
typedef union
{
   int8_t b[2];
   int16_t d;
} Modbus_serial_crc;


  unsigned int make8(unsigned int var, unsigned int offset);
  void delay_us (uint32_t delayInMs);



#endif /* MODBUS_H_ */
