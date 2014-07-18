# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

OUT	= mm.o

OBJS	= memory.o

CC	= gcc
AS	= gas
LD	= ld

CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include -m32 \
	  -nostdinc
LDFLAGS = -m elf_i386 -r

all:	$(OBJS)
	$(LD) $(LDFLAGS) -o $(OUT) $(OBJS)

clean:
	rm -f $(OBJS) $(OUT) *.c~

.PHONY:	clean

