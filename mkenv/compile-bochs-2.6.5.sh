#!/bin/bash

cd bochs-2.6.5
./configure --enable-debugger --enable-disasm --with-x --prefix=/opt/bochs/ --enable-smp --enable-readline LDFLAGS=-pthread
make
sudo make install
cd ..
rm -fr bochs-2.6.5

