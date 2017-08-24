#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fillMapWithCommandLineValues(int map[23][23], int argc, char *argv[]);
void dropSand(int map[23][23]);
void toppleSand(int map[23][23]);

int main(int argc, char *argv[])
{
    int map[23][23] = {{0}};
    int x = 0;
    int y = 0;

    if(argc - 1 % 3 == 0 && argc != 1) {
        printf("%s\n", "Please put parameters on the command line in form of y x h");
        return 0;
    }

    fillMapWithCommandLineValues(map, argc, argv);

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

void fillMapWithCommandLineValues(int map[23][23], int argc, char *argv[])
{
    int i = 0, x = 0, y = 0, h = 0;

    for(i = 1; i < argc; i++) {
        y = atoi(argv[i]); i++;
        x = atoi(argv[i]); i++;
        h = atoi(argv[i]);

        map[x][y] = h;
    }
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
