# x86os
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
# TODO: bochs on quit returns 1.
# TODO: remove hardcoded kernel filename in symb

SUBDIRS=\
	kernel
IMG=disk.img

MAKE=make
QEMU=qemu-system-i386
CPU_NR=1
RAM_SZ=32
QEMUOPTS=-smp $(CPU_NR) -m $(RAM_SZ)
BOCHS=/opt/bochs/bin/bochs
BOCHSCONF=bochs.conf

all:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

img:	all
	sudo ./mkdisk.sh $(IMG)
	sudo chown `id -u`:`id -g` $(IMG)

qemu:	all img
	$(QEMU) $(QEMUOPTS) -hda $(IMG)

symb:	all
	nm disk/boot/kernel | cut -f 1,3 -d ' ' > System.map

bochs:	all img symb
	$(BOCHS) -f $(BOCHSCONF) -q

backup:	clean
	cd .. && tar -cvzf x86os-backup.tar.gz x86os

clean:
	rm -f $(IMG) System.map
	rm -rf docs/
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
