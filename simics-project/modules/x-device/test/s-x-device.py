import dev_util as du
import conf
import stest

target_mem_space_test = SIM_create_object ('memory-space','target_mem_space',[])
dev = SIM_create_object('x_device', 'x_device', [['target_mem_space', target_mem_space_test],
                                                 ['img_path', 'img.bmp']])


# Test the registers of the device
def test_regs():

    # Verify that the buffer size is loaded and zero
    buffer_size = du.Register(dev.bank.regs, 0x00)
    stest.expect_equal(buffer_size.read(), 0)

    # Verify that the command register is loaded and zero
    cmd_reg = du.Register(dev.bank.regs, 0x04)
    stest.expect_equal(cmd_reg.read(), 0x0)

    # Verify that the file size is loaded and zero
    file_size = du.Register(dev.bank.regs, 0x08)
    stest.expect_equal(file_size.read(), 0x0)

    # Verify that the file offset is loaded and zero
    file_offset = du.Register(dev.bank.regs, 0x12)
    stest.expect_equal(file_offset.read(), 0x0)

    # Verify that the buffer is loaded and zero
    buffer_reg = du.Register(dev.bank.regs, 0x16)
    stest.expect_equal(buffer_reg.read(), 0x0)

def test_write_and_read():

    # Test setting BAR to map the device in memory
    cmd_reg = du.Register(dev.bank.regs, 0x04)
    file_size = du.Register(dev.bank.regs, 0x08)
    buffer_reg = du.Register(dev.bank.regs, 0x16)
    file_offset = du.Register(dev.bank.regs, 0x12)

    # Verify that the size of file is zero sice it has not been read yet
    stest.expect_equal(file_size.read(), 0)

    #Reads the file size
    cmd_reg.write(0x1)

    # Verify that size of file is not zero since it has been read
    stest.expect_equal(file_size.read(), 180138)

    # Verify that the buffer is empty
    stest.expect_equal(buffer_reg.read(), 0x0)

    # Verify that the file offset is zero
    stest.expect_equal(file_offset.read(), 0)

    # Writes the first 256 bytes of the file to the buffer
    cmd_reg.write(0x2)

    # Verify that the buffer is not empty
    stest.expect_equal(buffer_reg.read(), 0xbfaa4d42)

    # Verify that the file offset is 256
    stest.expect_equal(file_offset.read(), 256)
    

stest.trap_log('info', obj = None)
stest.collect_failures()
test_regs()
test_write_and_read()
stest.check_failures()

