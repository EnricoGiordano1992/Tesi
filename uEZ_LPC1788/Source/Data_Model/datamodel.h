#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include "port.h"
#include "modbus.h"
#include "GUI.h"
#include "DIALOG.h"

#define MB_MSG_COIL 8000

#define MB_MSG_SENSOR 9000

/*********************
*
*   data from GUI
*
***********************/

typedef struct spinbox_val{

  int slave_id;
  int multiple_register_from;
  int multiple_register_to;
  int single_register_number;
  int write_value;

}spinbox_value;

typedef struct button_val{

  BOOL send_button;

}button_value;


typedef struct radio_val{

  int radio_selection;

}radio_value;


typedef struct gui{

  spinbox_value SPINBOX_value;
  button_value BUTTON_value;
  radio_value RADIO_value;

}GUI_value;




#endif
