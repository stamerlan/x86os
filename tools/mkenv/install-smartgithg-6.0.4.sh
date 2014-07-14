#!/bin/bash

wget http://www.syntevo.com/download/smartgithg/smartgithg-6_0_4.deb -O smartgithg-6_0_4.deb

yes | sudo gdebi smartgithg-6_0_4.deb
rm -f smartgithg-6_0_4.deb
