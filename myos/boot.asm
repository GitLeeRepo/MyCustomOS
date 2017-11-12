bits 32
global _start
extern kernel_early
extern main

section .text
    align 4
    dd 0x1badb002           ; grub magic
    dd 0x00                 ; grub flags
    dd -(0x1badb002+0x00)   ; checksum

_start:
    cli
    mov esp,stack
    call kernel_early
    call main
    cli                     ; clear interupts
    jmp $                   ; infinite loop
    hlt


section .bss
    resb 8192
stack:

