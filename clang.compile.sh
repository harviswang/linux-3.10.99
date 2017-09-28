#!/bin/sh

#make distclean
#this function works
#gmake distclean ARCH=mips CROSS_COMPILE=mipsel-unknown-linux-gnu- HOSTCC=clang CC=clang HOSTCFLAGS='-I/usr/local/include -L/usr/local/lib/ -lintl' HOSTLDFLAGS='-L/usr/local/lib -lintl'

#make defconfig
#this function works
gmake m200_defconfig ARCH=mips CROSS_COMPILE=mipsel-unknown-linux-gnu- HOSTCC=clang CC=clang HOSTCFLAGS='-I/usr/local/include -L/usr/local/lib/ -lintl' HOSTLDFLAGS='-L/usr/local/lib -lintl'

#compile
#this function not works
gmake ARCH=mips CROSS_COMPILE=mipsel-unknown-linux-gnu- HOSTCC=clang CC=clang HOSTCFLAGS='-I/usr/local/include -L/usr/local/lib/ -lintl' HOSTLDFLAGS='-L/usr/local/lib -lintl'
