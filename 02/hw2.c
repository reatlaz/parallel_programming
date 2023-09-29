#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// номер группы 534
// место в списке 4
// X = 538
// A = 2
// B = 13

#define B 13

#define N size

#define THREAD_CREATE_ERROR -10
#define THREAD_JOIN_ERROR -11

char* f;
int x = B;
int size;

int read_file() {
    FILE *fh = fopen("../input.jpeg", "rb");
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
    int threadnum;
} arguments_t;

void * do_stuff(void * args) {
    arguments_t *arg = (arguments_t*) args;
    int threadnum = arg->threadnum;
    for (int i = 0; i < B; i++) {
        int k = i * (N/B) + threadnum;
        if (k<N)
            f[k] += (k * x) & 255;
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

    for (int i = N/B*B; i < N; i++) {
        printf("%d %d\n", i, N);
        f[i] += (i * x) & 255;
    }
    printf("%d %d\n", N/B*B+1, N);


    // free(status_addr);
    write_file();
    printf("N=%d\n", N);
    printf("SUCCESS\n");

    // char charVariable;
    // scanf("%c", &charVariable);
    return 0;


}
