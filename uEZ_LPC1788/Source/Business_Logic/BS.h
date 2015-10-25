#ifndef _BS_H_
#define _BS_H_

#include <uEZ.h>
#include "DIALOG.h"

typedef enum command{

	EXIT,
	
	SWITCH_CONTEXT_TO_LEDS_CONTROLLER,
	SWITCH_CONTEXT_TO_SENSORS_CONTROLLER,
	SWITCH_CONTEXT_TO_DEBUG_MODBUS_CONTROLLER,
	
	INIT_LEDS_CONTROLLER,
	EXIT_TO_LEDS_CONTROLLER,
	CHANGE_LED_STATUS_1,
	CHANGE_LED_STATUS_2,
	CHANGE_LED_STATUS_3,
	CHANGE_LED_STATUS_4,
	CHANGE_LED_STATUS_5,
	CHANGE_LED_STATUS_6,
	UPDATE_LEDS_STATUS,
	
	GET_TEMPERATURE
	
}command;


//destinatario chiamata
typedef enum recipient{

	INTERNAL,
	EXTERNAL
	
}recipient;


typedef struct message{
	int i;
}message;



void BS_wrapper(command signal, recipient rec, void *obj);

#endif