#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

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

int read_file() {
    FILE *fh = fopen(INPUT_FILE, "rb");
    fseek(fh, 0, SEEK_END);
    size = ftell(fh);
    rewind(fh);
    f = (char *)malloc(size * sizeof(char));
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

//typedef struct arguments {
//    int threadnum;
//} arguments_t;

void do_stuff(int i) {
//    arguments_t *arg = (arguments_t*) args;
//    int threadnum = arg->threadnum;
//    int k;
//    int i;
//    for (i = 0; i < N/B; i++) {
//        k = i * B + threadnum;
//        if (k >= N) break;
//        f[k] += (k * x) & 255;
//    }
    std::cout << i;
}


int main() {

    read_file();
    std::thread threads[B];
    // arguments_t args[B];
    int status;
    int status_addr;

//    for (int i = 0; i < B; i++) {
//        args[i].threadnum = i;
//    }

    // создание потоков
    for (int i = 0; i < B; i++) {
        threads[i] = std::thread(do_stuff, 1);
    }

    // запуск потоков
    for (int i = 0; i < B; i++) {
        threads[i].join();
    }

    // остаток от деления на B последовательностей байтов
//    for (int i = N/B*B; i < N; i++) {
//        f[i] += (i * x) & 255;
//    };
//    write_file();
//    printf("SUCCESS\n");

    return 0;


}
