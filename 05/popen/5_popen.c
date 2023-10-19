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

int main() {
    STARTUPINFO si[B];
    PROCESS_INFORMATION pi[B];
    char cmd[512];
    ZeroMemory(&si, sizeof(si));
    for (int i = 0; i < B; i++)
    {
        si[i].cb = sizeof(si[i]);
    }
    ZeroMemory(&pi, sizeof(pi));

    // создание процессов
    for (int i = 0; i < B; i++){
        sprintf(cmd, "5_WorkerProcess.exe %d", i);
        // printf("%s\n", cmd);
        if(!CreateProcess(NULL,   // No module name (use command line)
                          cmd,        // Command line
                          NULL,           // Process handle not inheritable
                          NULL,           // Thread handle not inheritable
                          FALSE,          // Set handle inheritance to FALSE
                          0,              // No creation flags
                          NULL,           // Use parent's environment block
                          NULL,           // Use parent's starting directory
                          &si[i],            // Pointer to STARTUPINFO structure
                          &pi[i] )           // Pointer to PROCESS_INFORMATION structure
                )
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return 0;
        }
    }

    // остаток от деления на B последовательностей байтов
    read_file();
    for (int i = N/B*B; i < N; i++) {
        f[i] += (i * x) & 255;
    };
    write_file();
    
    for (int i = 0; i < B; i++){
        WaitForSingleObject(pi[i].hProcess, INFINITE);

        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }




    printf("SUCCESS\n");
    return 0;
}
