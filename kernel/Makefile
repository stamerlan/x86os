# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

OBJS=\
	startup.o \
	kernel.o \
	log.o \
	i8259.o \
	vectors.o \
	trap.o \
	trapasm.o \
	string.o \
	proc.o \
	swtch.o \
	x86.o \
	sched.o

OUT=../disk/boot/kernel

CC=gcc
AS=gas
LD=ld

CFLAGS=-fno-pic -static -fno-builtin -fno-strict-aliasing -Wall -ggdb -m32 \
       -Werror -fno-omit-frame-pointer -fno-stack-protector -I $(INCDIR)
ASFLAGS=-m32 -gdwarf-2 -Wa,-divide -I $(INCDIR)
LDFLAGS=-m elf_i386 -L$(LIBDIR) -l$(LIBS)

all:	$(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -Ttext 0x100000 -o $(OUT) 
vectors.S:	vectors.pl
	perl vectors.pl > vectors.S

clean:
	rm -f $(OBJS) $(OUT)
	rm -f vectors.S
