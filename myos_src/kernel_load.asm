; boot.asm
; Based on code from
; Original code from "Create Your Own Operating System" by Lucas Darnell
; Modified:  commented by me (TK)
;
; This program is the initial entry point when the boot loader loads the 
; kernel.  It defines the initial stack and stack pointer.  It also 
; provides a header that is checked by the GRUB multiboot boot loader

bits 32
global _start
extern kernel_init
extern kernel_main

section .multiboot
    align 4
    dd 0x1badb002           ; grub magic
    dd 0x00                 ; grub flags
    dd -(0x1badb002+0x00)   ; checksum

section .text

_start:
    cli
    mov esp,stack           ; key part setting intially stack
    call kernel_init
    call kernel_main
    cli
;    jmp $                   ; infinite loop
h:  hlt
    jmp h


section .bss
resb 8192
stack:

