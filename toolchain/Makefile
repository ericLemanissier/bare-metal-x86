GCC_VERSION=15.2.0
BINUTILS_VERSION=2.45

export PREFIX:=${PWD}/../opt/cross
export TARGET:=i686-elf
export PATH:=$(PREFIX)/bin:$(PATH)

.PHONY: all binutils gcc clean

all:
	$(MAKE) binutils
	$(MAKE) gcc

binutils:
	rm binutils-$(BINUTILS_VERSION).tar.xz || true
	wget https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.xz
	tar -xf binutils-$(BINUTILS_VERSION).tar.xz
	mkdir -p build-binutils
	cd build-binutils && \
	../binutils-$(BINUTILS_VERSION)/configure --target=${TARGET} --prefix="${PREFIX}" --with-sysroot --disable-nls --disable-werror && \
	$(MAKE) && \
	$(MAKE) install



gcc:
	rm gcc-$(GCC_VERSION).tar.xz || true
	wget https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz
	tar -xf gcc-$(GCC_VERSION).tar.xz
	which -- ${TARGET}-as || echo ${TARGET}-as is not in the PATH

	mkdir -p build-gcc
	cd build-gcc && \
	../gcc-$(GCC_VERSION)/configure --target=${TARGET} --prefix="${PREFIX}" --disable-nls --enable-languages=c,c++ --without-headers --enable-libstdcxx --disable-hosted-libstdcxx --disable-shared --disable-threads --disable-bootstrap && \
	$(MAKE) all-gcc && \
	$(MAKE) all-target-libgcc && \
	$(MAKE) install-gcc && \
	$(MAKE) install-target-libgcc && \
	$(MAKE) all-target-libstdc++-v3 && \
	$(MAKE) install-target-libstdc++-v3

clean:
	$(RM) binutils-$(BINUTILS_VERSION).tar.xz
	$(RM) -r binutils-$(BINUTILS_VERSION)
	$(RM) -r build-binutils

	$(RM) gcc-$(GCC_VERSION).tar.xz
	$(RM) -r gcc-$(GCC_VERSION)
	$(RM) -r build-gcc
