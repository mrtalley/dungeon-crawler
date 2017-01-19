#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int counter[24][80];
char world[24][80];
char temp[24][80];

void count(int row, int col);
void rules();

int main(int argc, char *argv[]){
    int xStart = 0;
    int yStart = 0;
    int frame = 1;
    int x, y, i;

    for(i = 1; i < argc; i++)
    {
        xStart = atoi(argv[i]);
        i++;
        yStart = atoi(argv[i]);
        world[yStart][xStart] = 'O';
        xStart = 0; yStart = 0;
    }

    for(y = 0; y < 24; y++)
    {
        for(x = 0; x < 80; x++)
        {
            if(world[y][x] != 'O')
                world[y][x] = ' ';

            // printf("%c", world[y][x]);
            count(y, x);
            temp[y][x] = world[y][x];
        }
    }
    while(1) {
        usleep(83333);
        for(y = 0; y < 24; y++)
        {
            for(x = 0; x < 80; x++)
            {
                printf("%c", world[y][x]);
                //printf("%2d", counter[y][x]);
                count(y,x);
            }
            printf("\n");
        }
        rules();
        memcpy(world, temp, sizeof(world));
        printf("%d\n",frame++);
    }
    // for(int i = 0; i < 24; i++)
    //   for(int j = 0; j < 80; j++)
    //     count(i, j);
    //
    // for(int row = 0; row < 24; row++) {
    //   for(int col = 0; col < 80; col++) {
    //     printf("%2d", counter[row][col]);
    //   }
    //   printf("\n");
    // }
    return 0;
}


void count(int row, int col)
{
  int count = 0, i, j;
  //top left
  if(row == 0 && col == 0)
  {
    if(world[0][79] == 'O')
      count++;
    if(world[1][79] == 'O')
      count++;
    if(world[23][79] == 'O')
      count++;
    if(world[23][78] == 'O')
      count++;
    if(world[23][77] == 'O')
      count++;
    if(world[0][1] == 'O')
      count++;
    if(world[1][0] == 'O')
      count++;
    if(world[1][1] == 'O')
      count++;
  }
  //top right
  else if(row == 0 && col == 79)
  {
    if(world[23][78] == 'O')
      count++;
    if(world[23][79] == 'O')
      count++;
    if(world[23][0] == 'O')
      count++;
    if(world[0][78] == 'O')
      count++;
    if(world[0][0] == 'O')
      count++;
    if(world[1][78] == 'O')
      count++;
    if(world[1][79] == 'O')
      count++;
    if(world[1][0] == 'O')
      count++;
  }
  //bottom left
  else if(row == 23 && col == 0)
  {
    if(world[22][79] == 'O')
      count++;
    if(world[22][0] == 'O')
      count++;
    if(world[22][1] == 'O')
      count++;
    if(world[23][79] == 'O')
      count++;
    if(world[23][1] == 'O')
      count++;
    if(world[0][79] == 'O')
      count++;
    if(world[0][0] == 'O')
      count++;
    if(world[0][1] == 'O')
      count++;
  }
  //bottom right
  else if(row == 23 && col == 79)
  {
    if(world[22][78] == 'O')
      count++;
    if(world[22][79] == 'O')
      count++;
    if(world[22][0] == 'O')
      count++;
    if(world[23][78] == 'O')
      count++;
    if(world[23][0] == 'O')
      count++;
    if(world[0][78] == 'O')
      count++;
    if(world[0][79] == 'O')
      count++;
    if(world[0][0] == 'O')
      count++;
  }
  //0 column
  else if(col == 0)
  {
    if(world[row + 1][79] == 'O')
      count++;
    if(world[row + 1][0] == 'O')
      count++;
    if(world[row + 1][1] == 'O')
      count++;
    if(world[row][79] == 'O')
      count++;
    if(world[row][1] == 'O')
      count++;
    if(world[row - 1][79] == 'O')
      count++;
    if(world[row - 1][0] == 'O')
      count++;
    if(world[row - 1][1] == 'O')
      count++;
  }
  //79 column
  else if(col == 79)
  {
    if(world[row + 1][78] == 'O')
      count++;
    if(world[row + 1][79] == 'O')
      count++;
    if(world[row + 1][0] == 'O')
      count++;
    if(world[row][78] == 'O')
      count++;
    if(world[row][0] == 'O')
      count++;
    if(world[row - 1][78] == 'O')
      count++;
    if(world[row - 1][79] == 'O')
      count++;
    if(world[row - 1][0] == 'O')
      count++;
  }
  //0 row
  else if(row == 0)
  {
    if(world[23][col - 1] == 'O')
      count++;
    if(world[23][col] == 'O')
      count++;
    if(world[23][col + 1] == 'O')
      count++;
    if(world[0][col - 1] == 'O')
      count++;
    if(world[0][col + 1] == 'O')
      count++;
    if(world[1][col - 1] == 'O')
      count++;
    if(world[1][col] == 'O')
      count++;
    if(world[1][col + 1] == 'O')
      count++;
  }
  //23 row
  else if(row == 23)
  {
    if(world[22][col - 1] == 'O')
      count++;
    if(world[22][col] == 'O')
      count++;
    if(world[22][col + 1] == 'O')
      count++;
    if(world[23][col - 1] == 'O')
      count++;
    if(world[23][col + 1] == 'O')
      count++;
    if(world[0][col - 1] == 'O')
      count++;
    if(world[0][col] == 'O')
      count++;
    if(world[0][col + 1] == 'O')
      count++;
  }
  //non edge case
  else
  {
    for(i = row - 1; i < row + 2; i++) {
      for(j = col - 1; j <  col + 2; j++) {
        if(world[i][j] == 'O') {
          if(i == row && j == col) {}
          else count++;
        }
      }
    }
  }
  counter[row][col] = count;
}

void rules() {
  int x, y;
  for(y = 0; y < 24; y++)
  {
    for(x = 0; x < 80; x++)
    {
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
