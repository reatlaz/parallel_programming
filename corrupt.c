#include <stdio.h>

int main() {
    FILE *fh = fopen("barsiq.jpeg", "rb");
    fseek(fh, 0, SEEK_END);
    long size = ftell(fh);
    rewind(fh);

    char f[size];
    int x = 13;

    fread(f, sizeof(char), size, fh);

    fclose(fh);

    for (long i = 0; i < size ; i++) {
        if (i < size *13/13)
        f[i] -= (i * x) & 255;
    }

    fh = fopen("input.jpeg", "wb"); 
    fwrite(f, sizeof(char), size, fh);
	fclose(fh);
    printf("file corrupted successfully: input.jpeg\n");
    return 0;
}
