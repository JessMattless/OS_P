# Automatically generate all needed sources using wildcards
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

#Convert all .c file names to .o to give a list of needed object files
OBJ = $(C_SOURCES:.c=.o)

# Default build target
all : os-image clean

# The disk image that the computer uses to load
# A combination of the kernel and boot sector
os-image : boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# Builds the binary of the kernel from two object files
# kernel_entry, which jumps to main() in the kernel file
# the compiled C kernel
kernel.bin : ${OBJ}
	ld -T 'linker.ld'

# Rule for compiling C code to object files
# To make it easier all C files depend on all header files
%.o : %.c ${HEADERS}
# gcc -ffreestanding -c $< -o $@
	gcc -c $< -o $@

# Assemble kernel_entry
%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -I '../../16bit/' -o $@

clean:
	rm -fr *.bin *.dis *.o
	rm -fr kernel/*.o boot/*.bin drivers/*.o

clean-all: clean
	rm os-image