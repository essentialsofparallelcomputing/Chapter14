#!/bin/sh
cd libquo
autoreconf -i
./configure CC=mpicc --prefix=`pwd`
make
make install
