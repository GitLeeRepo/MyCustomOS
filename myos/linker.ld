ENTRY(_start)
SECTIONS
{
. = 0x100000;
.text : { *(.text) }
.multiboot : { *(.multiboot) }
.bss : { *(.bss) }
}
