#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//#include <endian.h>

#include "dungeon.h"
#include "distance_map.h"
#include "endian.h"
#include "movement.h"

int generateRandom(int max, int min) {
    return (rand() % (max - min)) + min;
}

// test this
// monster_t getMonsterByCoordinates(dungeon_t *d, int y, int x)
// {
//     int i = 0;
//     monster_t monst;
//     for(i = 0; i < d->num_monsters; i++) {
//         if(d->monsters[i].position[dim_y] == y && d->monsters[i].position[dim_x] == x) {
//             monst = d->monsters[i];
//         }
//     }
//     return monst;
// }

static void createEmptyMap(dungeon_t *dungeon) {
    srand((unsigned) time(NULL));
    int y = 0, x = 0;
    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            // dungeon->npc[y][x]->alive = 0;
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

static int roomCheck(int xPos, int yPos, int xSize, int ySize, dungeon_t *dungeon) {
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

// FIXME
static void printMap(dungeon_t *d) {
    int y = 0, x = 0;

    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            if(d->npc[y][x].alive) {
                printf("%x", determineMonsterTraits(d->npc[y][x].type));
            }
            else {
               printf("%c", d->map[y][x]);
           }
        }
        printf("\n");
    }
}

void setRoomsFromFile(dungeon_t *d) {
    int i = 0, x = 0, y = 0;

    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            mapxy(x,y) = ' ';
        }
    }

    for(i = 0; i < d->num_rooms; i++) {
        for(y = d->rooms[i][0]; y < d->rooms[i][0] + d->rooms[i][2]; y++) {
            for(x = d->rooms[i][1]; x < d->rooms[i][1] + d->rooms[i][3]; x++) {
                mapxy(x,y) = '.';
            }
        }
    }
}

void setMapFromFile(dungeon_t *d) {
    setRoomsFromFile(d);
    int y = 0, x = 0;

    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            if(mapxy(x,y) == '.') {
                continue;
            }
            if(y == 0 || y == ROWS - 1) {
                mapxy(x,y) = '-';
            }
            else if(x == 0 || x == COLS - 1) {
                mapxy(x,y) = '|';
            }
            else if(d->hardness[y][x] == 0) {
                mapxy(x,y) = '#';
            }
            else {
                mapxy(x,y) = ' ';
            }
        }
    }
}

static void generateCorridors(dungeon_t *dungeon) {
    int count = 1;
    int secondX, secondY, firstX, firstY;

    printf("Generating corridors...");

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

    printf("done\n");
}

void writeToFile(dungeon_t *dungeon, char *filePath) {
    FILE *file = fopen(filePath, "w");
    dungeon->version = 0;
    uint32_t size = 0;

    // Name of File, 4 bytes, Bytes 0 - 5
    fwrite("RLG327", 1, 6, file);

    // File Version Number, Bytes 6 - 9
    dungeon->version = htobe32(dungeon->version);
    fwrite(&dungeon->version, 4, 1, file);

    // Size of File, 4 bytes, Bytes 10 - 13
    printf("NUM_ROOMS: %d\n", dungeon->num_rooms);
    size = 1694 + dungeon->num_rooms * 4;
    size = htobe32(size);
    fwrite(&size, 4, 1, file);

    // Dungeon Hardness, 1680 bytes, Bytes 14 - 1693
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
    fclose(file);
}

void loadFromFile(dungeon_t *dungeon, char *filePath) {
    FILE *file = fopen(filePath, "r");
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
    fclose(file);
}

void placeRandom(dungeon_t *d, int *y, int *x)
{
    int set = 0;
    while(!set) {
        *y = generateRandom(ROWS, 0);
        *x = generateRandom(COLS, 0);

        if(d->map[*y][*x] == '.') {
            set = 1;
        }
    }
}

void placeMonster(dungeon_t *d, int *y, int *x)
{

    placeRandom(d, y, x);
    printf("monsterY: %d, monsterX: %d\n", *y, *x);
    d->npc[*y][*x].position[dim_y] = *y;
    d->npc[*y][*x].position[dim_x] = *x;
    d->npc[*y][*x].alive = 1;
    d->npc[*y][*x].speed = generateRandom(20, 5);
    // d->map[y][x] = 'M';
}

void createMonsters(dungeon_t *d, heap_t *h)
{
    printf("Placing monsters..."); fflush(stdout);
    d->num_monsters = generateRandom(MAX_MONSTERS, MIN_MONSTERS);
    printf("Number of monsters: %d\n", d->num_monsters);
    int i = 0;
    int y = 0, x = 0;
    for(i = 0; i < d->num_monsters; i++) {
        placeMonster(d, &y, &x);
        d->npc[y][x].type = genMonsterCode();
        heap_insert(h, &d->npc[y][x]);
    }
    printf("done. Number of monsters: %d\n", d->num_monsters);
}

void initializePC(dungeon_t *d, int pcY, int pcX)
{
    d->npc[pcY][pcX].type = PC;
    d->npc[pcY][pcX].speed = 10;
    d->npc[pcY][pcX].queueNum = 100;
    d->npc[pcY][pcX].alive = 1;
    d->npc[pcY][pcX].position[dim_y] = pcY;
    d->npc[pcY][pcX].position[dim_x] = pcX;
    // d->pc.position[dim_y] = pcY;
    // d->pc.position[dim_x] = pcX;

    d->map[pcY][pcX] = '@';
    d->pc = d->npc[pcY][pcX];
}

// might not be correct
// static int32_t compare_queueNum(const void *firstNPC, const void *secondNPC) {
//     return (((pc_t *) firstNPC)->queueNum - ((pc_t *) secondNPC)->queueNum);
// }

int main(int argc, char* argv[]) {
    dungeon_t dungeon;
    heap_t h;
    char *saveFlag = "--save";
    int save = 0;
    char *loadFlag = "--load";
    int load = 0;
    char *customFilePath = NULL;
    char *setPCFlag = "--p";
    int setPC = 0;
    int pcY = 0, pcX = 0;
    int i = 0;
    char *home = getenv("HOME");
    char *filePath = strcat(home, "/.rlg327/");

    for(i = 0; i < argc; i++) {
        if(!strcmp(argv[i], saveFlag)) {
            save = 1;
        }
        else if(!strcmp(argv[i], loadFlag)) {
            load = 1;
        }
        else if(strstr(argv[i], ".rlg327") != NULL) {
            customFilePath = malloc(sizeof(argv[i]));
            customFilePath = argv[i];
        }
        else if(!strcmp(argv[i], setPCFlag)) {
            setPC = 1; i++;
            pcY = atoi(argv[i]); i++;
            pcX = atoi(argv[i]);
            printf("SETPC x: %d, y: %d\n", pcX, pcY);
        }
    }

    if(load && (customFilePath != NULL)) {
        printf("Custom file set\n");
        strcat(filePath, customFilePath);
    }
    else {
        strcat(filePath, "rlg327");
    }

    if(load) {
        printf("Loading from file\n");
        loadFromFile(&dungeon, filePath);
    }

    if(save && load) {
        printf("Saving loaded map to file\n");
        writeToFile(&dungeon, filePath);
    }
    else if(save) {
        printf("Saving new map to file\n");
        createEmptyMap(&dungeon);
        generateRooms(&dungeon);
        generateCorridors(&dungeon);
        writeToFile(&dungeon, filePath);
    }

    if(!save && !load) {
        printf("Generating random map\n");
        createEmptyMap(&dungeon);
        generateRooms(&dungeon);
        generateCorridors(&dungeon);
    }

    if(setPC) {
        if((pcY < ROWS && pcY > 0) && (pcX < COLS && pcX > 0)) {
            printf("Setting PC @ x: %d, y: %d\n", pcX, pcY);
            initializePC(&dungeon, pcY, pcX);
        }
        else {
            printf("PC Coordinates are out of map bounds.\nSetting random.\n");
            setPC = 0;
        }
    }

    if(!setPC) {
        printf("Placing pc in random location...");
        placeRandom(&dungeon, &pcY, &pcX);
        initializePC(&dungeon, pcY, pcX);
        printf("done\n");
    }

    // dungeon.npc = *malloc(COLS * ROWS * sizeof(pc_t));

    if((!save) || (save && load)) {
        createMonsters(&dungeon, &h);
        printMap(&dungeon);
        create_distance_map(&dungeon);
        create_tunnel_distance_map(&dungeon);
    }

    // heap_t h;
    // heap_init(&h, compare_queueNum, NULL);

    // moveCharacter(&dungeon, h);

    return 0;
}
