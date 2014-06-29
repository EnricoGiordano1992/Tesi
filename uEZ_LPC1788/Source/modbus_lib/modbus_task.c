
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

/***********************************
*
* DECLARATION SECTION
*
************************************/

extern GUI_value new_GUI_value;
extern _modbus_rx modbus_rx;
void modbus_task();
void reset_modbus_struct();



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


void reset_modbus_data()
{
   int i = 0;
  
   for (i=0; i < 125; i++)
      modbus_rx.data_converted[i]=0;

}