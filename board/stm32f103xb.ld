/*
 * Linker script file for stm32f103c8
 */

ENTRY(Reset_Handler)

MEMORY
{
    /*
     * Memory layout per fig. 11, page 34 of the
     * stm32f103x8/stm32f103xb datasheet, rev. 17, dated 21-aug-2015
     */
    flash : org = 0x08000000, len = 64k
    ram : org = 0x20000000, len = 20k
}

SECTIONS
{
    . = ORIGIN(flash);
    .text : {
        . = ALIGN(4);
        KEEP (*(.vectors))
        *(.text.Reset_Handler);
        *(.text);
	*(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >flash

    _data_flash = .;
    . = ORIGIN(ram);
     .data : AT (_data_flash) {
        _data_begin = .;
        *(.data);
        *(.data*);
        . = ALIGN(4);
        _data_end = .;
    } >ram

    .bss : {
        _bss_begin = .;
        *(.bss);
        *(.bss*);
	*(COMMON);
	. = ALIGN(4);
        _bss_end = .;
    } >ram

    _stack_size = 16;
    _stack_end = ALIGN(ORIGIN(ram) + LENGTH(ram) - 8, 8);
    _stack_begin = _stack_end - _stack_size;
    . = _stack_begin;
    ._stack : {
        . = . + _stack_size;
    } >ram

}
