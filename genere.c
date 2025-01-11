#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int width = 20, height = 20;
    printf("1,%d,%d\n", width, height);

    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;
            printf("(%d.%d.%d) ", r, g, b);
        }
        printf("\n");
    }
    return 0;
}
