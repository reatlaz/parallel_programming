#include <stdio.h>

int main() {
    FILE *fh = fopen("barsiq.jpeg", "rb");
    fseek(fh, 0, SEEK_END);
    long size = ftell(fh);
    rewind(fh);

    char f[size];
    int x = 5;

    fread(f, sizeof(char), size, fh);

    fclose(fh);

    for (long i = 0; i < size; i++) {
        f[i] += (i * x) & 256;
    }

    fh = fopen("newbarsiq.jpeg", "wb"); 
    fwrite(f, sizeof(char), size, fh);
    return 0;
}
