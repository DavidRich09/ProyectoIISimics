#include <stdio.h>
#include <stdlib.h>


int main() {
    FILE *file;
    FILE *file2;
    char input_file_name[] = "a.bmp"; // Name of the input file
    char input_txt_name[] = "img_device.bmp"; // Name of the input file

    // Open the file for reading
    file = fopen(input_file_name, "rb");
    file2 = fopen(input_txt_name, "rb");


    int sizeOfBuffer;

    fseek(file, 0L , 2);
    sizeOfBuffer = ftell(file);
    fseek(file, 0L, 0);

    int iguales = 0;
    int no_iguales = 1;

    char *img_char = (char *)malloc(sizeOfBuffer * sizeof(char));

    // Read characters from the file until EOF or buffer limit
    for (int index = 0; index < sizeOfBuffer; index++) {
        char char_read = fgetc(file); // Read a character
        char char_txt_read = fgetc(file2);

        printf("index: %d \n" , index);

        if(char_read == char_txt_read) {
            printf("iguales\n");
            iguales++;
        }else {
            printf("no iguales\n");
            no_iguales++;
        }
    }

    printf("iguales: %d \n" , iguales);
    printf("no_iguales: %d \n" , no_iguales);

    // Close the file



    return 0; // Return success
}