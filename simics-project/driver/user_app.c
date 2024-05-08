#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#define CHARACTER_DEVICE_DRIVER_PATH "/dev/pci_capture_chr_dev-0"
#define WR_VALUE _IOW('a','a',int32_t *)
#define RD_VALUE _IOR('b','b',int32_t *)

struct ioctl_args {
    int32_t value;
    int32_t dir;
};


int main()
{
    int fd;
    const char *chr_dev_name = CHARACTER_DEVICE_DRIVER_PATH;
    //structs to read and write
    struct ioctl_args args_read;
    struct ioctl_args args_write;
    // size img
    int img_size;
    // index for know how many bytes have been read and indicate the pos of every byte
    int32_t index = 0;
    // calculate how many ciclyes are requeried
    int32_t times_to_read = 0; 
    int32_t buffer_size = 256;
    int32_t buffer_dir = 0x18; //0x18 is the initial reg from buffer
    //bytes for split 32 bit
    int8_t byte0;
    int8_t byte1;
    int8_t byte2;
    int8_t byte3;

    printf("[User App Info] Opening character device\n");
    fd = open(chr_dev_name, O_RDWR);
    if (fd < 0) {
        perror("[User App Info] open(chr_dev_name, O_RDWR):");
        printf("[User App Info] Cannot open character device file...\n");
        return 0;
    }
    printf("[User App Info] Sending signal to read img\n");
    args_write.dir = 0x14; //0x14 is cmd register
    args_write.value = 0x1; //0x1 is start to read
    ioctl(fd, WR_VALUE, &args_write);
    printf("[User App Info] Write 0x%x in address 0x%x\n", args_write.value, args_write.dir);

    printf("[User App Info] Sending signal to read the img size\n");
    args_write.dir = 0x14;  //0x14 is cmd register
    args_write.value = 0x3; //0x1 is get the file size
    ioctl(fd, WR_VALUE, &args_write);
    printf("[User App Info] Write 0x%x in address 0x%x\n", args_write.value, args_write.dir);

    printf("[User App Info] Reading the img size\n");
    args_read.dir = 0x0; // 0x0 is the reg where the size is stored
    args_read.value = 0x0; // 0x0 is for set the value to 0
    ioctl(fd, RD_VALUE, &args_read);
    printf("[User App Info] Read in address 0x%x: 0x%x\n", args_read.dir, args_read.value);
    img_size = args_read.value;
    times_to_read = (img_size/buffer_size) + 1;
    char *img_char = (char *) malloc(img_size * sizeof(char));

    printf("[User App Info] Reading img \n");
    for(int i = 0; i <= times_to_read; i++){

        // Say "continue to read" to pci and device
        args_write.dir = 0x14; //0x14 is cmd register
        args_write.value = 0x2; //0x2 is continue to read
        ioctl(fd, WR_VALUE, &args_write);

        for(int j = 0; j < 256; j += 4){
            
            //if they have been read break the cicle
            if(index == img_size ){
                break;
            }

            // Read 32 bits from PCI Buffer
            args_read.dir = buffer_dir + j; // 0x18 is the initial reg from buffer and j is the offset
            args_read.value = 0x0;
            ioctl(fd, RD_VALUE, &args_read);
            
            // Split bytes with masks
            byte0 = (args_read.value >> 24) & 0xFF; 
            byte1 = (args_read.value >> 16) & 0xFF;
            byte2 = (args_read.value >> 8) & 0xFF;
            byte3 = args_read.value & 0xFF;

            // Save in array. 
            if(index != img_size - 1){
                img_char[index] = *(char*)&byte3;
                index++;
            }
            if(index != img_size - 1){
                img_char[index] = *(char*)&byte2;
                index++;
            }
            if(index != img_size - 1){
                img_char[index] = *(char*)&byte1;
                index++;
            }
            if(index != img_size - 1){
                img_char[index] = *(char*)&byte0;
                index++;
            }

        }

        //if they have been read break the cicle
        if(index == img_size ){
            break;
        }
    }

    printf("[User App Info] Closing character device\n");
    close(fd);

    printf("[User App Info] openning file img\n");
    FILE *file_img_bmp = fopen("./images/img_device.bmp", "wb");
    if (file_img_bmp == NULL) {
        printf("[User App Info] Cannot open img file.\n");
        return 1;
    }

    fwrite(img_char, 1, img_size, file_img_bmp);

    fclose(file_img_bmp);

    printf("[User App Info] Img saved.\n");

    //call python to apply filters
    system("python3 filters.py");

}
