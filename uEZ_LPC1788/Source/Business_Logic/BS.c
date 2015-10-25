#include <BS.h>
#include "modbus.h"

//defines
#define ID_EDIT_0_LEDS  (GUI_ID_USER + 0x0A)

//Executor
void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(WM_MESSAGE *), WM_MESSAGE *obj);
void BS_pre_exec(void (*f)(void *), void *obj);
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
void stop_leds_subtask();
void exit_to_leds_controller();
void exit_to_leds_window(WM_MESSAGE *pMsg);
void change_led_status_modbus_task();
void update_led_status_controller();
void update_led_status_window(WM_MESSAGE *pMsg);
void autoupdate_leds_status_controller();
void autoupdate_leds_status_window(WM_MESSAGE *pMsg);

//GUI Interfaces
extern WM_HWIN ExecLedControl(void);
extern WM_HWIN ExecSensor_Control(void);
extern WM_HWIN ExecModbus_Master_testWindow(void);

//external functions
extern void modbus_led_check();
extern T_uezTaskFunction poll_led_check (T_uezTask aTask, void *aParameters);
extern void modify_label(WM_MESSAGE *msg);
extern void modify_label_sensors(WM_MESSAGE *msg);
extern void modify_edit_text(int led, BOOL status, char *string);

//external variables
extern _modbus_rx modbus_rx;

//internal variabiles
BOOL led_status[7] = {FALSE};
BOOL exit_thread_led_control;
BOOL changed;
int led;
int led_value;

//handler al task modbus
T_uezTask poll_led_t;


void BS_wrapper(command signal, recipient rec, void *obj){

	int i;

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
				case EXIT_TO_LEDS_CONTROLLER:
					BS_pre_exec(stop_leds_subtask, NULL);
					BS_exec(exit_to_leds_controller, exit_to_leds_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_1:
					led = 1;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_2:
					led = 2;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_3:
					led = 3;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_4:
					led = 4;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_5:
					led = 5;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;

				case CHANGE_LED_STATUS_6:
					led = 6;
					BS_pre_exec(change_led_status_modbus_task, NULL);
					BS_exec(update_led_status_controller, update_led_status_window, (WM_MESSAGE *)obj);
					break;
				
				//UNKNOWN
				default:
					break;
			}
			break;
			
		//chiamate esterne (invio di valori)
		case EXTERNAL:
			switch(signal){
				//LEDS CONTROL
				case INIT_LEDS_CONTROLLER:
					BS_pre_exec(modbus_led_check, NULL);
					BS_exec(init_leds_controller, init_leds_window, (WM_MESSAGE *)obj);
					BS_post_exec(activate_leds_subtask, obj);
					break;
				
				case UPDATE_LEDS_STATUS:
					BS_exec(autoupdate_leds_status_controller, autoupdate_leds_status_window, (WM_MESSAGE *)obj);
					break;

			}
			break;
		
		default:
			//chiamata anomala
			break;
	}

}



//FUNCTIONS

void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(WM_MESSAGE *), WM_MESSAGE *obj){

	(*f_for_tcp)();
	(*f_for_gui)(obj);
}

void BS_pre_exec(void (*f)(void *), void *obj){
	(*f)(obj);
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

void stop_leds_subtask(){
	UEZTaskDelete(poll_led_t);
	exit_thread_led_control = TRUE;

}

void exit_to_leds_controller(){
	;
}

void exit_to_leds_window(WM_MESSAGE *pMsg){
	GUI_EndDialog(pMsg->hWin, 0);	
}

void change_led_status_modbus_task(){
		modbus_led_task(led, led_status[led]);
}

void update_led_status_controller(){
	;
}

void update_led_status_window(WM_MESSAGE *pMsg){
	WM_HWIN hItem;
	
	led_status[led] = !led_status[led];
			
	if(led_status[led]){
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + led - 1);
		EDIT_SetText(hItem, " 0N");						
		}
	else{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + led - 1);
		EDIT_SetText(hItem, " 0FF");						
		}		
}

void autoupdate_leds_status_window(WM_MESSAGE *pMsg){
	int pos_led;
	WM_HWIN hItem;
	BOOL value;

	pos_led = pMsg->Data.v/10;
	if(pMsg->Data.v%10 == 1)
		value = TRUE;
	else
		value = FALSE;

	led_status[led] = led_value;
			
	if(!exit_thread_led_control){
		if(led_status[led]){
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + led - 1);
			EDIT_SetText(hItem, " 0N");						
		}
		else{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0_LEDS + led - 1);
			EDIT_SetText(hItem, " 0FF");						
		}
	}		
}


void autoupdate_leds_status_controller(){
	;
}



