#!/usr/bin/python

import os
import re
import struct
import subprocess

BMC_PRES_SYS_PATH = '/sys/devices/platform/sys_cpld/bmc_present_l'

class APIHelper():

    def __init__(self):
        pass
        
    def run_command(self, cmd):
        status = True
        result = ""
        try:
            p = subprocess.Popen(
                cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            raw_data, err = p.communicate()
            if err.decode('UTF-8') == '':
                result = raw_data.strip().decode('UTF-8')
        except Exception:
            status = False
        return status, result
        
    def get_register_value(self, getreg_path, register):
        cmd = "echo {1} > {0}; cat {0}".format(getreg_path, register)
        return self.run_command(cmd)
        
    def set_register_value(self, setreg_path, register, value):
        cmd = "echo {1} {2} > {0}".format(setreg_path, register, value)
        status, result = self.run_command(cmd)
        return status

    def cpld_lpc_read(self, reg):
        register = "0x{:X}".format(reg)
        return self.get_register_value("/sys/devices/platform/sys_cpld/getreg", register)

    def cpld_lpc_write(self, reg, val):
        register = "0x{:X}".format(reg)
        value = "0x{:X}".format(val)
        return self.set_register_value("/sys/devices/platform/sys_cpld/setreg", register, value)

    def grep(self, cmd, key):
        status, result = self.run_command("{0} | grep '{1}'".format(cmd, key))
        m = re.search(key, result)
        if status:
            status = True if m else False
        return status, result

    def read_txt_file(self, file_path):
        try:
            with open(file_path, 'r') as fd:
                data = fd.read()
                return data.strip()
        except IOError:
            pass
        return None
        
    def ipmi_fru(self, id=0, key=None):
        status = True
        result = ""
        cmd = "ipmitool fru print {0}".format(id)
        if not key:
            try:
                status, result = self.run_command(cmd)
            except:
                status = False
        else:
            status, result = self.grep(cmd, str(key))
        return status, result

    def with_bmc(self):
        """
        Get the BMC card present status

        Returns:
            A boolean, True if present, False if absent
        """
        presence = self.read_txt_file(BMC_PRES_SYS_PATH)
        if presence == None:
            print("Failed to get BMC card presence status")
        return True if presence == "0" else False

    def fsc_enable(self, enable=True):
        if self.with_bmc():
            if enable:
                status, result = self.run_command('ipmitool raw 0x2e 0x04 0xcf 0xc2 0x00 1 0 0')
            else:
                status, result = self.run_command('ipmitool raw 0x2e 0x04 0xcf 0xc2 0x00 1 0 1')
            return status
        else:
            return False

    def fsc_enabled(self):
        if self.with_bmc():
            status, result = self.run_command('ipmitool raw 0x2e 0x04 0xcf 0xc2 0x00 0x00 0')
            if status == True:
                data_list = result.split()
                if len(data_list) == 4:
                    if int(data_list[3]) == 0:
                        return True
        else:
            pass
        return False