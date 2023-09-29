
#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <string>

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
    f = (char*)malloc(size * sizeof(char));
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

struct arguments_t {
    int threadnum;
};

unsigned int do_stuff(void * args) {
    arguments_t* arg = (arguments_t*) args;
    int threadnum = arg->threadnum;
    for (int i = 0; i < B; i++) {
        int k = i * (N/B) + threadnum;
        if (k<N)
            f[k] += (k * x) & 255;
    }
    return 0;
}


int main()
{
    arguments_t task[B];
    std::vector<std::thread> my_threads;
    for (int i = 0; i < B; i++)
    {
        task[i].threadnum = i;
        my_threads.push_back(std::thread(do_stuff, &task[i]));
    }
    auto master = my_threads.begin();
    while (master != my_threads.end())
    {
        master->join();
        master++;
    }

    return 0;
}
