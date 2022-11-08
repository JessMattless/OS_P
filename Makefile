# Automatically generate all needed sources using wildcards
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

#Convert all .c file names to .o to give a list of needed object files
OBJ = $(C_SOURCES:.c=.o)

.PHONY: clean clean-all

# Default build target
all : os-image.iso clean runqemu.sh runqemu.bat

# The disk image that the computer uses to load
# A combination of the kernel and boot sector
os-image.iso : boot/boot_sect.bin kernel.bin
	cat $^ > os-image.iso

# Builds the binary of the kernel from two object files
# kernel_entry, which jumps to main() in the kernel file
# the compiled C kernel
kernel.bin : ${OBJ}
	ld -T 'linker.ld' --gc-sections

# Rule for compiling C code to object files
%.o : %.c
	gcc -Idrivers/headers -c $< -o $@

# screen.o:
# 	gcc -Idrivers/headers -c drivers/screen.c -o drivers/screen.o

# kernel.o:
# 	gcc -Idrivers/headers -c kernel/kernel.c -o kernel/kernel.o

# Assemble kernel_entry
%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -I '../../16bit/' -o $@

clean:
	rm -fr *.bin *.dis *.o
	rm -fr kernel/*.o boot/*.bin drivers/*.o

clean-all: clean
	rm os-image.iso

runqemu.bat:
	echo 'qemu-system-x86_64.exe -monitor stdio -drive file=os-image.iso,format=raw,index=0,media=disk' > runqemu.bat

runqemu.sh:
	echo 'qemu-system-x86_64 -monitor stdio -drive file=os-image.iso,format=raw,index=0,media=disk' > runqemu.sh
	chmod +x runqemu.sh