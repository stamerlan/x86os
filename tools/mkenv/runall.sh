#/bin/bash

FILENAME=`readlink -e "$0"`
DIRNAME=`dirname "$FILENAME"`

$DIRNAME/get-programs.sh
$DIRNAME/get-libs.sh
$DIRNAME/config-vim.sh
$DIRNAME/install-smartgithg-6.0.4.sh
$DIRNAME/install-bochs-2.6.5.sh

