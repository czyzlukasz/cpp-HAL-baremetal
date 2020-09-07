MEMORY {
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

SECTIONS {
    . = ORIGIN(FLASH);
    .text : {
        LONG(ORIGIN(SRAM) + LENGTH(SRAM));
        KEEP(*(.vectors))
        KEEP(*(.vectors*))
        KEEP(*(.text))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
        KEEP(*(.rodata))
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .preinit_array ALIGN(4): {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
    } >FLASH

    .init_array ALIGN(4): {
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
    } >FLASH

    .fini_array ALIGN(4): {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;
    } >FLASH

    __stacktop = ORIGIN(SRAM) + LENGTH(SRAM);
    __data_load = LOADADDR(.data);
    . = ORIGIN(SRAM);

    .data ALIGN(4) : {
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } >SRAM AT >FLASH

    .bss ALIGN(4) (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        __bss_end = .;
        *(.noinit)
        *(.noinit*)
    } >SRAM

    . = ALIGN(4);
    __heap_start = .;
}

