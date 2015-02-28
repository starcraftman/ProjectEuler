#!/bin/sh
SRC=https://raw.githubusercontent.com/starcraftman/.my_scripts/master/archive/GetLibsI.py
curl -o GetLibsI.py $SRC
./GetLibsI.py boost
rm GetLibsI.py
