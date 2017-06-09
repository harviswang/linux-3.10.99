#!/bin/sh

export PATH=$PATH:/usr/local/share/cross-compiler/bin
gmake m200_defconfig ARCH=mips CROSS_COMPILE=mipsel-unknown-linux-gnu-  &&
gmake V=0 -j1 zImage ARCH=mips CROSS_COMPILE=mipsel-unknown-linux-gnu-  &&

# ramdisk.cpio.gz comes from boot.img
#/home/ingenic/B/opensource/bootimg-tools/mkbootimg/mkbootimg --kernel  arch/mips/boot/compressed/zImage --ramdisk /home/ingenic/Downloads/test/aoe/ramdisk.cpio.gz --output boot.img

# ramdisk.cpio.gz comes from recovery.img
# /home/ingenic/B/opensource/bootimg-tools/mkbootimg/mkbootimg --kernel  arch/mips/boot/compressed/zImage --ramdisk /home/ingenic/Downloads/test/aoe/recovery/ramdisk.cpio.gz --output boot.img

# ramdisk.cp.gz comes from rootfs
/home/harvis/opensource/bootimg-tools/mkbootimg/mkbootimg --kernel  arch/mips/boot/compressed/zImage --ramdisk /home/harvis/Downloads/bootimage/ramdisk.cpio.gz --output boot.img

