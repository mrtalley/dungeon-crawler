#include <stdio.h>

int main(int argc, char *argv[])
{
    int map[23][23] = {{0}};
    int x = 0;
    int y = 0;

    for(y = 0; y < 23; y++) {
        for(x = 0; x < 23; x++) {
            printf("%d", map[x][y]);
        }
        printf("\n");
    }
}
