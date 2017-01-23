#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int roomCheck(int yPos, int xPos, int yLen, int xLen, int hardness[105][160]);
char hardnessScan(int hardness[105][160], int roomLocations[][2], int curY, int curX, int roomNum);

int main(int argc, char *argv[])
{
  char dungeon[105][160];
  int hardness[105][160];
  int seed = time(NULL);
  srand(seed);
  int row, col, xLen, yLen, count = 0, xPos, yPos, success, roomX = 0, roomY = 0;

  // number of rooms in the dungeon
  int numRooms = rand() % (25 - 10) + 10;
  printf("%d\n", numRooms);

  int roomLocations[numRooms][2];

  // rock hardness generation
  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++)
    {
      dungeon[row][col] = ' ';
      if(row == 0 || row == 104 || col == 0 || col == 159)
        hardness[row][col] = 255;

      else hardness[row][col] = rand() % 255;
    }
  }

  // room generation, drawn with periods
  while(count < numRooms)
  {
    success = 0, roomX = 0;
    xPos = rand() % (160 - 1) + 1;
    yPos = rand() % (105 - 1) + 1;
    xLen = rand() % (15 - 7) + 7;
    yLen = rand() % (10 - 5) + 5;

    if(xPos + xLen < 160 && yPos + yLen < 105) {
      if(roomCheck(yPos, xPos, yLen, xLen, hardness)) {
          for(row = yPos; row < yPos + yLen; row++) {
            for(col = xPos; col < xPos + xLen; col++)
            {
              hardness[row][col] = 0;
              dungeon[row][col] = '.';
              success = 1;
            }
          }
        }
      }

    if(success == 1) {
     count++;
     roomLocations[roomY][roomX] = (yPos + yLen) / 2; roomX++;
     roomLocations[roomY][roomX] = (xPos + xLen) / 2; roomY++;
    }
  }

  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++) {
      printf("%c", dungeon[row][col]);
    }
    printf("\n");
  }

  //begin corridor generation, built with hashes (#)







}

int roomCheck(int yPos, int xPos, int yLen, int xLen, int hardness[105][160])
{
  // if(hardness[yPos][xPos - 2] == 0 || hardness[yPos - 2][xPos] == 0)
  //   return 0;
  //
  // if(hardness[yPos][xPos + xLen + 1] == 0 || hardness[yPos - 1][xPos + xLen] == 0)
  //   return 0;
  //
  // if(hardness[yPos + yLen][xPos - 2] == 0 || hardness[yPos + yLen + 1][xPos] == 0)
  //   return 0;
  //
  // if(hardness[yPos + yLen][xPos + xLen + 1] == 0 || hardness[yPos + yLen + 1][xPos + xLen] == 0)
  //   return 0;
  int i, j;
  for(i = yPos; i < yPos + yLen; i++)
    if(hardness[i][xPos - 1] == 0 || hardness[i][xPos + xLen] == 0)
      return 0;

  for(j = xPos; j < xPos + xLen; j++)
    if(hardness[yPos - 1][j] == 0 || hardness[yPos + yLen][j] == 0)
      return 0;

  return 1;
}

char hardnessScan(int hardness[105][160], int roomLocations[][2], int curY, int curX, int roomNum)
{
  //returns UP: 0; RIGHT: 1; DOWN: 2; LEFT: 3;
  int up = hardness[curY - 1][curX];
  int down = hardness[curY + 1][curX];
  int right = hardness[curY][curX + 1];
  int left = hardness[curY][curX - 1];
  int markX, markY;

  //direction check
  if(roomLocation[roomNum][0] > curX)
    markX = 1;
  else if(roomLocation[roomNum][0] < curX)
    markX = 0;
  if(roomLocation[roomNum][1] > curY)
    markY = 1;
  else if(roomLocation[roomNum][1] < curY)
    markY = 0;

  //hardness check
  if(markX && markY) {
    if(up < right)
      return 0;
    else if(right < up)
      return 1;
  }

  else if(markX) {
    if(down < right)
      return 2;
    else if(right < down)
      return 1;
  }

  else if(markY) {
    if(up < left)
      return 0;
    else if(left < up)
      return 3;
  }
  
  else {
    if(down < left)
      return 2;
    else if(left < down)
      return 3;
  }
}
