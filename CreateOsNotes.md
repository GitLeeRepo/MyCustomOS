# Overview

Notes on writing your own OS in x86 Assembler and C, using name and gcc.

# References

## Books

* [Create Your Own Operating System](https://www.amazon.com/gp/product/B01KU8N6FC/ref=oh_aui_d_detailpage_o00_?ie=UTF8&psc=1) by Lucus Darnell

## YouTube Videos

* [Write your own Operating System](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M) - In depth series of videos.  Uses C++ rather than C.

# Examples

Based on code from [Create Your Own Operating System](https://www.amazon.com/gp/product/B01KU8N6FC/ref=oh_aui_d_detailpage_o00_?ie=UTF8&psc=1) by Lucus Darnell.  I added my own comments and modified it slightly to add nulls on the end, inorder to fill it with nulls to pad the end of the 1.44M diskette image.  I needed to do this because I renamed the bootloader.img file to bootloader.vfd that is needed for a floppy disk image on Hyper-V in Windows 10 Professioal.  Without doing that I got an error saying it was an invalid virtual floppy disk.

Once solving the floppy disk issue, the image booted without issue as a Hyper-V virtual machine.  This is only the boot loader code, which runs in 16-bit Real Mode, displaying a message using .  It does not load the 32-bit kernel in protected mode.

```asm
; bootloader.asm
; Original code came from "Create Your Own Operating System" by Lucas Darnell
; Modified (additional fill for diskette size) and commented by me (TK)
;
; Creates the MBR code with the boot loader.  It runs in 16-bit real mode for the
; BIOS to initially invoke it.

bits 16

start:
    mov ax,0x07c0           ; the address the BIOS loads this bootloader code
    add ax,0x20             ; the offset for the stack seg register
    mov ss,ax
    mov sp,4096             ; the stack size

    mov ax,0x07c0           ; load the same unmodified address into the data seg register
    mov ds,ax

    mov si,msg              ; load msg addr for the lodsb instruction for printing
    call print
    cli                     ; clear interupts
    hlt

data:
    msg db 'Hello world!  Welcome to my first OS, initially running in 16-bit real mode.',0

print:
    mov ah,0x0e
    
.printchar:
    lodsb                   ; load a char for si addr and increment to next char
    cmp al,0
    je .done
    int 0x10                ; call the BIOS to print a character
    jmp .printchar
.done:
    ret

times   510 -($ - $$) db 0      ; null fill until ...
dw      0xaa55                  ; the last two bytes of the MBR
times   1474560 -($ - $$) db 0  ; null fill the rest out to the size of a 1.44MB diskette    
```

Compile

```bash
nasm -f bin -o bootloader.img bootloader.asm
# no link run.  Object file copied to a diskette boot image that is booted in a Hyper-v virtual machine
```

## Kernel

TBD: Rought notes below, needs update

gcc -c kernel.c -o kernel.o -ffreestanding -fno-exceptions -m32

nasm -f elf32 -o boot.o boot.asm
gcc -m32 -nostdlib -nodefaultlibs boot.o -T linker.ld -o myos

### Linker.ld

ENTRY(_start)
SECTIONS
{
. = 0x10000;
.text : { *(.text) }
.data : { *(.data) }
.bss : { *(.bss) }
}

### Create Boot ISO

grub-mkrescue iso --output=myos.iso

# Issues

## Issues with grub-mkrescue

In trying to run **grub-mkrescue iso --output=myos.iso** I got an error saying I needed to update my version of **xorriso** so I ran **sudo apt install xorriso**.  On another Ubuntu install it failed with a **grub-mkrescue: error: `mformat` invocation failed**, this was solved by installing **mtools** with **sudo apt install mtools**, because mformat is part of mtools.
