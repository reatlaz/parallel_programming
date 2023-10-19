#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// номер группы 534
// место в списке 4
// X = 538
// A = 538 % 4 = 2
// B = 7 + 538 % 7 = 13

#define B 13
#define N size

#define THREAD_CREATE_ERROR -10
#define THREAD_JOIN_ERROR -11

#define INPUT_FILE "../input.jpeg"
#define OUTPUT_FILE "output.jpeg"

char* f;
const int x = B;
int size;
int total = 0;

pthread_mutex_t mutex;

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

typedef struct arguments {
    int threadnum;
} arguments_t;

void * do_stuff(void * args) {
    arguments_t *arg = (arguments_t*) args;
    int threadnum = arg->threadnum;
    int k;
    int i;
    // int local_total = 0;
    for (i = 0; i < N/B; i++) {
        k = i * B + threadnum;
        if (k >= N) break;
        f[k] += (k * x) & 255;
        pthread_mutex_lock(&mutex);
        total++;
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}


int main() {

    read_file();
    pthread_t threads[B];
    arguments_t args[B];
    int status;
    int status_addr;

    for (int i = 0; i < B; i++) {
        args[i].threadnum = i;
    }

    pthread_mutex_init(&mutex, NULL);

    // создание потоков
    for (int i = 0; i < B; i++) {
        status = pthread_create(&threads[i], NULL, do_stuff, (void*) &args[i]);
        if (status != 0) {
            printf("error: create thread, status = %d\n", status);
            exit(THREAD_CREATE_ERROR);
        }
    }

    // запуск потоков
    for (int i = 0; i < B; i++) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            printf("error: join thread, status = %d\n", status);
            exit(THREAD_JOIN_ERROR);
        }
    }

    // остаток от деления на B последовательностей байтов
    for (int i = N/B*B; i < N; i++) {
        f[i] += (i * x) & 255;
        pthread_mutex_lock(&mutex);
        total++;
        pthread_mutex_unlock(&mutex);
    };
    write_file();
    if (total == size)
        printf("SUCCESS\n");
    else
        printf("SOMETHING WENT WRONG\ntotal=%d; size=%d", total, size);

    return 0;


}
