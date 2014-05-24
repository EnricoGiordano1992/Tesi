/*
===============================================================================
 Name        : main.c
 Author      : Enrico Giordano
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

/*----------------------------- Includes ------------------------------------*/


#include "stdint.h"
#include "string.h"
#include "Includes.h"
#include "modbus.h"
#include "driver/ISR.h"
#include "utils.h"


// Config word (fuses)
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & BOREN_OFF & CP_OFF);


/*----------------------------- Registers -----------------------------------*/

int8_t coils = 0b00000000;
int8_t inputs = 0b00001001;
int16_t hold_regs[] = {0x8800,0x7700,0x6600,0x5500,0x4400,0x3300,0x2200,0x1100};
int16_t input_regs[] = {0x1100,0x2200,0x3300,0x4400,0x5500,0x6600,0x7700,0x8800};
int16_t event_count = 0;




/* ----------------------- Start implementation -----------------------------*/

void modbus_clear_message()
{
      modbus_rx.address = 0;
      modbus_rx.error = 0;
      modbus_rx.func = 0;
      modbus_rx.len = 0;
}


void modbus_slave_start()
{
      //check address against our address, 0 is broadcast
      if((modbus_rx.address == MODBUS_ADDRESS) || modbus_rx.address == 0)
      {
         switch(modbus_rx.func)
         {
            case FUNC_READ_COILS:    //read coils
            case FUNC_READ_DISCRETE_INPUT:    //read inputs
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int8_t data;

                  if(modbus_rx.func == FUNC_READ_COILS)
                     data = coils>>(modbus_rx.data[1]);      //move to the starting coil
                  else
                     data = inputs>>(modbus_rx.data[1]);      //move to the starting input

                  data = data & (0xFF>>(8-modbus_rx.data[3]));  //0 out values after quantity

                  if(modbus_rx.func == FUNC_READ_COILS)
                     modbus_read_discrete_input_rsp(MODBUS_ADDRESS, 0x01, &data);
                  else
                     modbus_read_discrete_input_rsp(MODBUS_ADDRESS, 0x01, &data);

                  event_count++;
               }
               break;

             case FUNC_WRITE_SINGLE_COIL:      //write coil
               if(modbus_rx.data[0] || modbus_rx.data[3] || modbus_rx.data[1] >= 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else if((modbus_rx.data[2] != 0xFF) && (modbus_rx.data[2] != 0x00))
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_VALUE);
               else
               {
                  //coils are stored msb->lsb so we must use 7-address
                  if(modbus_rx.data[2] == 0xFF)
                     bit_set(coils,modbus_rx.data[1]);
                  else
                     bit_clear(coils,modbus_rx.data[1]);

                  modbus_write_single_coil_rsp(MODBUS_ADDRESS,modbus_rx.data[1],((int16_t)(modbus_rx.data[2]))<<8);

                  event_count++;
               }
               break;
 
             case FUNC_WRITE_MULTIPLE_COILS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int i,j;

                  modbus_rx.data[5] = swap_bits(modbus_rx.data[5]);

                  for(i=modbus_rx.data[1],j=0; i < modbus_rx.data[1]+modbus_rx.data[3]; ++i,++j)
                  {
                     if(bit_test(modbus_rx.data[5],j))
                        bit_set(coils,7-i);
                     else
                        bit_clear(coils,7-i);
                  }

                  modbus_write_multiple_coils_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));

                  event_count++;
               }
               break;

             default:    //We don't support the function, so return exception
               modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_FUNCTION);
         }


      }

      modbus_clear_message();
}




int
main( void )
{

    TRISC1 = 0;

    blink(5);


   modbus_init();


    //PEIE, Interrupt di periferica attivato
    //GIE, Gestione Interrupt attiva

    GIE  = 1;  							// Enable global interrupts
    PEIE = 1;  							// Enable Peripheral Interrupts


   while(1)
   {
      if(modbus_kbhit())
      {
          //attendo di ricevere tutto il messaggio
          delay_us(30);
          modbus_slave_start();
      }

      else
      {
          if(bit_test(coils, 0))
              RC1 = 1;
          else
              RC1 = 0;
      }

  }
}

