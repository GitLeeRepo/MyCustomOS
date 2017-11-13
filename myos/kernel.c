static char* const VGA_MEMORY = (char*)0xb8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

void kernel_early(void) {
    // tbd
}

int main(void) {
    const char *str = "Hello World";
    unsigned int i = 0;
    unsigned int j = 0;

    while (str[i] != '\0'){
        VGA_MEMORY[j] = str[i];
        VGA_MEMORY[j+1] = 0x02;
        i++;
        j = j+2;
    }

    // temp infite loop
    while (1==1) {

    }
    return 0;
}
