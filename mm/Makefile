# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

OUT	= mm.o

OBJS	= memory.o

CC	= gcc
AS	= gas
LD	= ld

CFLAGS	= -Wall -Werror -O -fno-pic -static -fno-builtin \
	  -fno-strict-aliasing -fno-omit-frame-pointer \
	  -fno-stack-protector -finline-functions -I../include

all:	$(OBJS)
	$(LD) -r -o $(OUT) $(OBJS)

clean:
	rm -f $(OBJS) $(OUT)

.PHONY:	clean

