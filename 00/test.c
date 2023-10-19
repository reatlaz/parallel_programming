#include <stdio.h>

int main() {
    int x[5];
    int y = sizeof(x) / sizeof(int);
    printf("%d", y);
}
