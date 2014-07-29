#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "LPC17xx.h"

typedef struct sens{

	int mic;
	int vibro;
	int distance1;
	int distance2;
	int lumino;

}Sensors;

Sensors check_all_sensors();

int check_mic();
int check_vibro();
int check_distance1();
int check_distance2();
int check_lumino();

#endif
