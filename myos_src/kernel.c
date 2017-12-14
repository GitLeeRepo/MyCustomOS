static char* const VGA_MEMORY = (char*)0xb8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;


void kernel_init(void) {
    // tbd
}

unsigned int kprint(const char *str, int i, int j) {
    while (str[i] != '\0'){
        VGA_MEMORY[j] = str[i];
        VGA_MEMORY[j+1] = 0x02;
        i++;
        j = j+2;
    }
    return j;  // return current VGA buffer index, for when caller wants updated position
}

unsigned int kprintAddr(unsigned long addr, int i, int j) {
    unsigned long div = 0x10000000;
    unsigned long result;
    char ch;

    for (int i = 0; i < 8; i++) {
        result = addr/div;
        if (result >= 0 && result <= 9) {
           ch = (char) result+48;
        }
        else if (result >= 10 && result <= 15) {
            ch = (char) result+87;
        }
        else {  // shouldn't get here
            ch = '_';
        }
        VGA_MEMORY[j] = ch;
        VGA_MEMORY[j+1] = 0x02;
        
        j = j+2;
        addr = addr - (result*div);
        div = div/0x10;
    }
    return j;  // return current VGA buffer index, for when caller wants updated position
}

int kernel_main(void) {
    const char *str = "Hello World";
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int row = 0;

    kprint(str, i, j);
    row++;
    kprint("Welcome to my new OS", i, (j+1)*row*VGA_WIDTH*2);
    row++;

    // Line start
    j = kprint("0x", i, (j+1)*row*VGA_WIDTH*2);
    j = kprintAddr((unsigned long) str, i, j);
    kprint(" hex is the current stack address", i, j);
    j = 0; // line finished, reset

    row++;
    kprint("0x00100000 hex (approx 1MB decimal) was the initial kernel load address", 
                                                                i, (j+1)*row*VGA_WIDTH*2);

/* 
    // temp infite loop
    while (1) {
        //__asm__ __volatile__("nop");
        __builtin_return_address(0);
    }
*/
    return 0;
}


