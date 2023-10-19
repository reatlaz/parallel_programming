#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

// номер группы 534
// место в списке 4
// X = 538
// A = 538 % 4 = 2
// B = 7 + 538 % 7 = 13

#define B 13
#define N size

#define INPUT_FILE "../input.jpeg"
#define OUTPUT_FILE "output.jpeg"

char* f;
const int x = B;
int size;

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

int main(int argc, char *argv[])
{
    pid_t pids[B];
    read_file();

    void *memory_map = mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    printf("Memory map created: <%p>\n", memory_map);
    for(int i=0; i<size; i++){
        *((char *) memory_map + (sizeof(char) * i)) = f[i];
    }
    char map_value;
    for(int procnum=0; procnum<B; procnum++)
    {
        pid_t pid = fork();
        if (pid) {
            // master
            pids[procnum] = pid;
        } else {
            // worker
            int k;
            for (int i = 0; i < N/B; i++) {
                k = i * B + procnum;
                if (k >= N) break;
                map_value = *((char *) memory_map + (k * sizeof(char)));
                map_value += (k * x) & 255;
                *((char *) memory_map + (sizeof(char) * k)) = map_value;
            }
            printf("procnum=%d\n", procnum);
            return 0;
        }
    }
    // остаток от деления на B последовательностей байтов
    for (int i = N/B*B; i < N; i++)
    {
        map_value = *((char *) memory_map + (i * sizeof(char)));
        map_value += (i * x) & 255;
        *((char *) memory_map + (sizeof(char) * i)) = map_value;
    }
    printf("waiting for child processes\n");
    for (int i = 0; i < B; i++)
        waitpid(pids[i], NULL, 0);
    printf("child processes finished\n");

    for(int i=0; i<size; i++){
        f[i] = *((char *) memory_map + (i * sizeof(char)));
    }

    write_file();
    printf("SUCCESS\n");
    return 0;
}