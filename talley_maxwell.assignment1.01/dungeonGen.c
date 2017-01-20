#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
  //char dungeon[105][160];
  int hardness[105][160];
  int seed = time(NULL);
  srand(seed);
  int row, col;

  // room generation
  // rooms drawn with periods, corridors/tunnels with hashes(#) and
  // rock with spaces

  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++)
    {
      if(row == 0 || row == 104 || col == 0 || col == 159)
        hardness[row][col] = 255;

      else hardness[row][col] = rand() % 255 - 1;
    }
  }



}
