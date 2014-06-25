# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
# TODO: What about make dep?

OUT	= kernel.o

OBJS	= startup.o main.o log.o i8259.o vectors.o trap.o trapasm.o string.o \
	  proc.o swtch.o x86.o sched.o spinlock.o

CC	= gcc
AS	= gas
LD	= ld
PERL	= perl

# TODO: What about -nostdinc
CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include
ASFLAGS	= -Wa,-divide -I../include

all:	$(OBJS)
	$(LD) -r -o $(OUT) $(OBJS)

vectors.S:	vectors.pl
	$(PERL) vectors.pl > vectors.S

clean:
	rm -f $(OBJS) $(OUT) vectors.S

.PHONY:	clean

