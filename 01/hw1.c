#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fh = fopen("../input.jpeg", "rb");
    fseek(fh, 0, SEEK_END);
    int size = ftell(fh);
    rewind(fh);
    char* f;
    f = malloc(size * sizeof(char));
    int x = 13;

    fread(f, sizeof(char), size, fh);

    fclose(fh);

    for (int i = 0; i < size; i++) {
        f[i] += (i * x) & 255;
    }

    fh = fopen("output.jpeg", "wb"); 
    fwrite(f, sizeof(char), size, fh);
	fclose(fh);
    free(f);
    return 0;
}
