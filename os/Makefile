HOSTARCH=i386

CFLAGS?=-O2 -g
CPPFLAGS?=
CXXFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot


CFLAGS+= -ffreestanding -Wall -Wextra -Wimplicit -flto -fno-math-errno -funsafe-math-optimizations
CPPFLAGS+=
CXXFLAGS+= -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -std=c++23  -fmodules-ts -flto -fno-math-errno -funsafe-math-optimizations
LDFLAGS+=
LIBS+= -nostdlib -lgcc

ARCHDIR=arch/$(HOSTARCH)
APPDIR=app

include $(ARCHDIR)/make.config
include $(APPDIR)/make.config

CFLAGS+= $(KERNEL_ARCH_CFLAGS)
CPPFLAGS+= $(KERNEL_ARCH_CPPFLAGS)
CXXFLAGS+= $(KERNEL_ARCH_CFLAGS)
LDFLAGS+= $(KERNEL_ARCH_LDFLAGS)
LIBS+= $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
kernel/memmove.o \
kernel/memset.o \
kernel/memcpy.o \
kernel/debug.o \
kernel/geom.o \
kernel/keyboard.o \
kernel/terminate.o \
$(KERNEL_ARCH_OBJS) \
kernel/debug_impl.o \
kernel/keyboard_impl.o \
kernel/pixel.o \
$(APP_OBJS) \
kernel/kernel.o \

STDLIB_MODULES=\
$(KERNEL_STD_MODULES) \
cstddef.gcm \
cstdint.gcm \
source_location.gcm \
cstdlib.gcm \
bitset.gcm \
array.gcm \
algorithm.gcm \
$(APP_STDLIB_MODULES) \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install $(STDLIB_MODULES)
.SUFFIXES: .o .c .S

all: myos.kernel

myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CXX) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CXX) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(STDLIB_MODULES): %.gcm:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -xc++-system-header $*

$(OBJS): $(STDLIB_MODULES)

clean:
	$(RM) myos.kernel
	$(RM) $(OBJS) *.o */*.o */*/*.o
	$(RM) $(OBJS:.o=.d) *.d */*.d */*/*.d
	$(RM) -r gcm.cache

install: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $< $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
