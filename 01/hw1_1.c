#include <stdio.h>

#define FILE_NAME "barsiq.jpeg"
#define NEW_FILE_NAME "newbarsiq.jpeg"

int main() {
    FILE *fh = fopen(FILE_NAME, "rb");
    fseek(fh, 0, SEEK_END);
    long size = ftell(fh);
    rewind(fh);

    char f[size];
    int x = 5;

    fread(f, sizeof(char), size, fh);

    fclose(fh);

    // for (long i = 0; i < size; i++) {
    //     f[i] *= x;
    // }

    fh = fopen(NEW_FILE_NAME, "wb"); 
    fwrite(f, sizeof(unsigned char), size, fh);
    return 0;
}


