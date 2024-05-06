#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "src/unity.c"

#define CHARACTER_DEVICE_DRIVER_PATH "/dev/pci_capture_chr_dev-0"
#define WR_VALUE _IOW('a','a',int32_t *)
#define RD_VALUE _IOR('b','b',int32_t *)

void test_verify_empty_file_size(void);
void test_verify_empty_buffer(void);
void test_verify_not_empty_file_size(void);
void verify_not_empty_buffer(void);
void setUp(void);
void tearDown(void);

struct ioctl_args {
    int32_t value;
    int32_t dir;
};

int fd;
const char *chr_dev_name = CHARACTER_DEVICE_DRIVER_PATH;
struct ioctl_args args;

int main()
{
    
    printf("*********************************\n");
    printf(">>> Opening character device\n");
    fd = open(chr_dev_name, O_RDWR);
    if (fd < 0) {
        perror("open(chr_dev_name, O_RDWR):");
        printf("Cannot open character device file...\n");
        return 0;
    }
    printf("********** Running Tests *********\n");
    
    UNITY_BEGIN();
    RUN_TEST(test_verify_not_empty_file_size);
    RUN_TEST(verify_not_empty_buffer);

    printf("*********************************\n");
    printf(" >>> Closing character device\n");
    printf("*********************************\n");
    close(fd);
    return UNITY_END();
}

void test_verify_not_empty_file_size(){
    // Writes a 0x1 to the device, which loads the file size to the x-device
    args.dir = 0x14;
    args.value = 0x01;
    ioctl(fd, WR_VALUE, &args);

    // Writes a 0x3 to the device, which loads the file size to the PCI device
    args.dir = 0x14;
    args.value = 0x03;
    ioctl(fd, WR_VALUE, &args);

    // Verify that the size of file is not zero since it has been read
    args.dir = 0x00;
    args.value = 0x00;
    ioctl(fd, RD_VALUE, &args);
    TEST_ASSERT_EQUAL_UINT32(180138, args.value);
}

void verify_not_empty_buffer(){
    // Writes a 0x2 to the device, which loads the image values to the x-device and transfers the buffer to the PCI device
    args.dir = 0x14;
    args.value = 0x02;
    ioctl(fd, WR_VALUE, &args);

    // Verify that the buffer is not empty
    args.dir = 0x18;
    args.value = 0x00;
    ioctl(fd, RD_VALUE, &args);
    TEST_ASSERT_EQUAL_UINT32(0xbfaa4d42, args.value);
}


void setUp()
{

}

void tearDown()
{

}
