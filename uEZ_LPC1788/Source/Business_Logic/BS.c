#include <BS.h>
#include "modbus.h"

//defines
#define ID_EDIT_0_LEDS  (GUI_ID_USER + 0x0A)

//Executor
void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(WM_MESSAGE *), WM_MESSAGE *obj);
void BS_pre_exec(void (*f)(void));
void BS_post_exec(void (*f)(void *), void *obj);

//Internal private functions
void switch_context_to_leds_controller();
void switch_context_to_leds_window();
void switch_context_to_sensors_controller();
void switch_context_to_sensors_window();
void switch_context_to_debug_modbus_controller();
void switch_context_to_debug_modbus_window();

void init_leds_controller();
void init_leds_window(WM_MESSAGE *pMsg);
void activate_leds_subtask(WM_MESSAGE *pMsg);

//GUI Interfaces
extern WM_HWIN ExecLedControl(void);
extern WM_HWIN ExecSensor_Control(void);
extern WM_HWIN ExecModbus_Master_testWindow(void);

//external functions
extern void modbus_led_check();
extern T_uezTaskFunction poll_led_check (T_uezTask aTask, void *aParameters);

//external variables
extern _modbus_rx modbus_rx;

//internal variabiles
BOOL led_status[7] = {FALSE};
BOOL exit_thread_led_control;
BOOL changed;

//handler al task modbus
T_uezTask poll_led_t;


void BS_wrapper(command signal, recipient rec, void *obj){


	switch(rec){

		//chiamate interne (modifica interfaccia, valori, ecc)
		case INTERNAL:
			switch(signal){
				case EXIT:
					break;
				
				//MENU'
				case SWITCH_CONTEXT_TO_LEDS_CONTROLLER:
					BS_exec(switch_context_to_leds_controller, switch_context_to_leds_window, NULL);
					break;
				
				case SWITCH_CONTEXT_TO_SENSORS_CONTROLLER:
					BS_exec(switch_context_to_sensors_controller, switch_context_to_sensors_window, NULL);
					break;

				case SWITCH_CONTEXT_TO_DEBUG_MODBUS_CONTROLLER:
					BS_exec(switch_context_to_debug_modbus_controller, switch_context_to_debug_modbus_window, NULL);
					break;
				
				//LEDS CONTROL
				
				//UNKNOWN
				default:
					break;
			}
			break;
			
		//chiamate esterne (invio di valori)
		case EXTERNAL:
			switch(signal){
			//TODO JSON
				//LEDS CONTROL
				case INIT_LEDS_CONTROLLER:
					BS_pre_exec(modbus_led_check);
					BS_exec(init_leds_controller, init_leds_window, (WM_MESSAGE *)obj);
					BS_post_exec(activate_leds_subtask, obj);
					break;
			}
			break;
		
		default:
			//chiamata anomala
			break;
	}

}



void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(WM_MESSAGE *), WM_MESSAGE *obj){

	(*f_for_tcp)();
	(*f_for_gui)(obj);
}

void BS_pre_exec(void (*f)(void)){
	(*f)();
}

void BS_post_exec(void (*f)(void *), void *obj){
	(*f)(obj);
}



void switch_context_to_leds_controller(){
	;
}

void switch_context_to_leds_window(){

		if (UEZTaskCreate(ExecLedControl, "ExecLedControl", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
			; //anomalia
}

void switch_context_to_sensors_controller(){
	;
}

void switch_context_to_sensors_window(){

	if (UEZTaskCreate(ExecSensor_Control, "ExecSensor_Control", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
			; //anomalia

}

void switch_context_to_debug_modbus_controller(){
		;
}

void switch_context_to_debug_modbus_window(){

	if (UEZTaskCreate(ExecModbus_Master_testWindow, "ExecModbus_Master_testWindow", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
			; //anomalia
	
}

void init_leds_controller(){
	;
}

void init_leds_window(WM_MESSAGE *pMsg){
	WM_HWIN hItem;
	int i;
	
	for(i = 0; i < 7; i++)
	if(bit_test(modbus_rx.data_converted[0],i)){
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + i);
				EDIT_SetText(hItem, " 0N");
				led_status[i+1] = TRUE;
	}
	else{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + i);
				EDIT_SetText(hItem, " 0FF");
				led_status[i+1] = FALSE;
		
	}
}

void activate_leds_subtask(WM_MESSAGE *pMsg){
	exit_thread_led_control = FALSE;
	UEZTaskCreate((T_uezTaskFunction)poll_led_check, "poll_led_check", 512,(void *) pMsg->hWin , UEZ_PRIORITY_LOW, &poll_led_t);					
}


