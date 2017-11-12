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
times   1474560 -($ - $$) db 0  ; null fill the rest out to the size of a 1.4MB diskette

