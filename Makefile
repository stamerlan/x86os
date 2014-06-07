# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
# TODO: bochs on quit returns 1.

SUBDIRS=\
	kernel
IMG=disk.img

MAKE=make
QEMU=qemu
CPU_NR=1
RAM_SZ=32
QEMUOPTS=-smp $(CPU_NR) -m $(RAM_SZ)
BOCHS=bochs
BOCHSCONF=bochs.conf


all:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

img:	all
	sudo ./mkdisk.sh $(IMG)
	sudo chown `id -u`:`id -g` $(IMG)

qemu:	img
	$(QEMU) $(QEMUOPTS) -hda $(IMG)

bochs:	img
	$(BOCHS) -f $(BOCHSCONF)

backup:	clean
	cd .. && tar -cvzf x86os-backup.tar.gz x86os

clean:
	rm -f $(IMG)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
