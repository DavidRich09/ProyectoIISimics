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
    struct ioctl_args args;

    printf("*********************************\n");
    printf(">>> Opening character device\n");
    fd = open(chr_dev_name, O_RDWR);
    if (fd < 0) {
        perror("open(chr_dev_name, O_RDWR):");
        printf("Cannot open character device file...\n");
        return 0;
    }
    printf("*********************************\n");

    //Para escribir
    args.dir = 0x14; // Dirección de escritura deseada
    args.value = 0x1; // Valor a escribir
    ioctl(fd, WR_VALUE, &args);
    printf("Valor 0x%x escrito en la dirección 0x%x\n", args.value, args.dir);

    args.dir = 0x14; // Dirección de escritura deseada
    args.value = 0x3; // Valor a escribir
    ioctl(fd, WR_VALUE, &args);
    printf("Valor 0x%x escrito en la dirección 0x%x\n", args.value, args.dir);

        // Para leer
    args.dir = 0x0; // Dirección de lectura deseada
    args.value = 0x0; // Valor a escribir
    ioctl(fd, RD_VALUE, &args);
    printf("Valor leído desde la dirección 0x%x: 0x%x\n", args.dir, args.value);

    printf("*********************************\n");
    printf(" >>> Closing character device\n");
    printf("*********************************\n");
    close(fd);
}
