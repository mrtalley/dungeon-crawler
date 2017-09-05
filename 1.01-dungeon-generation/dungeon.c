#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>

struct Dungeon {
  unsigned char hardness[80][21];
  char map[80][21];
} dungeon;

static void createMapBorder(struct Dungeon *dungeon);
static void generateRooms(struct Dungeon *dungeon);
static void printMap(struct Dungeon *dungeon);
// static void generateHardness(struct Dungeon *dungeon);

int main(int argc, char* argv[]) {
  struct Dungeon dungeon;
  createMapBorder(&dungeon);
  generateRooms(&dungeon);
  printMap(&dungeon);
}

  static void createMapBorder(struct Dungeon *dungeon) {
    int y = 0, x = 0;
    for(y = 0; y < 21; y++) {
      for(x = 0; x < 80; x++) {
        if(x == 0 || x == 79) {
          dungeon->map[x][y] = '|';
        }
        else if(y == 0 || y == 20) {
          dungeon->map[x][y] = '-';
        }
        else {
          dungeon->map[x][y] = ' ';
        }
      }
    }
  }

  static void generateRooms(struct Dungeon *dungeon) {

  }

  static void printMap(struct Dungeon *dungeon) {
    int y = 0, x = 0;
    for(y = 0; y < 21; y++) {
      for(x = 0; x < 80; x++) {
        printf("%c", dungeon->map[x][y]);
      }
      printf("\n");
    }
  }

// static void generateHardness(struct Dungeon *dungeon) {
//   int y = 0, x = 0;
//   srand((unsigned) time(NULL));
//
//   for(y = 0; y < 21; y++) {
//     for(x = 0; x < 80; x++) {
//       if(dungeon->map[x][y] == '-' || dungeon->map[x][y] == '|') {
//         dungeon->hardness[x][y] = 255;
//       }
//       else if(dungeon->map[x][y] == '.') {
//         dungeon->hardness[x][y] = 0;
//       }
//       else {
//         dungeon->hardness[x][y] = rand() % 25
//       }
//     }
//   }
// }
