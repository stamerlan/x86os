# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
#
SUBDIRS	= minixfs
ARCHIVES= minixfs/minixfs.o

OUT	= fs.o

OBJS	= vfs.o nname.o

CC	= gcc
AS	= gas
LD	= ld

CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include -m32
LDFLAGS = -m elf_i386 -r

all:	$(OBJS)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
	$(LD) $(LDFLAGS) -o $(OUT) $(ARCHIVES) $(OBJS)

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f $(OBJS) $(OUT) *.c~

.PHONY:	clean

