#!/bin/sh
cd libquo
./autogen
./configure CC=mpicc --prefix=`pwd`
make
make install
