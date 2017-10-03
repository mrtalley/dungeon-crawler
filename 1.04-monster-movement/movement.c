#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "dungeon.h"

int genMonsterCode()
{
    int monster = 0000;
    int rando = 0;

    // Intelligent monsters
    rando = rand() % 2;
    if(rando) {
        monster += 1;
    }

    // Telepathic monsters
    rando = rand() % 2;
    if(rando) {
        monster += 10;
    }

    // Tunneling monsters
    rando = rand() % 2;
    if(rando) {
        monster += 100;
    }

    // // Erratic monsters
    rando = rand() % 2;
    if(rando) {
        monster += 1000;
    }

    return monster;
}

char *intToArray(int intr)
{
    char *array = malloc(4 * sizeof(*array));
    int i = 0;

    do {
        array[i] = intr % 10;
        intr /= 10;
        i++;
    } while(intr != 0);

    return array;
}

int determineMonsterTraits(int monsterCode)
{
    char *code = intToArray(monsterCode);
    int hex = 0;;

    // Intelligent
    if(code[0]) {
        hex += 1;
    }

    // Telepathic
    if(code[1]) {
        hex += 2;
    }

    // Tunneling
    if(code[2]) {
        hex += 4;
    }

    // Erratic
    if(code[3]) {
        hex += 8;
    }

    return hex;
}

void intelligence(monster_t *monster, uint8_t dist_map[ROWS][COLS])
{
    int y = monster->position[dim_y], x = monster->position[dim_x];
    int i = 0, j = 0, min = 255;
    int newY = 0, newX = 0;

    if(monster->lineOfSight) {
        for(j = y - 1; j < y + 1; j++) {
            for(i = x - 1; i < x + 1; i++) {
                if(dist_map[j][i] < min) {
                    min = dist_map[j][i];
                    newY = j, newX = i;
                }
            }
        }
    }
}

void telepathic(monster_t *monster, uint8_t dist_map[ROWS][COLS])
{
    monster->lineOfSight = 1;
}
