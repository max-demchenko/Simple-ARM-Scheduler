ENTRY(Reset_Handler)


MEMORY { 
    FLASH (RX) :ORIGIN = 0x08000000, LENGTH = 32K
    SRAM  (RW) :ORIGIN = 0x20000000, LENGTH = 10k
}

SECTIONS {
    .text :
    {
        *(.vector_table)
        *(.text)
        . = ALIGN(4);  
    } > FLASH

        
    .rodata : 
    { 
        *(.rodata)
        . = ALIGN(4);
    }> FLASH

    _ld_data = LOADADDR(.data);
    .data : 
    { 
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss : 
    {   
        _sbss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    }> SRAM
}