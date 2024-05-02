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
    struct ioctl_args args_read;
    struct ioctl_args args_write;
    int img_size;
    int32_t index = 0;
    int32_t times_to_read = 0;
    int32_t buffer_size = 256;
    int32_t buffer_dir = 0x18;
    int8_t byte0;
    int8_t byte1;
    int8_t byte2;
    int8_t byte3;

    printf("*********************************\n");
    printf(">>> Opening character device\n");
    fd = open(chr_dev_name, O_RDWR);
    if (fd < 0) {
        perror("open(chr_dev_name, O_RDWR):");
        printf("Cannot open character device file...\n");
        return 0;
    }
    printf("*********************************\n");
    printf(">>> Sending signal to read img\n");
    args_write.dir = 0x14; // Dirección de escritura deseada
    args_write.value = 0x1; // Valor a escribir
    ioctl(fd, WR_VALUE, &args_write);
    printf("Valor 0x%x escrito en la dirección 0x%x\n", args_write.value, args_write.dir);

    printf("*********************************\n");
    printf(">>> Sending signal to read the img size\n");
    args_write.dir = 0x14; // Dirección de escritura deseada
    args_write.value = 0x3; // Valor a escribir
    ioctl(fd, WR_VALUE, &args_write);
    printf("Valor 0x%x escrito en la dirección 0x%x\n", args_write.value, args_write.dir);


    printf("*********************************\n");
    printf(">>> Reading the img size\n");
    args_read.dir = 0x0; // Dirección de lectura deseada
    args_read.value = 0x0;
    ioctl(fd, RD_VALUE, &args_read);
    printf("Valor leído desde la dirección 0x%x: 0x%x\n", args_read.dir, args_read.value);
    img_size = args_read.value;
    times_to_read = (img_size/buffer_size) + 1;
    char *img_char = (char *) malloc(img_size * sizeof(char));
    printf("*********************************\n");

    printf("*********************************\n");

    printf(">>> Reading img \n");

    printf("*********************************\n");
    for(int i = 0; i <= times_to_read; i++){

        args_write.dir = 0x14; // Dirección de escritura deseada
        args_write.value = 0x2; // Valor a escribir
        ioctl(fd, WR_VALUE, &args_write);

        for(int j = 0; j < 256; j += 4){

            if(index == img_size ){
                break;
            }

            args_read.dir = buffer_dir + j;
            args_read.value = 0x0;
            ioctl(fd, RD_VALUE, &args_read);
            byte0 = (args_read.value >> 24) & 0xFF;
            byte1 = (args_read.value >> 16) & 0xFF;
            byte2 = (args_read.value >> 8) & 0xFF;
            byte3 = args_read.value & 0xFF;

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

        
        if(index == img_size ){
            break;
        }
    }

    printf("*********************************\n");
    printf(" >>> Closing character device\n");
    printf("*********************************\n");
    close(fd);

    printf("*********************************\n");
    printf(" >>> openning file img\n");
    printf("*********************************\n");

    FILE *file_img_bmp = fopen("img_device.bmp", "wb");
    if (file_img_bmp == NULL) {
        printf("Cannot open img file.\n");
        return 1;
    }

    printf("*********************************\n");

    fwrite(img_char, 1, img_size, file_img_bmp);

    fclose(file_img_bmp);

    printf("Img saved.\n");

    printf("*********************************\n");

    //setenv("/usr/bin/python3.7", "/home/simics/.local/lib/python3.7/site-packages/Pillow",1);

    system("python3 filters.py");

}
