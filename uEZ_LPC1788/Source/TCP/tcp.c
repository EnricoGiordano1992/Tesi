/*-------------------------------------------------------------------------*
 * File:  ConsoleMode.c
 *-------------------------------------------------------------------------*
 * Description:
 *      Put a console on the screen and allow TCP/IP to connect and
 *      control the device.
 *-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2010 Future Designs, Inc.
 *--------------------------------------------------------------------------
 * This file is part of the uEZ(R) distribution.  See the included
 * uEZLicense.txt or visit http://www.teamfdi.com/uez for details.
 *
 *    *===============================================================*
 *    |  Future Designs, Inc. can port uEZ(tm) to your own hardware!  |
 *    |             We can get you up and running fast!               |
 *    |      See http://www.teamfdi.com/uez for more details.         |
 *    *===============================================================*
 *
 *-------------------------------------------------------------------------*/
#include <uEZ.h>
#include <uEZLCD.h>
#include <uEZFile.h>
#include <Source/Library/Graphics/SWIM/lpc_rom8x8.h>
#include <Source/Library/Graphics/SWIM/lpc_helvr10.h>
#include <Source/Library/Graphics/SWIM/lpc_winfreesystem14x16.h>
#include <stdio.h>  				
#include <string.h>
#include <sys/types.h>  				
#include <sys/socket.h>  				
#include <netinet/in.h>  				
#include <netdb.h>
#include "uEZDemoCommon.h"
#include <UEZPlatform.h>
#include <UEZLCD.h>
#include <UEZKeypad.h>
#include <BS.h>

/*-------------------------------------------------------------------------*
 * External Hooks:
 *-------------------------------------------------------------------------*/
extern void BeepError(void);

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/


#define WPSPORT 23  	 /* server port number */  			

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/


typedef enum TCPCommand{
	
	QUIT,
	LEDS_CONTROLLER,
	SENSORS_CONTROLLER,
	DEBUG_MODBUS_CONTROLLER,
	RETURN_TO_MENU,
	
	INIT_LEDS,
	CHANGE_LED_STATUS_CONTROLLER_1,
	CHANGE_LED_STATUS_CONTROLLER_2,
	CHANGE_LED_STATUS_CONTROLLER_3,
	CHANGE_LED_STATUS_CONTROLLER_4,
	CHANGE_LED_STATUS_CONTROLLER_5,
	CHANGE_LED_STATUS_CONTROLLER_6,
	UPDATE_LEDS_STATUS_CONTROLLER,
	
	INIT_SENSORS,
	CHANGE_DELAY_QUERY_FROM_CONTROLLER_TO_WINDOW,
	NOTIFY_MAX_TEMPERATURE_THRESHOLD_FROM_CONTROLLER_TO_WINDOW,
	NOTIFY_ALARM_TEMPERATURE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW,
	NOTIFY_ALARM_PRESENCE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW,
	NOTIFY_NO_LIGHT_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW
		
}TCPCommand;





/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/

static TBool G_coModified = EFalse;
static T_uezSemaphore G_coSem = 0;
static TBool G_coExit;

#if (UEZ_ENABLE_TCPIP_STACK)
static int ClntDescriptor;  /* socket descriptor to client */  			
static int SrvrDescriptor;  /* socket descriptor for server */  			

static TBool G_tcpServerRunning = EFalse;
#endif

/*---------------------------------------------------------------------------*
 * Task:  BasicTCPServer
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#if (UEZ_ENABLE_TCPIP_STACK)
static TUInt32 BasicTCPServer(T_uezTask aMyTask, void *aParams)
{  	
    struct sockaddr_in BindInfo;
    struct sockaddr_in rem;
    char c;
    unsigned int len;		
    char command[80];
    char arg[80];
    char line[80];
    int clen = 0;
		int arglen = 0;
		unsigned short has_arg = 0;
    TBool needPrompt = EFalse;
    const char banner[] = "ready\r\n";
		TCPCommand n_command;
		int arg_val;

    /* create an Internet TCP socket */  			
    SrvrDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    /* bind it to port 2000 (> 1023, to avoid the  			
    "well-known ports"), allowing connections from  			
    any NIC */  			
    BindInfo.sin_family = AF_INET;  			
    BindInfo.sin_port = htons(WPSPORT);
    BindInfo.sin_addr.s_addr = INADDR_ANY;  			
    bind(SrvrDescriptor, (struct sockaddr *)&BindInfo, sizeof(BindInfo));

    /* OK, set queue length for client calls */  			
    listen(SrvrDescriptor,5);  			

    while (1){  			
        /* wait for a call */  			
        len = sizeof(rem);
        ClntDescriptor = accept(SrvrDescriptor,(struct sockaddr*)&rem,&len);			
        needPrompt = ETrue;

        write(ClntDescriptor, banner, sizeof(banner)-1);
        while (1){
            if (read(ClntDescriptor, &c, 1) <= 0)
                break;

						//CASO DI COMANDO CON ARGOMENTI
						if( c == ' ' || has_arg){

							//ho finito di leggere tutto
							if (c == '\r' && has_arg) {
							} else if (c == '\n' && has_arg) {
								n_command = atoi(command);
								arg_val = atoi(arg);

								switch(n_command){
									case CHANGE_DELAY_QUERY_FROM_CONTROLLER_TO_WINDOW:
										BS_wrapper(CHANGE_DELAY_QUERY_FROM_CONTROLLER, EXTERNAL, &arg_val);
										break;
									
									case NOTIFY_MAX_TEMPERATURE_THRESHOLD_FROM_CONTROLLER_TO_WINDOW:
										BS_wrapper(NOTIFY_MAX_TEMPERATURE_THRESHOLD_FROM_CONTROLLER, EXTERNAL, &arg_val);
										break;
									
								}

								memset(&command[0], '\0', sizeof(command));								
								memset(&arg[0], '\0', sizeof(arg));
								has_arg = 0;
								arglen = 0;
								clen = 0;
							}
							
							else{
								//ho appena letto ' '
								if( c == ' '){
									has_arg = 1;
									arglen = 0;
								}
								//altrimenti sto leggendo l'argomento
								else{
									arg[arglen++] = c;
								}
							}
						}
						
						else{
							//CASO DI COMANDO SENZA ARGOMENTI
							if (c == '\r' && !has_arg) {
							} else if (c == '\n' && !has_arg) {
								n_command = atoi(command);
								
								switch(n_command){

									case QUIT:
										closesocket(ClntDescriptor);
										break;

									//MENU' COMMANDS
									case LEDS_CONTROLLER:
										BS_wrapper(SWITCH_CONTEXT_TO_LEDS_CONTROLLER, INTERNAL, NULL);
										break;
									
									case SENSORS_CONTROLLER:
										BS_wrapper(SWITCH_CONTEXT_TO_SENSORS_CONTROLLER, INTERNAL, NULL);
										break;

									case DEBUG_MODBUS_CONTROLLER:
										BS_wrapper(SWITCH_CONTEXT_TO_DEBUG_MODBUS_CONTROLLER, INTERNAL, NULL);
										break;

									case RETURN_TO_MENU:
										BS_wrapper(EXIT_CONTROLLER, INTERNAL, NULL);
										break;

									//LEDS COMMANDS

									case INIT_LEDS:
										BS_wrapper(INIT_LEDS_CONTROLLER, INTERNAL, NULL);
										break;

									case CHANGE_LED_STATUS_CONTROLLER_1:
										BS_wrapper(CHANGE_LED_STATUS_1, INTERNAL, NULL);
										break;

									case CHANGE_LED_STATUS_CONTROLLER_2:
										BS_wrapper(CHANGE_LED_STATUS_2, INTERNAL, NULL);
										break;

									case CHANGE_LED_STATUS_CONTROLLER_3:
										BS_wrapper(CHANGE_LED_STATUS_3, INTERNAL, NULL);
										break;

									case CHANGE_LED_STATUS_CONTROLLER_4:
										BS_wrapper(CHANGE_LED_STATUS_4, INTERNAL, NULL);
										break;

									case CHANGE_LED_STATUS_CONTROLLER_5:
										BS_wrapper(CHANGE_LED_STATUS_5, INTERNAL, NULL);
										break;
									
									case CHANGE_LED_STATUS_CONTROLLER_6:
										BS_wrapper(CHANGE_LED_STATUS_6, INTERNAL, NULL);
										break;

									//INIT SENSORS 
									
									case INIT_SENSORS:
										BS_wrapper(INIT_SENSORS_CONTROLLER, INTERNAL, NULL);
										break;

									case NOTIFY_ALARM_PRESENCE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW:
										BS_wrapper(NOTIFY_ALARM_PRESENCE_SENSOR_CHANGED_FROM_CONTROLLER, EXTERNAL, NULL);
										break;

									case NOTIFY_ALARM_TEMPERATURE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW:
										BS_wrapper(NOTIFY_ALARM_TEMPERATURE_SENSOR_CHANGED_FROM_CONTROLLER, EXTERNAL, NULL);
										break;

									case NOTIFY_NO_LIGHT_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW:
										BS_wrapper(NOTIFY_NO_LIGHT_SENSOR_CHANGED_FROM_CONTROLLER, EXTERNAL, NULL);
										break;
									
									
									//UNKNOW COMMAND
									default:
										write(ClntDescriptor, "400\r\n", 5);
										break;									
									
									};

									needPrompt = ETrue;
									clen = 0;
									memset(&command[0], '\0', sizeof(command));								

								} else if (c == '\b') {
										if (clen > 0) {
												clen--;
												command[clen] = '\0';
										}
								} else {
										if (clen < sizeof(command)-1)
												command[clen++] = c;
										command[clen] = '\0';
								}
							}
						}
        close(ClntDescriptor);
    }
} 	 			
#endif /* UEZ_ENABLE_TCPIP_STACK */


void write_from_BS(char *string){
	char str[120];
	int len;

	strcpy(str, string);
	len = strlen(str);

	write(ClntDescriptor, str, len);
}


void server_setup(){
	if (UEZTaskCreate(BasicTCPServer, "TCPServ", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
		return;	
}
/*-------------------------------------------------------------------------*
 * File:  ConsoleMode.c
 *-------------------------------------------------------------------------*/
