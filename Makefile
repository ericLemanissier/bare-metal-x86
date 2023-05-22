
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

AS=i686-elf-as
CC=i686-elf-gcc
CXX=i686-elf-g++

CFLAGS:=$(CFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS:=$(LDFLAGS) -ffreestanding -O2
LIBS:=$(LIBS) -nostdlib -lgcc

export PREFIX:=${PWD}/opt/cross
export TARGET:=i686-elf
export PATH:=$(PREFIX)/bin:$(PATH)

all: boot.asm
	$(AS) boot.asm -o boot.o
	$(CXX) -c kernel.cpp -o kernel.o $(CPPFLAGS)
	$(CXX) -T linker.ld -o myos.bin $(LDFLAGS) $(LIBS) boot.o kernel.o
	qemu-system-i386 -kernel myos.bin

toolchain:
	cd toolchain && $(MAKE)
