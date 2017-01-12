#include "game_of_life.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MIN(x, y)  (((x) > (y)) ? (x) : (y))
#define MAX(x, y)  (((x) < (y)) ? (x) : (y))

int counter(char *world[80]);
//in a 2D array, rows come first, then cols.

int main(int argc, char *argv[]){
    //swap
    //char world[80][24];
    char world[24][80];
    int xStart = 0;
    int yStart = 0;
    int frameCount = 1;
    //swap
    //int counter[80][24];
    int counter[24][80];
    
    for(int i = 1; i < argc; i++)
    {
        xStart = atoi(argv[i]);
        i++;
        yStart = atoi(argv[i]);
        world[xStart][yStart] = 'O';
        xStart = 0; yStart = 0;
    }
    
    //swap x and y for loop
    for(int y = 0; y < 24; y++)
    {
        for(int x = 0; x < 80; x++)
        {
            //swap
            if(world[y][x] != 'O')
            {
                world[y][x] = ' ';
            }
            
            printf("%c", world[x][y]);
            
        }
        printf("\n");
    }
    
    while(1) {
        usleep(83333);
        
        //swap x and y in for loop
        for(int y = 0; y < 24; y++)
        {
            for(int x = 0; x < 80; x++)
            {
                //swap
                if(world[y][x] != 'O')
                {
                    world[y][x] = ' ';
                }
                
                //swap
                printf("%c", world[y][x]);
                
            }
            printf("\n");
        }
    
        counter(world, counter);
        
    }
    
    
    return 0;
    
}


void counter(char *world[80], int *counter[80])
{
    //swap
    int cellCount[24][80];
    for(int m = 0; m < 24; m++)
    {
        for(int n = 0; n < 80; n++)
        {
            for(int i = MAX(0, m - 1); i <= MIN(m - 1, m + 1); i++)
            {
                for(int j = MAX(0, n - 1); j <= MIN(n - 1, n + 1); j++)
                {
                    //swap
                    //y, x
                    if(world[i][j] == 'O')
                        counter[n][m]++;
                }
            }
        }
    }
}
