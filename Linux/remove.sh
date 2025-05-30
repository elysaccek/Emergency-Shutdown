#!/bin/bash

echo "[*] Unloading kernel module..."
sudo rmmod emergency_linux
if [ $? -ne 0 ]; then
    echo "[!] Failed to unload module. Maybe it wasn't loaded?"
    exit 1
fi

echo "[*] Cleaning build files..."
make clean

echo "[+] Module removed successfully."
