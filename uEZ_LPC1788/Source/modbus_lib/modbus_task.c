
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




/***********************************
*
* DECLARATION SECTION
*
************************************/

extern void modify_label(WM_MESSAGE *msg);
extern GUI_value new_GUI_value;
extern _modbus_rx modbus_rx;
void modbus_task();
void reset_modbus_struct();
extern void modify_edit_text(int led, BOOL status, char *string);
extern BOOL changed;
extern BOOL exit_thread_led_control;

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
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_from + 2000,
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_to + 2 - new_GUI_value.SPINBOX_value.multiple_register_from);

        break;

        //write single registers
        case 1:
          modbus_write_single_register((int8_t)new_GUI_value.SPINBOX_value.slave_id, 
                                       (int16_t)new_GUI_value.SPINBOX_value.single_register_number + 2000,
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
                            (int16_t)new_GUI_value.SPINBOX_value.multiple_register_to + 2 - new_GUI_value.SPINBOX_value.multiple_register_from);

        default:
        break;

      }

      for(i = 0, j = 2; j < modbus_rx.len - 3; i++, j+=2)
        modbus_rx.data_converted[i] = modbus_rx.data[j+1] << 8 | modbus_rx.data[j];

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