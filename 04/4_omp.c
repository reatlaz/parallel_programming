#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// номер группы 534
// место в списке 4
// X = 538
// A = 538 % 4 = 2
// B = 7 + 538 % 7 = 13

#define B 13
#define N size

int main() {
    FILE *fh = fopen("../input.jpeg", "rb");
    fseek(fh, 0, SEEK_END);
    int size = ftell(fh);
    rewind(fh);
    char* f;
    f = malloc(size * sizeof(char));
    int x = B;

    fread(f, sizeof(char), size, fh);

    fclose(fh);
    int i, j, k;
    #pragma omp parallel num_threads(B) private(i, j, k)
    {
        #pragma omp for nowait
        for (i = 0; i < B; i++) {
            for (j = 0; j < N/B; j++) {
                printf("%d ", i);
                k = j * B + i;
                if (k >= N) break;
                f[k] += (k * x) & 255;
            }
        }
        #pragma omp single
        for (i = N/B*B; i < N; i++) {
            printf("master: %d ", i);
            f[i] += (i * x) & 255;
        };
    }

    fh = fopen("output.jpeg", "wb"); 
    fwrite(f, sizeof(char), size, fh);
	fclose(fh);
    free(f);
    printf("SUCCESS\n");
    return 0;
}
