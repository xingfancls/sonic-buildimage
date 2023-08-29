#!/bin/bash

declare -r CPLD_SETREG="/sys/bus/platform/devices/baseboard/setreg"
declare -r CPLD_GETREG="/sys/bus/platform/devices/baseboard/getreg"

# Load fpga extend driver after fpga device created
modprobe pddf_custom_fpga_extend

# attach ucd90120 devices, bus 107, devices 0x34 and 0x35
echo "ucd90120 0x34" > /sys/bus/i2c/devices/i2c-107/new_device
echo "ucd90120 0x35" > /sys/bus/i2c/devices/i2c-107/new_device

# attach mp2975 devices, bus 108, devices 0x70 and 0x7a
echo "mp2975 0x70" > /sys/bus/i2c/devices/i2c-108/new_device
echo "mp2975 0x7a" > /sys/bus/i2c/devices/i2c-108/new_device

# Set SYS_LED to Green, assuming everything came up fine.
echo "0xa162 0xdc" > ${CPLD_SETREG}

# Enable thermal shutdown by default
echo "0xa175 0x1" > ${CPLD_SETREG}
