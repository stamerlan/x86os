# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

SUBDIRS	= ram
ARCHIVES = ram/ram.o

OUT	= block.o

OBJS	= bio.o blk-dev.o

CC	= gcc
AS	= gas
LD	= ld

CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include -m32 \
	  -nostdinc
LDFLAGS = -m elf_i386 -r

all:	$(OBJS)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
	$(LD) $(LDFLAGS) -o $(OUT) $(ARCHIVES) $(OBJS)

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f $(OBJS) $(OUT) *.c~

.PHONY:	clean

