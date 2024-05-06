# This Software is part of Simics. The rights to copy, distribute,
# modify, or otherwise make use of this Software may be licensed only
# pursuant to the terms of an applicable license agreement.
# 
# Copyright 2012-2021 Intel Corporation

# Tests for the sample PCI device.

import stest
import dev_util as du
import pyobj

# Create a simple device class implementing the signal interface
class FakeInterface(pyobj.ConfObject):

    class start_read(pyobj.SimpleAttribute(1, 'i')):
        val = 0
    
    class read_buffer_device(pyobj.SimpleAttribute(1, 'i')):
        val = 0
    
    class read_file_size(pyobj.SimpleAttribute(1, 'i')):
        val = 0

    class control_Device_PCI(pyobj.Interface):
        def start_read(self):
            self._up.raised.start_read += 1
        def read_buffer_device (self):
            self._up.raised.read_buffer_device += 1
        def read_file_size (self):
            self._up.raised.read_file_size += 1

# Create a device instance of the fake PIC
#fake_interface = pre_conf_object('fake_interface', 'FakeInterface')
fake_interface = SIM_create_object('FakeInterface', 'fake_interface', [])

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

pci.conexion_to_Device = fake_interface

# Test the PCI vendor and device IDs
def test_ids():
    stest.expect_equal(pci.attr.pci_config_vendor_id, 0x104c, "Bad vendor ID")
    stest.expect_equal(pci.attr.pci_config_device_id, 0xac10, "Bad device ID")

# Test the registers of the device
def test_regs():
    buffer_size = du.Register_LE((pci, 1, 0x4))
    stest.expect_equal(buffer_size.read(), 256)

    cmd_reg = du.Register_LE((pci, 1, 0x14))
    stest.expect_equal(cmd_reg.read(), 0x0)

    buffer_reg = du.Register_LE((pci, 1, 0x18))
    stest.expect_equal(buffer_reg.read(), 0x0)

# Test setting BAR to map the device in memory
def test_write_and_read():
    cmd_reg = du.Register_LE((pci, 1, 0x14))
    buffer_reg = du.Register_LE((pci, 1, 0x18))

    stest.expect_exception(cmd_reg.write, 0x1, Exception)
    stest.expect_equal(pci.conexion_to_Device.start_read, 1)

    stest.expect_exception(cmd_reg.write, 0x2, Exception)
    stest.expect_equal(pci.conexion_to_Device.read_buffer_device, 1)

    stest.expect_exception(cmd_reg.write, 0x3, Exception)
    stest.expect_equal(pci.conexion_to_Device.read_file_size, 1)

    cmd_reg.write(0x4)
    stest.expect_equal(cmd_reg.read(), 0x4)

    buffer_reg.write(0xff)
    stest.expect_equal(buffer_reg.read(), 0xff)

    
stest.trap_log('info', obj = None)
stest.collect_failures()
test_ids()
test_regs()
test_write_and_read()
stest.check_failures()


print("All tests passed.")
