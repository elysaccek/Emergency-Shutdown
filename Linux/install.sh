#!/bin/bash

echo "[*] Compiling kernel module..."
make
if [ $? -ne 0 ]; then
    echo "[!] Compilation failed."
    exit 1
fi

echo "[*] Loading kernel module..."
sudo insmod emergency_linux.ko
if [ $? -ne 0 ]; then
    echo "[!] Failed to load kernel module."
    exit 1
fi

make clean
echo "[+] Module loaded successfully."
dmesg | tail -n 10
