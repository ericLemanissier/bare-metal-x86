SYSTEM_HEADER_PROJECTS="kernel"
PROJECTS="kernel"

export HOST=i686-elf

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CXX=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$(PREFIX)
export BOOTDIR=/boot
export LIBDIR=$(EXEC_PREFIX)/lib
export INCLUDEDIR=$(PREFIX)/include

export CFLAGS=-O2 -g
export CPPFLAGS=
export CXXFLAGS=-O2 -g

# Configure the cross-compiler to use the desired system root.
export SYSROOT=$(shell pwd)/sysroot
CC+= --sysroot=$(SYSROOT)
CXX+= --sysroot=$(SYSROOT)

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
CC+= -isystem=$(INCLUDEDIR)
CXX+= -isystem=$(INCLUDEDIR)

.PHONY: clean headers build qemu toolchain

.DEFAULT_GOAL := myos.iso

clean:
	for PROJECT in $(PROJECTS); do \
	(cd $$PROJECT && $(MAKE) clean) \
	done
	$(RM) -r sysroot
	$(RM) -r isodir
	$(RM) myos.iso
	cd toolchain && $(MAKE) clean

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
	qemu-system-i386 -cdrom myos.iso -serial stdio