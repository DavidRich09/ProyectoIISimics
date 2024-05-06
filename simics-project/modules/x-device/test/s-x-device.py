import dev_util as du
import conf
import stest

target_mem_space_test = SIM_create_object ('memory-space','target_mem_space',[])
dev = SIM_create_object('x_device', 'x_device', [['target_mem_space', target_mem_space_test]])


# Test the registers of the device
def test_regs():
    buffer_size = du.Register(dev.bank.regs, 0x0)
    stest.expect_equal(buffer_size.read(), 0)

    cmd_reg = du.Register(dev.bank.regs, 0x4)
    stest.expect_equal(cmd_reg.read(), 0x0)

    file_size = du.Register(dev.bank.regs, 0x8)
    stest.expect_equal(file_size.read(), 0x0)

    file_offset = du.Register(dev.bank.regs, 0x12)
    stest.expect_equal(file_offset.read(), 0x0)

    buffer_reg = du.Register(dev.bank.regs, 0x16)
    stest.expect_equal(buffer_reg.read(), 0x0)

def test_write_and_read():
    pass

stest.trap_log('info', obj = None)
stest.collect_failures()
test_regs()
test_write_and_read()
stest.check_failures()

import dev_util as du
import conf
import stest

target_mem_space_test = SIM_create_object ('memory-space','target_mem_space',[])
dev = SIM_create_object('x_device', 'x_device', [['target_mem_space', target_mem_space_test]])


# Test the registers of the device
def test_regs():
    buffer_size = du.Register(dev.bank.regs, 0x0)
    stest.expect_equal(buffer_size.read(), 0)

    cmd_reg = du.Register(dev.bank.regs, 0x4)
    stest.expect_equal(cmd_reg.read(), 0x0)

    file_size = du.Register(dev.bank.regs, 0x8)
    stest.expect_equal(file_size.read(), 0x0)

    file_offset = du.Register(dev.bank.regs, 0x12)
    stest.expect_equal(file_offset.read(), 0x0)

    buffer_reg = du.Register(dev.bank.regs, 0x16)
    stest.expect_equal(buffer_reg.read(), 0x0)

def test_write_and_read():
    pass

stest.trap_log('info', obj = None)
stest.collect_failures()
test_regs()
test_write_and_read()
stest.check_failures()


print("All tests passed.")