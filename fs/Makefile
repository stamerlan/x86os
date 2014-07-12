# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

OUT	= fs.o

SUBDIRS	= testfs
OBJS	= super.o file.o filesystems.o inode.o open.o namei.o
# TODO: Try to use $SUBDIRS instead duplication
ARCHIVES= testfs/testfs.o

CC	= gcc
AS	= gas
LD	= ld

CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include -m32
LDFLAGS = -m elf_i386 -r

all:	$(OBJS)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
	$(LD) $(LDFLAGS) -o $(OUT) $(OBJS) $(ARCHIVES)

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f $(OBJS) $(OUT)

.PHONY:	clean

