
/***********************************
*
* INCLUDE SECTION
*
************************************/

#include "datamodel.h"
#include <uEZ.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modbus.h"

#ifndef ID_EDIT_0
#define ID_EDIT_0  (GUI_ID_USER + 0x0A)
#endif


T_uezTask alarm_t;
int gia_accese = 0;
int gia_spente = 1;

/***********************************
*
* DECLARATION SECTION
*
************************************/

extern void modify_label(WM_MESSAGE *msg);
extern void modify_label_sensors(WM_MESSAGE *msg);
extern void modify_edit_text(int led, BOOL status, char *string);


extern GUI_value new_GUI_value;
extern _modbus_rx modbus_rx;
void modbus_task();
void reset_modbus_struct();
extern BOOL changed;
extern BOOL exit_thread_led_control;
extern int delay_from_slider;
extern Check_Sensor check_sensors;
extern Sensors sensors;
extern int temperature_limit;


/***********************************
*
* FUNCTION SECTION
*
************************************/


void modbus_task()
{
    BOOL coil_value;
    int i, j;

    //creo il messaggio modbus da mandare in base ai parametri di new_gui_value
    //1: decido la funzione
      switch(new_GUI_value.RADIO_value.radio_selection)
      {
        //read holding registers
        case 0:
          modbus_read_holding_registers((int8_t)new_GUI_value.SPINBOX_value.slave_id, 
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_from + 1999,
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_to + 1 - new_GUI_value.SPINBOX_value.multiple_register_from);

        break;

        //write single registers
        case 1:
          modbus_write_single_register((int8_t)new_GUI_value.SPINBOX_value.slave_id, 
                                       (int16_t)new_GUI_value.SPINBOX_value.single_register_number + 1999,
                                       (int16_t)new_GUI_value.SPINBOX_value.write_value);

        break;

        //read single coil
        case 2:
          modbus_read_coils((int8_t)new_GUI_value.SPINBOX_value.slave_id,
                            (int16_t)new_GUI_value.SPINBOX_value.single_register_number,
                             1);

        break;

        //write single coil
        case 3:
          if(new_GUI_value.SPINBOX_value.write_value == 0)
            coil_value = FALSE;
          else
            coil_value = TRUE;

          modbus_write_single_coil((int8_t)new_GUI_value.SPINBOX_value.slave_id,
                                   (int16_t)new_GUI_value.SPINBOX_value.single_register_number,
                                            coil_value);


        break;

        //read multiple coils
        case 4:
          modbus_read_coils((int8_t)new_GUI_value.SPINBOX_value.slave_id,
                            (int16_t)new_GUI_value.SPINBOX_value.multiple_register_from,
                            (int16_t)new_GUI_value.SPINBOX_value.multiple_register_to + 1 - new_GUI_value.SPINBOX_value.multiple_register_from);

        default:
        break;

      }

			if(new_GUI_value.RADIO_value.radio_selection == 0)
				for(i = 0, j = 1; j < modbus_rx.len - 3; i++, j+=2)
					modbus_rx.data_converted[i] = modbus_rx.data[j+1] | modbus_rx.data[j] << 8 ;

			
			else if(new_GUI_value.RADIO_value.radio_selection == 2){
				modbus_rx.data_converted[0] = modbus_rx.data[1];
				i = 1;
			}
			
			else if (new_GUI_value.RADIO_value.radio_selection == 4){
				for(i = 0, j = 1; i < modbus_rx.len - 3 + (new_GUI_value.SPINBOX_value.multiple_register_to - new_GUI_value.SPINBOX_value.multiple_register_from); i++){
					modbus_rx.data_converted[i] = bit_test(modbus_rx.data[j], i);
					if(i!=0)
						if(i%8 == 0)
								j++;
				}
			}
			
      modbus_rx.len = i;

}



void modbus_led_check()
{
	int i,j;
	//i risultati si trovano su modbus rx [2, modbus_rx.len -3]
  modbus_read_coils((int8_t) 1,
		(int16_t)1,
		(int16_t)6);
	
		modbus_rx.data_converted[0] = modbus_rx.data[1];

}


void modbus_sensor_check(){
	
	int i, j;
	
	modbus_read_holding_registers(10, 2000, 6);
	
		for(i = 0, j = 1; j < modbus_rx.len - 3; i++, j+=2)
			modbus_rx.data_converted[i] = modbus_rx.data[j+1] | modbus_rx.data[j] << 8 ;

	modbus_rx.len = i;
	
}



//controlla che, in base al check relativo, debba accendere o spegnere i led
void led_function(){
	
		if(check_sensors.light != 0){
		
		if(sensors.light == 0){
			if(!gia_accese){
				
					PlayAudio(300, 50);				
					PlayAudio(100, 50);

          modbus_write_single_coil(1, 1, 1);
          modbus_write_single_coil(1, 2, 1);
          modbus_write_single_coil(1, 3, 1);
          modbus_write_single_coil(1, 4, 1);
          modbus_write_single_coil(1, 5, 1);
          modbus_write_single_coil(1, 6, 1);
				
					gia_accese = 1;
					gia_spente = 0;
			}
		}		
		else{
			if(!gia_spente){

					PlayAudio(100, 50);				
					PlayAudio(300, 50);
				
          modbus_write_single_coil(1, 1, 0);
          modbus_write_single_coil(1, 2, 0);
          modbus_write_single_coil(1, 3, 0);
          modbus_write_single_coil(1, 4, 0);
          modbus_write_single_coil(1, 5, 0);
          modbus_write_single_coil(1, 6, 0);

					gia_accese = 0;
					gia_spente = 1;

				}
			}
		}
	
}

//attiva gli allarmi selezionati
T_uezTaskFunction alarm_task (T_uezTask aTask, void *aParameters){
	
	if(check_sensors.alarm != 0){
		
		if(sensors.presence == 0){
			while(check_sensors.alarm != 0 && sensors.presence == 0){
				PlayAudio(1000, 50);				
        PlayAudio(1000, 50);				
        PlayAudio(100, 50);
				PlayAudio(100, 50);				
			}
		}
	}
	

	if(check_sensors.temperature != 0){
		
		if(sensors.temperature > temperature_limit)
			while(check_sensors.temperature != 0 && sensors.temperature > temperature_limit ){
				PlayAudio(800, 50);				
        PlayAudio(800, 50);				
        PlayAudio(100, 50);
				PlayAudio(100, 50);				
			}
			
		
	}
	
	return 0;
}



T_uezTaskFunction poll_sensor_check (T_uezTask aTask, void *aParameters){
	
	int i;
	int j;
	WM_MESSAGE msg;

	msg.hWin = (WM_HWIN) aParameters;

	while(1){
		modbus_read_holding_registers(10, 1999, 7);
		
		for(i = 0, j = 1; j < modbus_rx.len - 3; i++, j+=2)
			modbus_rx.data_converted[i] = modbus_rx.data[j+1] | modbus_rx.data[j] << 8 ;

		
		modbus_rx.len = i;


		//estrazione della temperatura
		sensors.temperature = modbus_rx.data_converted[6];
		
		//estrazione dell'umidita'
		sensors.humidity = modbus_rx.data_converted[5];
				
		//estrazione suono
		sensors.mic = modbus_rx.data_converted[3];
		
		//estrazione distanza
		sensors.distance = modbus_rx.data_converted[0];

		//estrazione presenza
		sensors.presence = modbus_rx.data_converted[1];
		
		//estrazione vibrazione
		sensors.vibration = modbus_rx.data_converted[4];

		//estrazione luce
		sensors.light = modbus_rx.data_converted[2];
		
		//se almeno un check allarme e' stato attivato
		if(check_sensors.alarm != 0 || check_sensors.light != 0 || check_sensors.temperature != 0){
					UEZTaskCreate((T_uezTaskFunction)alarm_task, "_alarm_task", 512,(void *) 0 , UEZ_PRIORITY_LOW, &alarm_t);				
					led_function();
		}
		msg.MsgId = MB_MSG_SENSOR;
		
		modify_label_sensors(&msg);
			
		UEZTaskDelay(delay_from_slider);
	}
	
	return 0;
}




T_uezTaskFunction poll_led_check (T_uezTask aTask, void *aParameters){
	
	int i;
	WM_MESSAGE msg;
	msg.hWin = (WM_HWIN) aParameters;
	
	do{
		
		UEZTaskDelay(2000);
		
		modbus_led_check();

			for(i = 0; i < 7; i++)
			if(bit_test(modbus_rx.data_converted[0],i)){
				
				msg.MsgId = MB_MSG_COIL;
				msg.Data.v = ((i+1)*10) + 1;
				modify_label(&msg);
			}
			else{
				msg.MsgId = MB_MSG_COIL;
				msg.Data.v = ((i+1)*10) + 0;
				modify_label(&msg);

			}

	}while(!exit_thread_led_control);

	return 0;	
}




void modbus_led_task(int led, BOOL on)
{
		//se è acceso, lo spegni
		if(on)
			modbus_write_single_coil((int8_t) 1, (int16_t) led, FALSE);

		else
			modbus_write_single_coil((int8_t) 1, (int16_t) led, TRUE);

}



void reset_modbus_data()
{
   int i = 0;
  
   for (i=0; i < 125; i++)
      modbus_rx.data_converted[i]=0;

}