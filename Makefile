export HOST=i686-elf

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CXX=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$(PREFIX)
export BOOTDIR=/boot
export LIBDIR=$(EXEC_PREFIX)/lib

export CFLAGS=-O3 -g
export CPPFLAGS=
export CXXFLAGS=-O3 -g

# Configure the cross-compiler to use the desired system root.
export SYSROOT=$(shell pwd)/sysroot
CC+= --sysroot=$(SYSROOT)
CXX+= --sysroot=$(SYSROOT)

.PHONY: clean build qemu toolchain

.DEFAULT_GOAL := myos.iso

clean:
	cd os && $(MAKE) clean
	$(RM) -r sysroot
	$(RM) -r isodir
	$(RM) myos.iso
	cd toolchain && $(MAKE) clean

build:
	export PATH="$(shell pwd)/opt/cross/bin:${PATH}" && \
	cd os && DESTDIR="$(SYSROOT)" $(MAKE) install

sysroot/boot/myos.kernel: build

myos.iso: sysroot/boot/myos.kernel grub.cfg
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub

	cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

qemu: myos.iso
	qemu-system-i386 -cdrom $< -serial stdio
