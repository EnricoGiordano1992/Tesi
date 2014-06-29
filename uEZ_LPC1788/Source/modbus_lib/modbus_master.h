/*
 * modbus_master.h
 *
 *  Created on: 25/ago/2013
 *      Author: enrico
 */

#ifndef MODBUS_MASTER_H_
#define MODBUS_MASTER_H_

#include "modbus.h"


int8_t swap_bits(uint8_t c);
exception read_all_coils();
exception read_all_inputs();
exception read_all_holding();
exception read_all_input_reg();
void write_coil();
void write_reg();
void write_coils();
void write_regs();
void unknown_func();
void parse_read(char c);
void parse_write(char c);


#endif /* MODBUS_MASTER_H_ */
