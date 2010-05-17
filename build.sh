#!/bin/sh

# Building system for 2ndboot-ng

BOOT_MODULE="xbootmod/2ndboot.ko"
BOOT_USER="xbootuser/2ndboot"
BOOT_IMAGE="boot/boot.elf"

echo "Building kernel module"
cd xbootmod
make
echo "Building user tool"
cd ../xbootuser
make
echo "Building bootloader"
cd ../boot
make
echo "Done!"
cd ..

# Copying files in out directory

if [ -d out ]; then
   echo "Directory in not empty! Erasing it!"
   rm -rf out
fi
mkdir out
if [ -f $BOOT_MODULE ]; then
   cp -f $BOOT_MODULE out/2ndboot.ko
else
   echo "Kernel module not found!"
fi
if [ -f $BOOT_USER ]; then
   cp -f $BOOT_USER out/2ndboot
else
   echo "User utility not found!"
fi
if [ -f $BOOT_IMAGE ]; then
   cp -f $BOOT_IMAGE out/boot.elf
else
   echo "Boot image not found!"
fi

# Cleaning tree after building

cd xbootmod
make clean
cd ../xbootuser
make clean
cd ../boot
make clean
cd ..

echo "Enjoy!"
