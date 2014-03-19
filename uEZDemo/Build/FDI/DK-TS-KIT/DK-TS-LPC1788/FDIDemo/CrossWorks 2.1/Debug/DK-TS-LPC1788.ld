MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  CM3_System_Control_Space (wx) : ORIGIN = 0xe000e000, LENGTH = 0x00001000
  SDRAM (wx) : ORIGIN = 0xa0000000, LENGTH = 0x00800000
  AHB_Peripherals (wx) : ORIGIN = 0x20080000, LENGTH = 0x00040000
  APB1_Peripherals (wx) : ORIGIN = 0x40080000, LENGTH = 0x00080000
  APB0_Peripherals (wx) : ORIGIN = 0x40000000, LENGTH = 0x00080000
  AHBSRAM1 (wx) : ORIGIN = 0x20004000, LENGTH = 0x00004000
  AHBSRAM0 (wx) : ORIGIN = 0x20000000, LENGTH = 0x00004000
  RAM (wx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
  EEPROM (wx) : ORIGIN = 0x00200000, LENGTH = 0x00001000
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00080000
}


SECTIONS
{
  __CM3_System_Control_Space_segment_start__ = 0xe000e000;
  __CM3_System_Control_Space_segment_end__ = 0xe000f000;
  __SDRAM_segment_start__ = 0xa0000000;
  __SDRAM_segment_end__ = 0xa0800000;
  __AHB_Peripherals_segment_start__ = 0x20080000;
  __AHB_Peripherals_segment_end__ = 0x200c0000;
  __APB1_Peripherals_segment_start__ = 0x40080000;
  __APB1_Peripherals_segment_end__ = 0x40100000;
  __APB0_Peripherals_segment_start__ = 0x40000000;
  __APB0_Peripherals_segment_end__ = 0x40080000;
  __AHBSRAM1_segment_start__ = 0x20004000;
  __AHBSRAM1_segment_end__ = 0x20008000;
  __AHBSRAM0_segment_start__ = 0x20000000;
  __AHBSRAM0_segment_end__ = 0x20004000;
  __RAM_segment_start__ = 0x10000000;
  __RAM_segment_end__ = 0x10010000;
  __EEPROM_segment_start__ = 0x00200000;
  __EEPROM_segment_end__ = 0x00201000;
  __FLASH_segment_start__ = 0x00000000;
  __FLASH_segment_end__ = 0x00080000;

  __STACKSIZE__ = 2048;
  __STACKSIZE_PROCESS__ = 2000;
  __STACKSIZE_IRQ__ = 0;
  __STACKSIZE_FIQ__ = 0;
  __STACKSIZE_SVC__ = 0;
  __STACKSIZE_ABT__ = 0;
  __STACKSIZE_UND__ = 0;
  __HEAPSIZE__ = 2500000;

  __emacmem_load_start__ = __AHBSRAM1_segment_start__;
  .emacmem __AHBSRAM1_segment_start__ (NOLOAD) : AT(__AHBSRAM1_segment_start__)
  {
    __emacmem_start__ = .;
    *(.emacmem .emacmem.*)
  }
  __emacmem_end__ = __emacmem_start__ + SIZEOF(.emacmem);

  __emacmem_load_end__ = __emacmem_end__;

  __AHBSRAM1_segment_used_end__ = __AHBSRAM1_segment_start__ + SIZEOF(.emacmem);

  . = ASSERT(__emacmem_end__ >= __AHBSRAM1_segment_start__ && __emacmem_end__ <= (__AHBSRAM1_segment_start__ + 0x00004000) , "error: .emacmem is too large to fit in AHBSRAM1 memory segment");

  __usbhostmem_load_start__ = __AHBSRAM0_segment_start__;
  .usbhostmem __AHBSRAM0_segment_start__ (NOLOAD) : AT(__AHBSRAM0_segment_start__)
  {
    __usbhostmem_start__ = .;
    *(.usbhostmem .usbhostmem.*)
  }
  __usbhostmem_end__ = __usbhostmem_start__ + SIZEOF(.usbhostmem);

  __usbhostmem_load_end__ = __usbhostmem_end__;

  __AHBSRAM0_segment_used_end__ = __AHBSRAM0_segment_start__ + SIZEOF(.usbhostmem);

  . = ASSERT(__usbhostmem_end__ >= __AHBSRAM0_segment_start__ && __usbhostmem_end__ <= (__AHBSRAM0_segment_start__ + 0x00004000) , "error: .usbhostmem is too large to fit in AHBSRAM0 memory segment");

  __vectors_load_start__ = __FLASH_segment_start__;
  .vectors __FLASH_segment_start__ : AT(__FLASH_segment_start__)
  {
    __vectors_start__ = .;
    *(.vectors .vectors.*)
  }
  __vectors_end__ = __vectors_start__ + SIZEOF(.vectors);

  __vectors_load_end__ = __vectors_end__;

  . = ASSERT(__vectors_end__ >= __FLASH_segment_start__ && __vectors_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .vectors is too large to fit in FLASH memory segment");

  __init_load_start__ = ALIGN(__vectors_end__ , 4);
  .init ALIGN(__vectors_end__ , 4) : AT(ALIGN(__vectors_end__ , 4))
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);

  __init_load_end__ = __init_end__;

  . = ASSERT(__init_end__ >= __FLASH_segment_start__ && __init_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .init is too large to fit in FLASH memory segment");

  __text_load_start__ = ALIGN(__init_end__ , 4);
  .text ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);

  __text_load_end__ = __text_end__;

  . = ASSERT(__text_end__ >= __FLASH_segment_start__ && __text_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .text is too large to fit in FLASH memory segment");

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);

  __dtors_load_end__ = __dtors_end__;

  . = ASSERT(__dtors_end__ >= __FLASH_segment_start__ && __dtors_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .dtors is too large to fit in FLASH memory segment");

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);

  __ctors_load_end__ = __ctors_end__;

  . = ASSERT(__ctors_end__ >= __FLASH_segment_start__ && __ctors_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .ctors is too large to fit in FLASH memory segment");

  __rodata_load_start__ = ALIGN(__ctors_end__ , 4);
  .rodata ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);

  __rodata_load_end__ = __rodata_end__;

  . = ASSERT(__rodata_end__ >= __FLASH_segment_start__ && __rodata_end__ <= (__FLASH_segment_start__ + 0x00080000) , "error: .rodata is too large to fit in FLASH memory segment");

  __fast_load_start__ = ALIGN(__rodata_end__ , 4);
  .fast ALIGN(__RAM_segment_start__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);

  __fast_load_end__ = __fast_load_start__ + SIZEOF(.fast);

  . = ASSERT((__fast_load_start__ + SIZEOF(.fast)) >= __FLASH_segment_start__ && (__fast_load_start__ + SIZEOF(.fast)) <= (__FLASH_segment_start__ + 0x00080000) , "error: .fast is too large to fit in FLASH memory segment");

  .fast_run ALIGN(__RAM_segment_start__ , 4) (NOLOAD) :
  {
    __fast_run_start__ = .;
    . = MAX(__fast_run_start__ + SIZEOF(.fast), .);
  }
  __fast_run_end__ = __fast_run_start__ + SIZEOF(.fast_run);

  __fast_run_load_end__ = __fast_run_end__;

  . = ASSERT(__fast_run_end__ >= __RAM_segment_start__ && __fast_run_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .fast_run is too large to fit in RAM memory segment");

  __data_load_start__ = ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4);
  .data ALIGN(__fast_run_end__ , 4) : AT(ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);

  __data_load_end__ = __data_load_start__ + SIZEOF(.data);

  __FLASH_segment_used_end__ = ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4) + SIZEOF(.data);

  . = ASSERT((__data_load_start__ + SIZEOF(.data)) >= __FLASH_segment_start__ && (__data_load_start__ + SIZEOF(.data)) <= (__FLASH_segment_start__ + 0x00080000) , "error: .data is too large to fit in FLASH memory segment");

  .data_run ALIGN(__fast_run_end__ , 4) (NOLOAD) :
  {
    __data_run_start__ = .;
    . = MAX(__data_run_start__ + SIZEOF(.data), .);
  }
  __data_run_end__ = __data_run_start__ + SIZEOF(.data_run);

  __data_run_load_end__ = __data_run_end__;

  . = ASSERT(__data_run_end__ >= __RAM_segment_start__ && __data_run_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .data_run is too large to fit in RAM memory segment");

  __bss.libdebugio_load_start__ = ALIGN(__data_run_end__ , 4);
  .bss.libdebugio ALIGN(__data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__data_run_end__ , 4))
  {
    __bss.libdebugio_start__ = .;
    *(.bss.libdebugio.*)
  }
  __bss.libdebugio_end__ = __bss.libdebugio_start__ + SIZEOF(.bss.libdebugio);

  __bss.libdebugio_load_end__ = __bss.libdebugio_end__;

  . = ASSERT(__bss.libdebugio_end__ >= __RAM_segment_start__ && __bss.libdebugio_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .bss.libdebugio is too large to fit in RAM memory segment");

  __non_init_load_start__ = ALIGN(__bss.libdebugio_end__ , 4);
  .non_init ALIGN(__bss.libdebugio_end__ , 4) (NOLOAD) : AT(ALIGN(__bss.libdebugio_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);

  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_end__ >= __RAM_segment_start__ && __non_init_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .non_init is too large to fit in RAM memory segment");

  __stack_load_start__ = ALIGN(__non_init_end__ , 4);
  .stack ALIGN(__non_init_end__ , 4) (NOLOAD) : AT(ALIGN(__non_init_end__ , 4))
  {
    __stack_start__ = .;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 4);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);

  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_end__ >= __RAM_segment_start__ && __stack_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .stack is too large to fit in RAM memory segment");

  __stack_process_load_start__ = ALIGN(__stack_end__ , 4);
  .stack_process ALIGN(__stack_end__ , 4) (NOLOAD) : AT(ALIGN(__stack_end__ , 4))
  {
    __stack_process_start__ = .;
    *(.stack_process .stack_process.*)
    . = ALIGN(MAX(__stack_process_start__ + __STACKSIZE_PROCESS__ , .), 4);
  }
  __stack_process_end__ = __stack_process_start__ + SIZEOF(.stack_process);

  __stack_process_load_end__ = __stack_process_end__;

  . = ASSERT(__stack_process_end__ >= __RAM_segment_start__ && __stack_process_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .stack_process is too large to fit in RAM memory segment");

  __IRAM_load_start__ = ALIGN(__stack_process_end__ , 4);
  .IRAM ALIGN(__stack_process_end__ , 4) (NOLOAD) : AT(ALIGN(__stack_process_end__ , 4))
  {
    __IRAM_start__ = .;
    *(.IRAM)
  }
  __IRAM_end__ = __IRAM_start__ + SIZEOF(.IRAM);

  __IRAM_load_end__ = __IRAM_end__;

  __RAM_segment_used_end__ = ALIGN(__stack_process_end__ , 4) + SIZEOF(.IRAM);

  . = ASSERT(__IRAM_end__ >= __RAM_segment_start__ && __IRAM_end__ <= (__RAM_segment_start__ + 0x00010000) , "error: .IRAM is too large to fit in RAM memory segment");

  __frames_load_start__ = ALIGN(__SDRAM_segment_start__ , 4);
  .frames ALIGN(__SDRAM_segment_start__ , 4) (NOLOAD) : AT(ALIGN(__SDRAM_segment_start__ , 4))
  {
    __frames_start__ = .;
    *(.frames .frames.*)
    . = ALIGN(MAX(__frames_start__ + 640*480*2*7 , .), 4);
  }
  __frames_end__ = __frames_start__ + SIZEOF(.frames);

  __frames_load_end__ = __frames_end__;

  . = ASSERT(__frames_end__ >= __SDRAM_segment_start__ && __frames_end__ <= (__SDRAM_segment_start__ + 0x00800000) , "error: .frames is too large to fit in SDRAM memory segment");

  __heap_load_start__ = ALIGN(__frames_end__ , 4);
  .heap ALIGN(__frames_end__ , 4) (NOLOAD) : AT(ALIGN(__frames_end__ , 4))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 4);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);

  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_end__ >= __SDRAM_segment_start__ && __heap_end__ <= (__SDRAM_segment_start__ + 0x00800000) , "error: .heap is too large to fit in SDRAM memory segment");

  __loadspace_load_start__ = ALIGN(__heap_end__ , 4);
  .loadspace ALIGN(__heap_end__ , 4) (NOLOAD) : AT(ALIGN(__heap_end__ , 4))
  {
    __loadspace_start__ = .;
    *(.loadspace .loadspace.*)
    . = ALIGN(MAX(__loadspace_start__ + 1*1024*1024 , .), 4);
  }
  __loadspace_end__ = __loadspace_start__ + SIZEOF(.loadspace);

  __loadspace_load_end__ = __loadspace_end__;

  . = ASSERT(__loadspace_end__ >= __SDRAM_segment_start__ && __loadspace_end__ <= (__SDRAM_segment_start__ + 0x00800000) , "error: .loadspace is too large to fit in SDRAM memory segment");

  __lwip_load_start__ = ALIGN(__loadspace_end__ , 4);
  .lwip ALIGN(__loadspace_end__ , 4) (NOLOAD) : AT(ALIGN(__loadspace_end__ , 4))
  {
    __lwip_start__ = .;
    *(.lwip .lwip.*)
  }
  __lwip_end__ = __lwip_start__ + SIZEOF(.lwip);

  __lwip_load_end__ = __lwip_end__;

  . = ASSERT(__lwip_end__ >= __SDRAM_segment_start__ && __lwip_end__ <= (__SDRAM_segment_start__ + 0x00800000) , "error: .lwip is too large to fit in SDRAM memory segment");

  __bss_load_start__ = ALIGN(__lwip_end__ , 4);
  .bss ALIGN(__lwip_end__ , 4) (NOLOAD) : AT(ALIGN(__lwip_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);

  __bss_load_end__ = __bss_end__;

  __SDRAM_segment_used_end__ = ALIGN(__lwip_end__ , 4) + SIZEOF(.bss);

  . = ASSERT(__bss_end__ >= __SDRAM_segment_start__ && __bss_end__ <= (__SDRAM_segment_start__ + 0x00800000) , "error: .bss is too large to fit in SDRAM memory segment");

}

