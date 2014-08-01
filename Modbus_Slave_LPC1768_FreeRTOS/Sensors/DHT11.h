//
//    FILE: dht11.h
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://playground.arduino.cc/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
//

#ifndef _DHT11_H_
#define _DHT11_H_

#include <LPC17xx.h>
#include "port.h"

#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2

#define DHT11_PIN	LPC_GPIO2	//P2.4

#define OUTPUT 0
#define INPUT 1

#define LOW 0
#define HIGH 1

void pinMode(int mode);
void digitalWrite(int mode);



int read();

typedef struct dht{
	USHORT  humidity;
	USHORT  temperature;
} dht11;


dht11 test_temperature();


#endif
//
// END OF FILE
