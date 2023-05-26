SYSTEM_HEADER_PROJECTS="kernel"
PROJECTS="kernel"

DEFAULT_HOST!=./default-host.sh
HOST?=$(DEFAULT_HOST)

AR=${HOST}-ar
AS=${HOST}-as
CC=${HOST}-gcc
CXX=${HOST}-g++

PREFIX=/usr
EXEC_PREFIX=$(PREFIX)
BOOTDIR=/boot
LIBDIR=$(EXEC_PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include

CFLAGS=-O2 -g
CPPFLAGS=

# Configure the cross-compiler to use the desired system root.
SYSROOT=$(shell pwd)/sysroot
CC+= --sysroot=$(SYSROOT)
CXX+= --sysroot=$(SYSROOT)

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
CC+= -isystem=$(INCLUDEDIR)
CXX+= -isystem=$(INCLUDEDIR)

export DEFAULT_HOST
export HOST
export AR
export AS
export CC
export CXX

export PREFIX
export EXEC_PREFIX
export BOOTDIR
export LIBDIR
export INCLUDEDIR

export CFLAGS
export CPPFLAGS

export SYSROOT

.PHONY: clean headers build qemu toolchain

.DEFAULT_GOAL := myos.iso

clean:
	for PROJECT in $(PROJECTS); do \
	(cd $$PROJECT && $(MAKE) clean) \
	done

	rm -rf sysroot
	rm -rf isodir
	rm -rf myos.iso

headers:
	mkdir -p "$(SYSROOT)"
	for PROJECT in $(SYSTEM_HEADER_PROJECTS); do \
	(cd $$PROJECT && DESTDIR="$(SYSROOT)" $(MAKE) install-headers) \
	done

build: headers
	export PATH="$(shell pwd)/opt/cross/bin:${PATH}" && \
	for PROJECT in $(PROJECTS); do \
	(cd $$PROJECT && DESTDIR="$(SYSROOT)" $(MAKE) install) \
	done

sysroot/boot/myos.kernel: build

myos.iso: sysroot/boot/myos.kernel grub.cfg
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub

	cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

qemu: myos.iso
	qemu-system-$(shell ./target-triplet-to-arch.sh $(HOST)) -cdrom myos.iso -serial stdio
