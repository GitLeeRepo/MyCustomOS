# Overview

Notes on writing your own OS in x86 Assembler and C, using name and gcc.

# References

## Websites

* [Creating an Operating System](http://wiki.osdev.org/Creating_an_Operating_System)

## Books

* [Create Your Own Operating System](https://www.amazon.com/gp/product/B01KU8N6FC/ref=oh_aui_d_detailpage_o00_?ie=UTF8&psc=1) by Lucus Darnell

## My Other Notes

* [gdb debugger](https://github.com/GitLeeRepo/x86Andx64AsmNotes/blob/master/gdbDebugerNotes.md#overview)
* [Windows x86 ASM Notes](https://github.com/GitLeeRepo/x86Andx64AsmNotes/blob/master/Windows_x86AsmNotes.md#overview) - contains the bulk of my x86 notes.  The Linux Notes below are focused on Linux specific features.
* [Linux x64 ASM Notes](https://github.com/GitLeeRepo/x86Andx64AsmNotes/blob/master/Windows_x86AsmNotes.md#overview)

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

TBD: Rought notes below, needs updating

```
gcc -c kernel.c -o kernel.o -ffreestanding -fno-exceptions -m32

nasm -f elf32 -o boot.o boot.asm

gcc -m32 -nostdlib -nodefaultlibs boot.o -lgcc kernel.o -T linker.ld -o myos
```
Note: it initially would not link with the **-lgcc** (libgcc) option, which was solved by installing **gcc-multilib**. Normally it automatically links with **libgcc** without explicitly adding **-lgcc** but because the libraries are being excluded, it is also excluded unless explicitly added as is done here.

### Linker.ld

The linker.ld file used in the above gcc link command.  The **0x100000** entry is the starting address for the kernel that the boot loader looks for. The remaining entries tell the to merge the various .text, .data, .bss sections.

ENTRY(_start)
SECTIONS
{
. = 0x100000;
.text : { *(.text) }
.data : { *(.data) }
.bss : { *(.bss) }
}

### Create Boot ISO

Note: before the following is run the subfolders **iso/boot/grub** must be created and the grub folder needs a **grub.cfg** file and the **boot** folder needs the kernel image itself (this config file is listed after the command below).

```
grub-mkrescue iso --output=myos.iso
```

**grub.cfg:**

```
set timeout=10
set default=0
menuentry "MyOS" {
    multiboot /boot/myos
}
```

**Verifying valid multiboot image**

The following will verify that the multiboot headers are valid in the compiled kernel imsage (**myos** in my case)

```
grub-file --is-x86-multiboot myos ; echo $?
```
Return code of zero, indicating no issues.

### Make file

The **myos** subfolder of this repository contains a **Makefile** that automates the above compile, assemble, link, and grub-mkrescue commands.

### Current Issue with the Kernel

**Disregard, it is working now***.  Temporarily leaving this notes until I update the info further: it will currently not link with the -lgcc option.  By removing this option it links fine, but ultimately the kernel is not loaded properly by GRUB (no error it just returns immediately after attempting to load with **multiboot /boot/myos**, with GRUB still being active aferwards.  It appears (from [this article](http://wiki.osdev.org/Bare_Bones) that the reason the **-lgcc** doesn't work is a result of the **cross platform compiling** options of gcc not being installed.  Normally it automatically links with **libgcc** without explicitly adding **-lgcc** but because the libraries are being excluded, it is also excluded.  It is unknown at this time if this is causing the issue, and it may not since the **grub-file --is-x86-multiboot myos** command that verifies if this is a valid multiboot kernel indicates its good.  Also, the disassembly from running **objdump -d -M intel myos** shows the program correctly starting at address **0x10000000**, and shows all the expected instructions from both the boot.o and kernel.o object files.

# Issues

## Issues with grub-mkrescue

In trying to run **grub-mkrescue iso --output=myos.iso** I got an error saying I needed to update my version of **xorriso** so I ran **sudo apt install xorriso**.  On another Ubuntu install it failed with a **grub-mkrescue: error: `mformat` invocation failed**, this was solved by installing **mtools** with **sudo apt install mtools**, because mformat is part of mtools.
