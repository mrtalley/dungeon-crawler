#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int counter[24][80];
char world[24][80];
char temp[24][80];

void count(int row, int col);
void rules();
int adapt(int num, int mark);

int main(int argc, char *argv[]){
    int xStart = 0, yStart = 0, frame = 1, x, y, i;
    memset(world, ' ', sizeof(world));
    for(i = 1; i < argc; i++)
    {
        xStart = atoi(argv[i]);
        i++;
        yStart = atoi(argv[i]);
        world[yStart][xStart] = 'O';
        xStart = 0; yStart = 0;
    }
    memcpy(temp, world, sizeof(temp));
    while(1) {
        usleep(83333);
        for(y = 0; y < 24; y++) {
            for(x = 0; x < 80; x++) {
                printf("%c", world[y][x]);
                count(y,x);
            }
            printf("\n");
        }
        rules();
        memcpy(world, temp, sizeof(world));
        printf("%d\n",frame++);
    }
    return 0;
}

void count(int row, int col)
{
  int count = 0, i, j;
  for(i = row - 1; i < row + 2; i++) {
    for(j = col - 1; j <  col + 2; j++) {
      if(world[adapt(i, 0)][adapt(j, 1)] == 'O') {
        if(i == row && j == col) {}
        else count++;
      }
    }
  }
  counter[row][col] = count;
}

void rules() {
  int x, y;
  for(y = 0; y < 24; y++) {
    for(x = 0; x < 80; x++) {
      if((counter[y][x] == 0 || counter[y][x] == 1) && world[y][x] == 'O')
        temp[y][x] = ' ';
      else if((counter[y][x] == 2 || counter[y][x] == 3) && world[y][x] == 'O')
        temp[y][x] = 'O';
      else if(counter[y][x] >= 4 && world[y][x] == 'O')
        temp[y][x] = ' ';
      else if(counter[y][x] == 3 && world[y][x] == ' ')
        temp[y][x] = 'O';
    }
  }
}

int adapt(int num, int mark) {
  if(num == 80 && mark == 1)
    return 0;
  else if(num == 24 && mark == 0)
    return 0;
  else if(num == -1 && mark == 0) //y val
    return 23;
  else if(num == -1 && mark == 1) //x val
    return 79;
  else return num;
}
