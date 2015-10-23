#include <BS.h>

void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(void));

void change_to_leds_controller();
void change_to_leds_window();

extern WM_HWIN ExecLedControl(void);


void BS_wrapper(command signal){

		message m;
	
		switch(signal){
			case EXIT:
				break;
			
			case CHANGE_TO_LED_CONTROLLER:
				BS_exec(change_to_leds_controller, change_to_leds_window);
				break;
			
			case CHANGE_TO_SENSOR_CONTROLLER:
				break;
			
			default:
				break;
		}

}



void BS_exec(void (*f_for_tcp)(void), void (*f_for_gui)(void)){

	(*f_for_tcp)();
	(*f_for_gui)();
}

void change_to_leds_controller(){
	;
}

void change_to_leds_window(){

		if (UEZTaskCreate(ExecLedControl, "ExecLedControl", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
			; //anomalia
}

