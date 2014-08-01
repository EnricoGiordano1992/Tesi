#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "LPC17xx.h"
#include "port.h"

typedef struct sens{

	USHORT  mic;
	USHORT  vibro;
	USHORT  distance1;
	USHORT  distance2;
	USHORT  lumino;

}Sensors;

Sensors check_all_sensors();

USHORT check_mic();
USHORT check_vibro();
USHORT check_distance1();
USHORT check_distance2();
USHORT check_lumino();

#endif
