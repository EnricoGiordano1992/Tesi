/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.20                          *
*        Compiled Mar 19 2013, 15:01:00                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)

#include "BUTTON.h"
#include <BS.h>

// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_BUTTON_0  (GUI_ID_USER + 0x0D)
#define ID_BUTTON_1  (GUI_ID_USER + 0x0E)
#define ID_BUTTON_2  (GUI_ID_USER + 0x0F)
#define ID_HEADER_0  (GUI_ID_USER + 0x10)
#define ID_HEADER_1  (GUI_ID_USER + 0x11)
#define ID_HEADER_2  (GUI_ID_USER + 0x12)


// USER START (Optionally insert additional defines)
#define GUI_ID_TEXT0 (GUI_ID_USER + 0x12)

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmdebug_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmsensor_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmled_icon;
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)


// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "menu_window", ID_WINDOW_0, 0, 0, 640, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "led_button", ID_BUTTON_0, 99, 200, 60, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "sensor_button", ID_BUTTON_1, 273, 200, 60, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "debug_button", ID_BUTTON_2, 449, 200, 60, 60, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 70, 270, 120, 30, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 244, 270, 120, 30, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_2, 420, 270, 120, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'menu_window'
    //
    hItem = pMsg->hWin;
//    WINDOW_SetBkColor(hItem, 0x00FF3939);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_0);
    HEADER_AddItem(hItem, 120, "LEDS CONTROL", 14);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_1);
    HEADER_AddItem(hItem, 120, "SENSORS CONTROL", 14);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_2);
    HEADER_AddItem(hItem, 120, "MODBUS DEBUG", 14);
    // USER START (Optionally insert additional code for further widget initialization)

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText( hItem, "");
    BUTTON_SetBkColor (hItem, 0, GUI_RED);
    BUTTON_SetBkColor (hItem, 1, GUI_YELLOW);

    BUTTON_SetBitmapEx(hItem, 0, &bmdebug_icon, 5, 5);



    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText( hItem, "");
    BUTTON_SetBkColor (hItem, 0, GUI_GREEN);
    BUTTON_SetBkColor (hItem, 1, GUI_YELLOW);

    BUTTON_SetBitmapEx(hItem, 0, &bmled_icon, 5, 5);



    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText( hItem, "");
    BUTTON_SetBkColor (hItem, 0, GUI_BLUE);
    BUTTON_SetBkColor (hItem, 1, GUI_YELLOW);

    BUTTON_SetBitmapEx(hItem, 0, &bmsensor_icon, 5, 5);


    hItem = TEXT_CreateEx(0, 40, 640, 30, 0, WM_CF_SHOW, 0, GUI_ID_TEXT0, "House Control with MODBUS");
    TEXT_SetFont(hItem, &GUI_Font24B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_BLACK);


    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'led_button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        PlayAudio(150, 20);				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				
				BS_wrapper(CHANGE_TO_LED_CONTROLLER);
        
				// USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'sensor_button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        PlayAudio(150, 20);				
        
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
			//BS_wrapper(CHANGE_TO_SENSOR_CONTROLLER, TOUCH);			
			ExecSensor_Control();
			
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'debug_button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)

        PlayAudio(150, 20);				

        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)

        ExecModbus_Master_testWindow();
        
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_HEADER_0: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_HEADER_1: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_HEADER_2: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createmenu_window
*/
WM_HWIN Createmenu_window(void);
WM_HWIN Createmenu_window(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)


WM_HWIN ExecMenu_window(void);
WM_HWIN ExecMenu_window(void) {

  WM_HWIN hWin = GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER END

/*************************** End of file ****************************/
