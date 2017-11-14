// straddr32.c
// Compile & Link as 32-bit: gcc -m32 -o straddr32 straddr32.c

// Test for printing a memory address in MyOS kernel.  Since MyOS doesn't
// have access to the C library (at least not yet), I needed a lower level
// way of printing out a memory address without using printf format strings.
// This example includes printf functions, but only for reference to verify
// the accurracy.  The version in the kernel won't have the printf calls,
// and will have other necessary modifications (such as writing to VGA memory

#include <stdio.h>


void kprintaddr(unsigned long addr) {
    unsigned long div = 0x10000000;
    unsigned long result;

    printf("%08lx\n", addr);    // full addr in hex for visual comparison reference
    for (int i = 0; i < 8; i++) {
        result = addr/div;
        if (result >= 0 && result <= 9) {
            printf("%c", (char) result+48);
        }
        else if (result >= 10 && result <= 15) {
            printf("%c", (char) result+87);
        }
        else {
            printf("x");
        }
        addr = addr - (result*div);
        div = div/0x10;
    }
    printf("\n");
}

int main() {
    char *s1 = "test";

    // I was hoping a string literal would give me the address of the line of code
    // being executed, similar to an immediate in assembler, but it instead is placed
    // on the stack adjacent to the local string var (s1) in memory.  In fact, he second 
    // literal has the same address as the first literal, when they are identical..
    printf("%s and %s\n", s1, "literal");  // strings themselves for reference
    printf("%p and %p\n", s1, "literal");  // string addresses for reference
    kprintaddr((unsigned long)s1);
    kprintaddr((unsigned long)"literal");

}
