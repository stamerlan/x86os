#!/bin/bash

# Script creates disk image, creates ext2 partition, installs grub2 if necessaty
# and copies kernel to image
# Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
#
# DISK_SZ - disk size
# NOTE: /dev/loop0 and /dev/loop1 - should be unmounted
# NOTE: You need to be root to use this script
# NOTE: Disk file will belong to root:root
# TODO: Maybe move DISK_SZ to cmdline args?
# TODO: Use dynamic loop device numbers
# TODO: I dislike how fdisk used

DISK_SZ=10080
MOUNTPOINT=/mnt

FILENAME=`readlink -e "$0"`
DIRNAME=`dirname "$FILENAME"`


# Creates parition table
#  - arg1 - DiskImage file
create_parttab ()
{
fdisk $1 << EOF
n
p



a
1
w
EOF
}

# Creates GRUB2 config
create_grubcfg ()
{
cat > $MOUNTPOINT/boot/grub/grub.cfg << EOF
set timeout=3
set default=0

menuentry "x86os" {
	multiboot /boot/kernel
	module /boot/initrd
	boot
}
EOF
}

################################################################################

if [[ $# < 2 ]]; then
	echo Usage: $0 DiskFile KernelFile
	exit 1;
fi

if [ `id -u` != "0" ]; then
	echo -e "\E[31;1mYou must be root to use this script!\E[0m"
	exit 1;
fi

if [ ! -f $1 ]; then
	create_new_img=0	# true, uninitialized var - false
fi

if [ $create_new_img ]; then
	dd if=/dev/zero of=$1 bs=1k count=$DISK_SZ
	create_parttab $1
fi

losetup -o 1048576 /dev/loop0 $1

if [ $create_new_img ]; then
	mkfs.ext2 -L x86os /dev/loop0
fi

mount /dev/loop0 $MOUNTPOINT

if [ $create_new_img ]; then
	mkdir -p $MOUNTPOINT/boot/grub/
	create_grubcfg
	losetup /dev/loop1 $1
	grub-install --root-directory=$MOUNTPOINT \
		--boot-directory=$MOUNTPOINT/boot \
		--no-floppy --modules="normal ext2 part_msdos multiboot" \
		/dev/loop1
	losetup -d /dev/loop1
fi

cp -v $2 $MOUNTPOINT/boot/kernel
cp -v $DIRNAME/initrd $MOUNTPOINT/boot/initrd

umount /dev/loop0
losetup -d /dev/loop0

