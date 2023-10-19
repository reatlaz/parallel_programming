#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// номер группы 534
// место в списке 4
// X = 538
// A = 538 % 4 = 2
// B = 7 + 538 % 7 = 13

#define B 13

#define N size

#define THREAD_CREATE_ERROR -10
#define THREAD_JOIN_ERROR -11

#define INPUT_FILE "../../input.jpeg"
#define OUTPUT_FILE "output.jpeg"

int size;
char* f;
const int x = B;

int read_file() {
    FILE *fh = fopen(INPUT_FILE, "rb");
    fseek(fh, 0, SEEK_END);
    size = ftell(fh);
    rewind(fh);
    f = malloc(size * sizeof(char));
    if (f == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }
    fread(f, sizeof(char), size, fh);

    fclose(fh);
    return 0;
}

void write_file() {
    FILE *fh = fopen(OUTPUT_FILE, "wb"); 
    fwrite(f, sizeof(char), size, fh);
	fclose(fh);
    free(f);
}

int main(int argc, char *argv[]) {
    int procnum = atoi(argv[1]);
    printf("%d\n", procnum);
    int k;
    read_file();
    for (int i = 0; i < N/B; i++) {
        k = i * B + procnum;
        if (k >= N) break;
        f[k] += (k * x) & 255;
    }
    write_file();
    return 0;
}
