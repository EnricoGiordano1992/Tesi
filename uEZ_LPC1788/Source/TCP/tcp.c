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
    char line[80];
    int clen=0;
    TBool needPrompt = EFalse;
    const char banner[] = "House Control with TCP\r\n";

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

    while (1)  	 {  			
        /* wait for a call */  			
        len = sizeof(rem);
        ClntDescriptor = accept(SrvrDescriptor,(struct sockaddr*)&rem,&len);			
        needPrompt = ETrue;

        ////CO_AddString(banner);
        write(ClntDescriptor, banner, sizeof(banner)-1);
        while (1)  {
            if (needPrompt) {
                write(ClntDescriptor, "> ", 2);
                needPrompt = EFalse;
            }
            if (read(ClntDescriptor, &c, 1) <= 0)
                break;

            if (c == '\r') {
            } else if (c == '\n') {
                if (strcmp(command, "quit") == 0) {
                    closesocket(ClntDescriptor);
                } else if (strcmp(command, "dir") == 0) {
                    T_uezFileEntry entry;

                    if (UEZFileFindStart(CONSOLE_ROOT_DIRECTORY, &entry) == UEZ_ERROR_NONE) {
                        do {
                            sprintf(line, "%c %-12.12s %02d/%02d/%04d\r\n",
                                (entry.iFileAttributes & UEZ_FILE_ENTRY_ATTR_DIRECTORY)?'D':'F',
                                entry.iFilename,
                                entry.iModifiedDate.iDay,
                                entry.iModifiedDate.iMonth,
                                entry.iModifiedDate.iYear
                                );
                            write(ClntDescriptor, line, strlen(line));
                        } while (UEZFileFindNext(&entry) == UEZ_ERROR_NONE);
                    } else {
                        printf("No files found.\n");
                    }
                    UEZFileFindStop(&entry);
                } else {
                    write(ClntDescriptor, "\r\nUnknown cmd!\r\n", 16);
                }
                needPrompt = ETrue;
                clen = 0;
                command[0] = '\0';
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
        close(ClntDescriptor);
    }
} 	 			
#endif /* UEZ_ENABLE_TCPIP_STACK */



void server_setup(){
	if (UEZTaskCreate(BasicTCPServer, "TCPServ", UEZ_TASK_STACK_BYTES(2048), (void *)0, UEZ_PRIORITY_NORMAL, 0))
		return;	
}
/*-------------------------------------------------------------------------*
 * File:  ConsoleMode.c
 *-------------------------------------------------------------------------*/
