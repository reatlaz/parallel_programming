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
#define SEMAPHORE_CAP 3

#define THREAD_CREATE_ERROR -10
#define THREAD_JOIN_ERROR -11

#define INPUT_FILE "../input.jpeg"
#define OUTPUT_FILE "output.jpeg"
char* f;
const int x = B;
int size;
HANDLE my_semaphore;

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

DWORD __stdcall do_stuff(void * args) {
    DWORD dwWaitResult; 
    BOOL bContinue=TRUE;

    while(bContinue)
    {
        // Try to enter the semaphore gate.

        dwWaitResult = WaitForSingleObject( 
            my_semaphore,   // handle to semaphore
            0L);           // zero-second time-out interval

        switch (dwWaitResult) 
        { 
            // The semaphore object was signaled.
            case WAIT_OBJECT_0: 
                arguments_t *arg = (arguments_t*) args;
                int threadnum = arg->threadnum;
                int k;
                int i;
                for (i = 0; i < N/B; i++) {
                    k = i * B + threadnum;
                    if (k >= N) break;
                    f[k] += (k * x) & 255;
                }

                bContinue=FALSE;

                // Release the semaphore when task is finished

                if (!ReleaseSemaphore( 
                        my_semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       // not interested in previous count
                {
                    printf("ReleaseSemaphore error: %d\n", GetLastError());
                }
                break; 
            // The semaphore was nonsignaled, so a time-out 
            case WAIT_TIMEOUT: 
                break; 
        }
    }
    return 1;
}


int main() {

    read_file();
    HANDLE threads[B];
    DWORD tid[B];
    arguments_t args[B];
    int status;
    int status_addr;

    my_semaphore = CreateSemaphore( 
        NULL,           // default security attributes
        SEMAPHORE_CAP,  // initial count
        SEMAPHORE_CAP,  // maximum count
        NULL);          // unnamed semaphore

    for (int i = 0; i < B; i++) {
        args[i].threadnum = i;
    }

    // создание потоков
    for (int i = 0; i < B; i++) {
        threads[i] = CreateThread(NULL, 0, do_stuff, &args[i], 0, tid+i);

    }

    // остаток от деления на B последовательностей байтов
    for (int i = N/B*B; i < N; i++) {
        f[i] += (i * x) & 255;
    };

    WaitForMultipleObjects(B, threads, 1, INFINITE);
    write_file();
    printf("SUCCESS\n");

    for(int i=0; i < B; i++ )
        CloseHandle(threads[i]);
    CloseHandle(my_semaphore);
    return 0;


}
