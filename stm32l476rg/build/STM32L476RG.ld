EXTERN(VectorTable)
ENTRY(ResetHandler)

MEMORY
{
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    rom (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    ram2 (rwx) : ORIGIN = 0x10000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text*)
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    } >rom

    /*
    .preinit_array :
    {
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
    } >rom

    .init_array :
    {
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
    } >rom

    .fini_array :
    {
        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;
    } >rom
    */

    .ARM.extab :
    {
        *(.ARM.extab*)
    } >rom

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >rom

    . = ALIGN(4);
    _etext = .;

    .data :
    {
        _data = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >ram AT >rom

    _data_loadaddr = LOADADDR(.data);

    .bss :
    {
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >ram

    .fixed_size_heap (NOLOAD) :
    {
        _fixed_size_heap_start = .;
        *(.fixed_size_heap*)
        . = ALIGN(4);
        _fixed_size_heap_end = .;
    } >ram2

    .ram2 :
    {
        _ram2 = .;
        *(.ram2*)
        . = ALIGN(4);
        _eram2 = .;
    } >ram2 AT >rom

    _ram2_loadaddr = LOADADDR(.ram2);

    /DISCARD/ : { *(.eh_frame) }
    . = ALIGN(4);
    end = .;
}

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
