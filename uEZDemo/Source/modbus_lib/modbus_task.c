
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
    //creo il messaggio modbus da mandare in base ai parametri di new_gui_value

    //1: decido la funzione
      switch(new_GUI_value.RADIO_value.radio_selection)
      {
        //read holding registers
        case 0:
          modbus_read_holding_registers((int8_t)new_GUI_value.SPINBOX_value.slave_id, 
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_from,
                                        (int16_t)new_GUI_value.SPINBOX_value.multiple_register_to - new_GUI_value.SPINBOX_value.multiple_register_from);
        break;

        //write single registers
        case 1:
/*          modbus_read_holding_registers((int8_t)new_GUI_value.SPIN_val.slave_id, 
                                        (int16_t)new_GUI_value.SPIN_val.multiple_register_from,
                                        (int16_t)new_GUI_value.SPIN_val.multiple_register_to - new_GUI_value.SPIN_val.multiple_register_from;
*/

        break;

        //read single coil
        case 2:

        break;

        //write single coil
        case 3:

        break;

        default:
        break;

      }
 
}


void reset_modbus_struct()
{
   int i = 0;
  
   modbus_rx.address = 0;
   modbus_rx.len = 0;
   modbus_rx.func = 0;
   modbus_rx.error = 0;

   for (i=0; i < MODBUS_SERIAL_RX_BUFFER_SIZE; i++)
      modbus_rx.data[i]=0;

}