#ifndef _BS_H_
#define _BS_H_

#include <uEZ.h>
#include "DIALOG.h"

typedef enum command{

	EXIT,
	
	CHANGE_TO_LED_CONTROLLER,
	CHANGE_TO_SENSOR_CONTROLLER,
	
	SET_LED_1_ON,
	SET_LED_1_OFF,
	SET_LED_2_ON,
	SET_LED_2_OFF,
	SET_LED_3_ON,
	SET_LED_3_OFF,
	SET_LED_4_ON,
	SET_LED_4_OFF,
	SET_LED_5_ON,
	SET_LED_5_OFF,
	SET_LED_6_ON,
	SET_LED_6_OFF,

	GET_TEMPERATURE
	
}command;


typedef enum origin{

	PC,
	TOUCH
	
}origin;


typedef struct message{
	int i;
}message;



void BS_wrapper(command signal);

#endif