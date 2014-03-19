/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-${year}
 * (c) NXP Semiconductors 2013-${year}
 * Generated linker script file for 2014
 * Created from LPC2142 (Fri Mar 14 12:44:08 CET 2014)
 * By generic_c.ld on LPCXpresso v7.0.2 [Build 102] [2014-03-11] 
 */


INCLUDE "SModbus_lpc2148_Debug_lib.ld"
INCLUDE "SModbus_lpc2148_Debug_mem.ld"

ENTRY(_start)

SECTIONS
{

    /* MAIN TEXT SECTION */    
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */
        

        *(.after_vectors*)
        
        /* Code Read Protect data */
        . = 0x000001FC ;
        PROVIDE(__CRP_WORD_START__ = .) ;
        KEEP(*(.crp))
        PROVIDE(__CRP_WORD_END__ = .) ;
        ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
        /* End of Code Read Protect */
        
    } >Flash_64_81
    
    .text : ALIGN(4)    
    {
         *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
        
    } > Flash_64_81

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4)
    {
    	*(.ARM.extab* .gnu.linkonce.armextab.*)
    } > Flash_64_81
    __exidx_start = .;
    
    .ARM.exidx : ALIGN(4)
    {
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > Flash_64_81
    __exidx_end = .;
    
    _etext = .;
        
    
    /* MAIN DATA SECTION */
    

    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > Ram16

	
	/* Main DATA section (Ram16) */
	.data : ALIGN(4)
	{
	   FILL(0xff)
	   _data = . ;
	   *(vtable)
	   *(.ramfunc*)
	   *(.data*)
	   . = ALIGN(4) ;
	   _edata = . ;
	} > Ram16 AT>Flash_64_81


    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > Ram16
        
    
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > Ram16
    
    PROVIDE(_pvHeapStart = .);
    PROVIDE(_vStackTop = __top_Ram16 - 0);
}
