# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
# TODO: What about make dep?

OUT	= kernel.o

OBJS	= startup.o main.o log.o i8259.o vectors.o trap.o trapasm.o string.o \
	  proc.o x86.o spinlock.o swtch.o

CC	= gcc
AS	= gas
LD	= ld
PERL	= perl

# TODO: What about -nostdinc
CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include -m32
ASFLAGS	= -Wa,-divide -m32 -I../include
LDFLAGS = -m elf_i386 -r

all:	$(OBJS)
	$(LD) $(LDFLAGS) -o $(OUT) $(OBJS)

vectors.S:	vectors.pl
	$(PERL) vectors.pl > vectors.S

clean:
	rm -f $(OBJS) $(OUT) vectors.S

.PHONY:	clean

