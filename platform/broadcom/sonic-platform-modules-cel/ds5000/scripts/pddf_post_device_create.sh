#!/bin/bash

modprobe pddf_custom_fpga_extend

# Get BMC mode
PLATFORM=`sed -n 's/onie_platform=\(.*\)/\1/p' /host/machine.conf`
BMC_PRESENCE=`cat /sys/devices/platform/sys_cpld/bmc_present_l`
HW_SKU=`awk '{print $1}' /usr/share/sonic/device/$PLATFORM/default_sku`

# bmc not present
if [ ${BMC_PRESENCE} == "1" ]; then
	# attach ucdxxxx devices, bus 100, address 0x33 and 0x35
	echo ucd90320 0x33 > /sys/bus/i2c/devices/i2c-100/new_device
	echo ucd90160 0x35 > /sys/bus/i2c/devices/i2c-100/new_device
	
	# attach icp201xx devices, bus 77,78, address 0x63
	echo icp201xx 0x63 > /sys/bus/i2c/devices/i2c-77/new_device
	echo icp201xx 0x63 > /sys/bus/i2c/devices/i2c-78/new_device	
fi

modprobe i2c-imc
sleep 2

echo 0x0010 > /sys/devices/platform/cls_sw_fpga/FPGA/getreg
let boardRev=`cat /sys/devices/platform/cls_sw_fpga/FPGA/getreg`
lanemap=/usr/share/sonic/device/$PLATFORM/$HW_SKU/LANE_MAPPING-$boardRev.yml

for file in `ls /usr/share/sonic/device/$PLATFORM/$HW_SKU/*.config.yaml`
do
	grep PC_PM_CORE $file > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		eval $(grep -n PC_PM_CORE $file | awk 'BEGIN{FS=":"} {printf("start=%d",$1)}')
		let start=start-3; let end=`awk 'END {print NR}' $file`
		sed -i "${start},${end}d" $file
	fi
	cat $lanemap >> $file
	cat /usr/share/sonic/device/$PLATFORM/$HW_SKU/common.yaml >> $file
done

echo performance > /sys/module/pcie_aspm/parameters/policy

sync