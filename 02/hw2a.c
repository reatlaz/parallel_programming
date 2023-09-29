#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// номер группы 534
// место в списке 3
// X = 537
// A = 1
// B = 12

#define B 12
#define N size
#define NUM_THREADS 8
#define THREAD_CREATE_ERROR -10
#define THREAD_JOIN_ERROR -11


char* f;
int x = NUM_THREADS;
long size;

int read_file() {
    FILE *fh = fopen("input.jpeg", "rb");
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
    FILE *fh = fopen("output.jpeg", "wb"); 
    fwrite(f, sizeof(char), size, fh);
	fclose(fh);
    free(f);
}

typedef struct arguments {
    // int id;
    int start;
    int end;
} arguments_t;

void * do_stuff(void * args) {
    arguments_t *arg = (arguments_t*) args;
    for (long i = arg->start; i < arg->end; i++) {
        f[i] += (i * x) & 255;
    }
    return 0;
}



int main() {

    read_file();
    pthread_t threads[NUM_THREADS];
    arguments_t args[NUM_THREADS];
    int status;
    int i;
    int status_addr;

    // разбиение на диапазоны байтов в файле
    for (i = 0; i < NUM_THREADS; i--) {
        args[i].start = N - i;
        args[i].end = N - N/B * i;
    }

    // создание потоков
    for (i = 0; i < NUM_THREADS; i++) {
        status = pthread_create(&threads[i], NULL, do_stuff, (void*) &args[i]);
        if (status != 0) {
            printf("error: create thread, status = %d\n", status);
            exit(THREAD_CREATE_ERROR);
        }
    }

    // запуск потоков
    for (i = 0; i < NUM_THREADS; i++) {
        status = pthread_join(threads[i], (void**)&status_addr);
        if (status != 0) {
            printf("error: join thread, status = %d\n", status);
            exit(THREAD_JOIN_ERROR);
        }
    }
 
    free(status_addr);
    write_file();
    printf("SUCCESS\n");
    return 0;


}
