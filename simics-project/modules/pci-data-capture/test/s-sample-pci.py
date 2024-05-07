# This Software is part of Simics. The rights to copy, distribute,
# modify, or otherwise make use of this Software may be licensed only
# pursuant to the terms of an applicable license agreement.
# 
# Copyright 2012-2021 Intel Corporation

# Tests for the sample PCI device.

import stest
import dev_util as du
import pyobj

# Set up a PCI bus and a sample PCI device
pci_bridge = du.Dev([du.PciBridge])  # Non-used PCI bridge, required by bus
pci_conf = SIM_create_object('memory-space', 'pci_conf', [])
pci_io = SIM_create_object('memory-space', 'pci_io', [])
pci_mem = SIM_create_object('memory-space', 'pci_mem', [])

pci_bus = SIM_create_object('pci-bus', 'pci_bus', [['conf_space', pci_conf],
                                                   ['io_space', pci_io],
                                                   ['memory_space', pci_mem],
                                                   ['bridge', pci_bridge.obj]])

pci = SIM_create_object('pci_data_capture', 'pci_data_capture',
                        [['pci_bus', pci_bus]])

target_mem_space_test = SIM_create_object ('memory-space','target_mem_space',[])
dev = SIM_create_object('x_device', 'x_device', [['target_mem_space', target_mem_space_test],
                                                 ['img_path', 'img.bmp']])

pci.conexion_to_Device = dev

# Test the PCI vendor and device IDs
def test_ids():
    stest.expect_equal(pci.attr.pci_config_vendor_id, 0x104c, "Bad vendor ID")
    stest.expect_equal(pci.attr.pci_config_device_id, 0xac10, "Bad device ID")

# Test the registers of the device
def test_regs():
    # Verify that the buffer size is loaded and zero
    buffer_size = du.Register_LE((pci, 1, 0x4))
    stest.expect_equal(buffer_size.read(), 256)

    # Verify that the command register is loaded and zero
    cmd_reg = du.Register_LE((pci, 1, 0x14))
    stest.expect_equal(cmd_reg.read(), 0x0)

    # Verify that the file size is loaded and zero
    buffer_reg = du.Register_LE((pci, 1, 0x18))
    stest.expect_equal(buffer_reg.read(), 0x0)

def test_write_and_read():

    # Test setting BAR to map the device in memory
    cmd_reg = du.Register_LE((pci, 1, 0x14))
    buffer_reg = du.Register_LE((pci, 1, 0x18))
    file_size = du.Register_LE((pci, 1, 0x00))

    # Verify that the size of file is zero sice it has not been read yet
    stest.expect_equal(file_size.read(), 0)

    # Verify that the buffer is empty
    stest.expect_equal(buffer_reg.read(), 0)

    # Writes a 0x1 to the device, which loads the file size to the x-device
    cmd_reg.write(0x1)

    # Writes a 0x3 to the device, which loads the file size to the PCI device
    cmd_reg.write(0x3)

    # Verify that the size of file is not zero since it has been read
    stest.expect_equal(file_size.read(), 180138)

    # Writes a 0x2 to the device, which loads the image values to the x-device and transfers the buffer to the PCI device
    cmd_reg.write(0x2)

    # Verify that the buffer is not empty
    stest.expect_equal(buffer_reg.read(), 0xbfaa4d42)

    
stest.trap_log('info', obj = None)
stest.collect_failures()
test_ids()
test_regs()
test_write_and_read()
stest.check_failures()


print("All tests passed.")
