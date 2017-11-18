CC=gcc
AS=nasm
TARGET=myos
C_FILES=./kernel.c
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: finale
.PHONY: all compile clean finale

%.o:
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32

$(TARGET): $(OBJS)
	$(AS) -f elf kernel_load.asm -o kernel_load.o
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc kernel_load.o  $? -T linker.ld -o $(TARGET)

finale:
	$(shell cp $(TARGET) ./iso/boot/$(TARGET))
	$(shell grub-mkrescue iso --output=$(TARGET).iso)

clean:
	rm -f *.o $(TARGET) $(TARGET).iso
	find . -name \*.o | xargs --no-run-if-empty rm
