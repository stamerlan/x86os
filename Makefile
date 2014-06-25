# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>

SUBDIRS		= kernel \
		  mm \
		  block
# TODO: Try to use $SUBDIRS instead duplication
ARCHIVES	= kernel/kernel.o \
		  mm/mm.o \
		  block/block.o

KERNELFILE	= tools/kernel
IMG		= x86os.img

LD		= ld
NM		= nm
STRIP		= strip
QEMU		= qemu-system-i386
BOCHS		= /opt/bochs/bin/bochs

QEMUOPTS	= -smp 1 -m 32
BOCHSCONF	= bochs.conf

# TODO: What about ldscript?
# TODO: $(STRIP) -s $(KERNELFILE)
LDFLAGS		= -m elf_i386 -Ttext 0x100000

all:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
	$(LD) $(LDFLAGS) $(ARCHIVES) -o $(KERNELFILE)
	$(NM) $(KERNELFILE) | cut -f 1,3 -d ' ' > System.map

tools/kernel:	all

img:	tools/kernel
	sudo tools/mkdisk.sh $(IMG) tools/kernel
	sudo chown `id -u`:`id -g` $(IMG)

qemu:	img
	$(QEMU) $(QEMUOPTS) -hda $(IMG)

bochs:	img
	$(BOCHS) -f $(BOCHSCONF) -q

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f $(IMG) System.map $(KERNELFILE)

.PHONY:	clean qemu bochs img
