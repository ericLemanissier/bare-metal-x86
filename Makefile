
export PREFIX:=${PWD}/opt/cross
export TARGET:=i686-elf
export PATH:=$(PREFIX)/bin:$(PATH)

all: boot.asm
	i686-elf-as boot.asm -o boot.o
	i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	qemu-system-i386 -kernel myos.bin

toolchain:
	cd toolchain && $(MAKE)
