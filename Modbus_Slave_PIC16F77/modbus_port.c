/*

 MODBUS_PORT.C
 *
 * created by Enrico Giordano
 *
 * usefull c file for modbus porting
 * 

 */

#include "modbus.h"
#include "port.h"


extern int32_t modbus_serial_wait;
extern BOOL modbus_serial_new;
extern char Message[64];
extern int8_t indix;
extern Modbus_serial_state modbus_serial_state = 0;
extern Modbus_serial_crc modbus_serial_crc;



void delay_us(uint32_t delayInMs)
{
    DelayMs(delayInMs);
}


void TimersEnable(  )
{
    timer_enable();
}


void TimersDisable(  )
{
    timer_disable();
}




// Purpose:    Enable data reception
// Inputs:     None
// Outputs:    None
void RCV_ON(void)
{
    RX_ISR_activate();
}

void RCV_OFF(void)
{
    RX_ISR_deactivate();
}


// Purpose:    Start our timeout timer
// Inputs:     Enable, used to turn timer on/off
// Outputs:    None
void modbus_enable_timeout(BOOL enable)
{
   if (enable) {

	   TimersEnable();

   }

   else
	   TimersDisable();

}


// Purpose:    Check if we have timed out waiting for a response
// Inputs:     None
// Outputs:    None
//void modbus_timeout_now(void)
void timer_interrupt(void)
{

   if((modbus_serial_state == MODBUS_GETDATA)  && (!modbus_serial_new))
   {
      modbus_rx.len-=2;
      modbus_serial_new=TRUE;
   }
   else
      modbus_serial_new=FALSE;

   modbus_serial_crc.d=0xFFFF;
   modbus_serial_state=MODBUS_GETADDY;
   modbus_enable_timeout(FALSE);


}



// Purpose:    Puts a character onto the serial line
// Inputs:     Character
// Outputs:    None
void modbus_serial_putc(int8_t c)
{
	Message[indix] = c;
	indix++;

        SendByteSerially(c);

}




void uart_rx_interrupt(void)
{
   char c = ReceiveByteSerially();

   //nuovo carattere ricevuto
   modbus_serial_new = TRUE;

      if(modbus_serial_state == MODBUS_GETADDY)
      {
         modbus_serial_crc.d = 0xFFFF;
         modbus_rx.address = c;
         modbus_serial_state++;
         modbus_rx.len = 0;
         modbus_rx.error=0;
      }
      else if(modbus_serial_state == MODBUS_GETFUNC)
      {
         modbus_rx.func = c;
         modbus_serial_state++;
      }
      else if(modbus_serial_state == MODBUS_GETDATA)
      {
         if (modbus_rx.len>=MODBUS_SERIAL_RX_BUFFER_SIZE) {modbus_rx.len=MODBUS_SERIAL_RX_BUFFER_SIZE-1;}
         modbus_rx.data[modbus_rx.len]=c;
         modbus_rx.len++;
      }

      Message[indix++] = c;

      modbus_enable_timeout(TRUE);

}
