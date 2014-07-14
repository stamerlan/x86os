#!/bin/bash

FILENAME=`readlink -e "$0"`
DIRNAME=`dirname "$FILENAME"`

echo $HOME

cp -v $DIRNAME/vimrc $HOME/.vimrc
mkdir -p .vim/

mkdir $HOME/.vim/
cp $DIRNAME/nerdtree.zip $HOME/.vim/

cd $HOME/.vim/
unzip nerdtree.zip
rm -f nerdtree.zip

