#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//#include <endian.h>
#include <stdint.h>
#include <limits.h>

#include "endian.h"
#include "heap.h"

#define MAXROOMS 10
#define COLS 80
#define ROWS 21

typedef struct corridor_path {
    heap_node_t *hn;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
  } corridor_path_t;

  typedef enum dim {
    dim_x,
    dim_y,
    num_dims
  } dim_t;

  typedef int16_t pair_t[num_dims];  

  #define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
  #define mapxy(x, y) (d->map[y][x])
  #define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
  #define hardnessxy(x, y) (d->hardness[y][x])

typedef struct dungeon {
    uint32_t num_rooms;
    uint8_t hardness[ROWS][COLS];
    char map[ROWS][COLS];
    unsigned char rooms[MAXROOMS][4]; // y, x, y-size, x-size
    uint32_t version;
} dungeon_t;

int generateRandom(int max, int min) {
    return (rand() % (max - min)) + min;
}

static int32_t corridor_path_cmp(const void *key, const void *with) {
    return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
}

static void createEmptyMap(dungeon_t *dungeon) {
    srand((unsigned) time(NULL));
    int y = 0, x = 0;
    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            if(x == 0 || x == 79) {
                dungeon->map[y][x] = '|';
                dungeon->hardness[y][x] = 255;
            }
            else if(y == 0 || y == 20) {
                dungeon->map[y][x] = '-';
                dungeon->hardness[y][x] = 255;
            }
            else {
                dungeon->map[y][x] = ' ';
                dungeon->hardness[y][x] = generateRandom(254, 1);
            }
        }
    }
}

int roomCheck(int xPos, int yPos, int xSize, int ySize, dungeon_t *dungeon) {
    int y, x;
    for(y = yPos; y < yPos + ySize; y++) {
        if(dungeon->hardness[y][xPos - 1] == 0
            || dungeon->hardness[y][xPos + xSize] == 0)
            {
                return 0;
            }
    }

    for(x = xPos; x < xPos + xSize; x++) {
        if(dungeon->hardness[yPos - 1][x] == 0
            || dungeon->hardness[yPos + ySize][x] == 0)
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

    while(numRooms < dungeon->num_rooms) {
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
                    dungeon->map[j][i] = '.';
                    dungeon->hardness[j][i] = 0;
                }
            }

            dungeon->rooms[numRooms][roomPosition] = y;
            roomPosition++;
            dungeon->rooms[numRooms][roomPosition] = x;
            roomPosition++;
            dungeon->rooms[numRooms][roomPosition] = ySize;
            roomPosition++;
            dungeon->rooms[numRooms][roomPosition] = xSize;
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
            printf("%c", dungeon->map[y][x]);
        }
        printf("\n");
    }
}

static void setRoomsFromFile(dungeon_t *d) {
    int i = 0, x = 0, y = 0;

    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            d->map[y][x] = ' ';
        }
    }

    for(i = 0; i < d->num_rooms; i++) {
        for(y = d->rooms[i][0]; y < d->rooms[i][0] + d->rooms[i][2]; y++) {
            for(x = d->rooms[i][1]; x < d->rooms[i][1] + d->rooms[i][3]; x++) {
                d->map[y][x] = '.';
            }
        }
    }
}

static void setMapFromFile(dungeon_t *d) {
    setRoomsFromFile(d);
    int y = 0, x = 0;

    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            if(d->map[y][x] == '.') {
                continue;
            }
            if(y == 0 || y == ROWS - 1) {
                d->map[y][x] = '-';
            }
            else if(x == 0 || x == COLS - 1) {
                d->map[y][x] = '|';
            }
            else if(d->hardness[y][x] == 0) {
                d->map[y][x] = '#';
            }
            else {
                d->map[y][x] = ' ';
            }
        }
    }
}

static void generateCorridors(dungeon_t *dungeon) {
    int count = 1;
    int secondX, secondY, firstX, firstY;

    while(count < dungeon->num_rooms) {
        secondY = dungeon->rooms[count][0] + dungeon->rooms[count][2] / 2;
        secondX = dungeon->rooms[count][1] + dungeon->rooms[count][3] / 2;
        firstY = dungeon->rooms[count - 1][0] + dungeon->rooms[count - 1][2] / 2;
        firstX = dungeon->rooms[count - 1][1] + dungeon->rooms[count - 1][3] / 2;

        while(firstX != secondX) {
            if(firstX > secondX) {
                firstX--;
            }
            else if(firstX < secondX) {
                firstX++;
            }

            if(dungeon->hardness[firstY][firstX] != 0) {
                dungeon->map[firstY][firstX] = '#';
                dungeon->hardness[firstY][firstX] = 0;
            }
        }

        while(firstY != secondY) {
            if(firstY > secondY) {
                firstY--;
            }
            else if(firstY < secondY) {
                firstY++;
            }

            if(dungeon->hardness[firstY][firstX] != 0) {
                dungeon->map[firstY][firstX] = '#';
                dungeon->hardness[firstY][firstX] = 0;
            }
        }
        count++;
    }
}

void writeToFile(dungeon_t *dungeon, FILE *file) {
    printf("Saving to file\n");
    dungeon->version = 0;
    uint32_t size = 0;

    // Name of File, Bytes 0 - 5
    fwrite("RLG327", 1, 6, file);

    // File Version Number, Bytes 6 - 9
    dungeon->version = htobe32(dungeon->version);
    fwrite(&dungeon->version, 4, 1, file);

    // Size of File, Bytes 10 - 13
    size = 1694 + dungeon->num_rooms * 4;
    size = htobe32(size);
    fwrite(&size, 4, 1, file);

    // Dungeon Hardness, Bytes 14 - 1693
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            fwrite(&dungeon->hardness[y][x], 1, 1, file);
        }
    }

    // Location of All Rooms, Bytes 1694 - eof
    for(int i = 0; i < dungeon->num_rooms; i++) {
        for(int j = 0; j < 4; j++) {
            fwrite(&dungeon->rooms[i][j], 1, 1, file);
        }
    }
}

void loadFromFile(dungeon_t *dungeon, FILE *file) {
    printf("Loading from file\n");
    dungeon->version = 0;
    uint32_t size = 0;

    fseek(file, 6, SEEK_SET);

    // File Version Number, Bytes 6 - 9
    fread(&dungeon->version, 4, 1, file);

    // Size of File, Bytes 10 - 13
    fread(&size, 4, 1, file);

    dungeon->version = be32toh(dungeon->version);
    size = be32toh(size);

    // Dungeon Hardness, Bytes 14 - 1693
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            fread(&dungeon->hardness[y][x], 1, 1, file);
        }
    }

    dungeon->num_rooms = (size - 1694) / 4;

    // Location of All Rooms, Bytes 1694 - eof
    for(int i = 0; i < dungeon->num_rooms; i++) {
        for(int j = 0; j < 4; j++) {
            fread(&dungeon->rooms[i][j], 1, 1, file);
        }
    }
    printf("Number of rooms: %d\n", dungeon->num_rooms);

    setMapFromFile(dungeon);
}

static void dijkstra_corridor(dungeon_t *d, pair_t from, pair_t to)
{
  static corridor_path_t path[ROWS][COLS], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < ROWS; y++) {
      for (x = 0; x < COLS; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < ROWS; y++) {
    for (x = 0; x < COLS; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[from[dim_y]][from[dim_x]].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < ROWS; y++) {
    for (x = 0; x < COLS; x++) {
      if (mapxy(x, y) != '|' || mapxy(x, y) != '-') {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = heap_remove_min(&h))) {
    p->hn = NULL;

    if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
      for (x = to[dim_x], y = to[dim_y];
           (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
        if (mapxy(x, y) != '.') {
          mapxy(x, y) = '#';
          hardnessxy(x, y) = 0;
        }
      }
      heap_delete(&h);
      return;
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
  }
}

static void placePC(dungeon_t *d, int coordsGiven, int x, int y)
{
    if(coordsGiven && (x < COLS && y < ROWS)) {
        d->map[y][x] = '@';
    }
    else {
        int randRoom = generateRandom(d->num_rooms, 0);
        int yMin = d->rooms[randRoom][0];
        int yMax = yMin + d->rooms[randRoom][2];
        int xMin = d->rooms[randRoom][1];
        int xMax = xMin + d->rooms[randRoom][3];

        int yPos = generateRandom(yMax, yMin);
        int xPos = generateRandom(xMax, xMin);
        d->map[yPos][xPos] = '@';
    }
}

static void generateTunnelingMapToPC(dungeon_t *d) 
{

}

int main(int argc, char* argv[])
{
    dungeon_t dungeon;
    char *home = getenv("HOME");
    char *filePath = strcat(home, "/.rlg327/");
    FILE *file;

    if(argc > 1 && argc < 3) {
        strcat(filePath, "rlg327");
        if(!strcmp(argv[1], "--save")) {
            file = fopen(filePath, "w");
            createEmptyMap(&dungeon);
            generateRooms(&dungeon);
            generateCorridors(&dungeon);
            writeToFile(&dungeon, file);
            fclose(file);
        }
        else if(!strcmp(argv[1], "--load")) {
            file = fopen(filePath, "r");
            loadFromFile(&dungeon, file);
            printMap(&dungeon);
            fclose(file);
        }
    }
    else if(argc == 3) {
        if((!strcmp(argv[1], "--save") && !strcmp(argv[2], "--load"))
            || (!strcmp(argv[1], "--load") && !strcmp(argv[2], "--save"))) {
                strcat(filePath, "rlg327");
                printf("File path: %s\n", filePath);
                file = fopen(filePath, "r+");

                loadFromFile(&dungeon, file);
                printMap(&dungeon);
                writeToFile(&dungeon, file);
                fclose(file);
        }
        else if(!strcmp(argv[1], "--load")) {
            strcat(filePath, argv[2]);
            if((file = fopen(filePath, "r")) != NULL) {
                loadFromFile(&dungeon, file);
                printMap(&dungeon);
                fclose(file);
            }
            else {
                printf("Please enter a valid file name\n");
                return 0;
            }
        }
    }
    else {
        createEmptyMap(&dungeon);
        generateRooms(&dungeon);
        generateCorridors(&dungeon);
        printMap(&dungeon);
    }

    return 0;
}
