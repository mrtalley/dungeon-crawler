#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int roomCheck(int yPos, int xPos, int yLen, int xLen, int hardness[105][160]);

int main(int argc, char *argv[])
{
  char dungeon[105][160];
  int hardness[105][160];
  int seed = time(NULL);
  srand(seed);
  printf("%d\n", seed);
  int row, col, xLen, yLen, count = 0, xPos, yPos, success, roomX = 0;
  int roomY = 0, x0, x1, y0, y1;

  // number of rooms in the dungeon
  int numRooms = rand() % (25 - 10) + 10;
  printf("%d\n\n", numRooms);

  int roomLocations[numRooms][2];

  // rock hardness generation
  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++) {
      dungeon[row][col] = ' ';

      if(row == 0 || row == 104 || col == 0 || col == 159)
        hardness[row][col] = 255;

      else hardness[row][col] = rand() % (255 - 1) + 1;
    }
  }

  // room generation, drawn with periods
  while(count < numRooms) {
    success = 0, roomX = 0;
    xPos = rand() % (160 - 1) + 1;
    yPos = rand() % (105 - 1) + 1;
    xLen = rand() % (15 - 7) + 7;
    yLen = rand() % (10 - 5) + 5;

    if(xPos + xLen < 160 && yPos + yLen < 105) {
      if(roomCheck(yPos, xPos, yLen, xLen, hardness)) {
          for(row = yPos; row < yPos + yLen; row++) {
            for(col = xPos; col < xPos + xLen; col++) {
              hardness[row][col] = 0;
              dungeon[row][col] = '.';
              success = 1;
            }
          }
        }
      }

    if(success == 1) {
     roomLocations[roomY][roomX] = yPos + (yLen / 2); roomX++;
     roomLocations[roomY][roomX] = xPos + (xLen / 2); roomY++;
     count++;
    }
  }

  // begin corridor generation, built with hashes (#)
  count = 0;
  int roomNum = 1;
  while(count < numRooms - 1) {
    y0 = roomLocations[roomNum - 1][0];
    x0 = roomLocations[roomNum - 1][1];
    y1 = roomLocations[roomNum][0];
    x1 = roomLocations[roomNum][1];

    while(x0 != x1) {
      if(x0 > x1)
        x0--;
      else if(x0 < x1)
        x0++;

     if(hardness[y0][x0] != 0)
        dungeon[y0][x0] = '#';
    }

    while(y0 != y1) {
      if(y0 > y1)
        y0--;
      else if(y0 < y1)
        y0++;

      if(hardness[y0][x0] != 0)
         dungeon[y0][x0] = '#';
    }
    count++;
    roomNum++;
  }

  // print dungeon
  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++) {
      printf("%c", dungeon[row][col]);
    }
    printf("\n");
  }

  return 0;
}

/*
  Check around the potential room placement. If there is not another room 1 cell
  away, then function returns 1 so it will be set for that location, else returns
  0.
*/
int roomCheck(int yPos, int xPos, int yLen, int xLen, int hardness[105][160])
{
  int i, j;
  for(i = yPos; i < yPos + yLen; i++)
    if(hardness[i][xPos - 1] == 0 || hardness[i][xPos + xLen] == 0)
      return 0;

  for(j = xPos; j < xPos + xLen; j++)
    if(hardness[yPos - 1][j] == 0 || hardness[yPos + yLen][j] == 0)
      return 0;

  return 1;
}
