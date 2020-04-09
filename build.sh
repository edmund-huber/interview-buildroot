#!/bin/bash

make qemu_arm_versatile_defconfig
make linux-reconfigure
make watchdog_petter-reconfigure
make all
