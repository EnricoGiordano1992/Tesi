/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2014
 * (c) NXP Semiconductors 2013-2014
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v7.0.2 [Build 102] [2014-03-11]  on Fri Mar 14 12:44:08 CET 2014)
*/

MEMORY
{
  /* Define each memory region */
  Flash_64_81 (rx) : ORIGIN = 0x0, LENGTH = 0x10000 /* 64K bytes */
  Ram16 (rwx) : ORIGIN = 0x40000000, LENGTH = 0x4000 /* 16K bytes */


}
  /* Define a symbol for the top of each memory region */
  __top_Flash_64_81 = 0x0 + 0x10000;
  __top_Ram16 = 0x40000000 + 0x4000;



