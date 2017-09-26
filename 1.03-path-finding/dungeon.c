#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//#include <endian.h>

#include "dungeon.h"
#include "distance_map.h"
#include "endian.h"

int generateRandom(int max, int min) {
    return (rand() % (max - min)) + min;
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

static void printMap(dungeon_t *dungeon) {
    int y = 0, x = 0;
    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            printf("%c", dungeon->map[y][x]);
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

int main(int argc, char* argv[]) {
    dungeon_t dungeon;
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
    FILE *file;

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
        file = fopen(filePath, "r+");
    }
    else {
        strcat(filePath, "rlg327");
        file = fopen(filePath, "r+");
    }

    if(load) {
        printf("Loading from file\n");
        loadFromFile(&dungeon, file);
    }

    if(save && load) {
        printf("Saving loaded map to file\n");
        writeToFile(&dungeon, file);
    }
    else if(save) {
        printf("Saving new map to file\n");
        createEmptyMap(&dungeon);
        generateRooms(&dungeon);
        generateCorridors(&dungeon);
        writeToFile(&dungeon, file);
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
            dungeon.pc.position[dim_y] = pcY;
            dungeon.pc.position[dim_x] = pcX;
            dungeon.map[pcY][pcX] = '@';
        }
        else {
            printf("PC Coordinates are out of map bounds.\nSetting random.\n");
            setPC = 0;
        }
    }

    if(!setPC) {
        int set = 0;
        while(!set) {
            pcY = generateRandom(ROWS, 0);
            pcX = generateRandom(COLS, 0);

            if(dungeon.map[pcY][pcX] == '.') {
                dungeon.pc.position[dim_y] = pcY;
                dungeon.pc.position[dim_x] = pcX;
                dungeon.map[pcY][pcX] = '@';
                set = 1;
            }
        }
    }

    if((!save) || (save && load)) {
        printMap(&dungeon);
        printf("\n\n");
        create_distance_map(&dungeon, 0);
        print_distance_map(&dungeon, 0);
        printf("\n\n");
        create_distance_map(&dungeon, 1);
        print_distance_map(&dungeon, 1);
    }

    fclose(file);

    return 0;
}
