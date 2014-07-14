#!/bin/bash

wget "http://downloads.sourceforge.net/project/bochs/bochs/2.6.5/bochs-2.6.5.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fbochs%2Ffiles%2Fbochs%2F2.6.5%2F&ts=1402193869&use_mirror=kent" -O bochs-2.6.5.tar.gz
tar -xvf bochs-2.6.5.tar.gz
cd bochs-2.6.5
./configure --enable-debugger --enable-disasm --enable-all-optimizations --enable-smp --enable-readline --with-x LDFLAGS=-pthread --prefix=/opt/bochs/
make
sudo make install
cd ..
rm -f bochs-2.6.5.tar.gz
rm -fr bochs-2.6.5

