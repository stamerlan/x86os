#!/bin/bash

# Script creates disk image, creates ext2 partition, installs grub2
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

if [ $# == 0 ]; then
	echo Usage: $0 DiskFile
	exit 1;
fi

if [ `id -u` != "0" ]; then
	echo -e "\E[31;1mYou must be root to use this script!\E[0m"
	exit 1;
fi

dd if=/dev/zero of=$1 bs=1k count=$DISK_SZ
fdisk $1 << EOF
n
p



a
1
w
EOF

losetup /dev/loop0 $1
losetup -o 1048576 /dev/loop1 $1
mkfs.ext2 -L x86os /dev/loop1

mount /dev/loop1 /mnt

cp -R disk/* /mnt

grub-install --root-directory=/mnt --boot-directory=/mnt/boot --no-floppy --modules="normal ext2 part_msdos multiboot" /dev/loop0

umount /dev/loop1
losetup -d /dev/loop1
losetup -d /dev/loop0
