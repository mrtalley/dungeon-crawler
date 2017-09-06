#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXROOMS 10
#define COLS 80
#define ROWS 21

typedef struct dungeon {
  uint32_t num_rooms;
  uint8_t hardness[COLS][ROWS];
  char map[COLS][ROWS];
  unsigned char rooms[MAXROOMS][2];
} dungeon_t;

int generateRandom(int max, int min) {
  return (rand() % (max - min)) + min;
}

static void createEmptyMap(dungeon_t *dungeon) {
  srand((unsigned) time(NULL));
  int y = 0, x = 0;
  for(y = 0; y < ROWS; y++) {
    for(x = 0; x < COLS; x++) {
      if(x == 0 || x == 79) {
        dungeon->map[x][y] = '|';
        dungeon->hardness[x][y] = 255;
      }
      else if(y == 0 || y == 20) {
        dungeon->map[x][y] = '-';
        dungeon->hardness[x][y] = 255;
      }
      else {
        dungeon->map[x][y] = ' ';
        dungeon->hardness[x][y] = generateRandom(254, 1);
      }
    }
  }
}

int roomCheck(int xPos, int yPos, int xSize, int ySize, dungeon_t *dungeon){
  int y, x;
  for(y = yPos; y < yPos + ySize; y++) {
    if(dungeon->hardness[xPos - 1][y] == 0
      || dungeon->hardness[xPos + xSize][y] == 0)
      {
        return 0;
      }
  }

  for(x = xPos; x < xPos + xSize; x++) {
    if(dungeon->hardness[x][yPos - 1] == 0
      || dungeon->hardness[x][yPos + ySize] == 0)
      {
        return 0;
      }
  }

  return 1;
}

static void generateRooms(dungeon_t *dungeon) {
  srand((unsigned) time(NULL));
  int i = 0, j = 0;
  int max = 10, min = 5;
  dungeon->num_rooms = generateRandom(max, min);
  int numRooms = 0;
  int xMin = 3, yMin = 5, xMax = 10, yMax = 15;
  int x = 0, y = 0, xSize = 0, ySize = 0;
  int success = 1;
  int tries = 0;
  int roomPosition = 0;

  while(numRooms <= dungeon->num_rooms) {
    x = generateRandom(COLS - 1, 1);
    y = generateRandom(ROWS - 1, 1);
    xSize = generateRandom(xMax, xMin);
    ySize = generateRandom(yMax, yMin);

    if(!(x + xSize < COLS && y + ySize < ROWS)) {
      success = 0;
    }

    if(!(roomCheck(x, y, xSize, ySize, dungeon)) && success) {
      success = 0;
    }

    if(success) {
      for(j = y; j < y + ySize; j++) {
        for(i = x; i < x + xSize; i++) {
          dungeon->map[i][j] = '.';
          dungeon->hardness[i][j] = 0;
        }
      }

      dungeon->rooms[numRooms][roomPosition] = x + (xSize / 2);
      roomPosition++;
      dungeon->rooms[numRooms][roomPosition] = y + (ySize / 2);
      numRooms++;
      roomPosition = 0;
    }
    success = 1;
    tries++;
  }

  printf("Rooms generated in %d tries\n", tries);
}

static void printMap(dungeon_t *dungeon) {
  int y = 0, x = 0;
  for(y = 0; y < ROWS; y++) {
    for(x = 0; x < COLS; x++) {
      printf("%c", dungeon->map[x][y]);
    }
    printf("\n");
  }
}

static void generateCorridors(dungeon_t *dungeon) {
  int count = 1;
  int secondX, secondY, firstX, firstY;

  while(count <= dungeon->num_rooms) {
    secondX = dungeon->rooms[count][0];
    secondY = dungeon->rooms[count][1];
    firstX = dungeon->rooms[count - 1][0];
    firstY = dungeon->rooms[count - 1][1];

    while(firstX != secondX) {
      if(firstX > secondX) {
        firstX--;
      }
      else if(firstX < secondX) {
        firstX++;
      }

      if(dungeon->hardness[firstX][firstY] != 0) {
        dungeon->map[firstX][firstY] = '#';
        dungeon->hardness[firstX][firstY] = 0;
      }
    }

    while(firstY != secondY) {
      if(firstY > secondY) {
        firstY--;
      }
      else if(firstY < secondY) {
        firstY++;
      }

      if(dungeon->hardness[firstX][firstY] != 0) {
        dungeon->map[firstX][firstY] = '#';
        dungeon->hardness[firstX][firstY] = 0;
      }
    }
    count++;
  }
}

int main(int argc, char* argv[]) {
  dungeon_t dungeon;
  createEmptyMap(&dungeon);
  generateRooms(&dungeon);
  generateCorridors(&dungeon);
  printMap(&dungeon);
}
