
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
void modbus_task();




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

        //write holding registers
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