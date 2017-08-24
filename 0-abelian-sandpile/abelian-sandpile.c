#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void dropSand(int map[23][23]);
void toppleSand(int map[23][23]);

int main(int argc, char *argv[])
{
    int map[23][23] = {{0}};
    int x = 0;
    int y = 0;

    while(1) {
        usleep(83333);
        for(y = 0; y < 23; y++) {
            for(x = 0; x < 23; x++) {
                printf("%d ", map[x][y]);
            }
            printf("\n");
        }
        dropSand(map);
    }

    return 0;
}

void dropSand(int map[23][23])
{
    map[11][11] += 1;
}

void handleSand(int map[23][23])
{
    int x = 0;
    int y = 0;

    for(y = 0; y < 23; y++) {
        for(x = 0; x < 23; x++) {
            
        }
    }
}
