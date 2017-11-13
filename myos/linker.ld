ENTRY(_start)
SECTIONS
{
. = 1M;
.text : { *(.text) }
.multiboot : { *(.multiboot) }
.bss : { *(.bss) }
}
