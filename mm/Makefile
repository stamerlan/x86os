# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

OBJS=\
     mm.o

OUT=$(LIBDIR)libmm.a

CC=gcc
AS=gas
LD=ld
AR=ar
RANLIB=ranlib

CFLAGS=-fno-pic -static -fno-builtin -fno-strict-aliasing -Wall -ggdb -m32 \
       -Werror -fno-omit-frame-pointer -fno-stack-protector -I $(INCDIR)
ASFLAGS=-m32 -gdwarf-2 -Wa,-divide
LDFLAGS=-m elf_i386

all:	$(OBJS)
	$(AR) rc $(OUT) $(OBJS)
	$(RANLIB) $(OUT)

clean:
	rm -f $(OBJS) $(OUT)
