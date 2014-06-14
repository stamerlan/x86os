#!/bin/bash

cd bochs-2.6.5
./configure --enable-debugger --enable-disasm --enable-all-optimizations --enable-smp --enable-readline --with-x LDFLAGS=-pthread --prefix=/opt/bochs/
make
sudo make install
cd ..
rm -fr bochs-2.6.5

