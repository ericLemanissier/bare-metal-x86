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


CFLAGS+= -ffreestanding -Wall -Wextra -Wimplicit
CPPFLAGS+= -D__is_kernel
CXXFLAGS+= -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20  -fmodules-ts
LDFLAGS+= -ffreestanding -O2
LIBS+= -nostdlib -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS+= $(KERNEL_ARCH_CFLAGS)
CPPFLAGS+= $(KERNEL_ARCH_CPPFLAGS)
CXXFLAGS+= $(KERNEL_ARCH_CFLAGS)
LDFLAGS+= $(KERNEL_ARCH_LDFLAGS)
LIBS+= $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
kernel/debug.o \
$(KERNEL_ARCH_OBJS) \
kernel/debug_impl.o \
kernel/kernel.o \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(ARCHDIR)/cstddef.o \
$(ARCHDIR)/cstdint.o \
$(ARCHDIR)/source_location.o \
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

.PHONY: all clean install
.SUFFIXES: .o .c .S

all: myos.kernel

myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CXX) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CXX) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(ARCHDIR)/cstddef.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -xc++-system-header cstddef

$(ARCHDIR)/cstdint.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -xc++-system-header cstdint

$(ARCHDIR)/source_location.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -xc++-system-header source_location


clean:
	$(RM) myos.kernel
	$(RM) $(OBJS) *.o */*.o */*/*.o
	$(RM) $(OBJS:.o=.d) *.d */*.d */*/*.d
	$(RM) -r gcm.cache

install: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
