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

// FIXME
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

// FIXME
void telepathic(monster_t *monster, uint8_t dist_map[ROWS][COLS])
{
    monster->lineOfSight = 1;
}

void movePC(dungeon_t *d)
{
    srand(time(NULL));
    int randPosY = 0, randPosX = 0, set = 0;
    int y, x;

    while (!set) {
        randPosY = generateRandom(1, -1);
        randPosX = generateRandom(1, -1);
        y = d->pc.position[dim_y] + randPosY;
        x = d->pc.position[dim_x] + randPosX;

        if(d->hardness[y][x] == 0) {
            set = 1;
            d->pc.position[dim_y] = y;
            d->pc.position[dim_x] = x;
        }
    }
}

void moveOnGradient(dungeon_t *d, pc_t *c)
{
    int y = c->position[dim_y];
    int x = c->position[dim_x];
    int newY = 0, newX = 0, j = 0, i = 0;
    int min = 255;

    for(j = y - 1; j < y + 1; j++) {
        for(i = x - 1; i < x + 1; i++) {
            if(d->distance_to_pc[j][i] < min) {
                min = d->distance_to_pc[j][i];
                newY = j, newX = i;
            }
        }
    }

    c->position[dim_y] = newY;
    c->position[dim_x] = newX;
}

void determineCharacterType(pc_t *c)
{

}

void moveCharacter(dungeon_t *d, heap_t *h)
{
    /*  Model the dijkstra corridor code again
     *  I believe we need to use the heap -- except
     *  instead of just removing the item from the heap,
     *  you'll add it back in again at the bottom.
     *
     *  Need to add both monster_t and pc_t types to the
     *  heap...need to come up with a solution to this.
     */

    // int i = 0;

    if(d->pc.alive) {
        heap_insert(h, &d->npc[d->pc.position[dim_y]][d->pc.position[dim_x]]);
    }
    // insert pc
    // heap_insert(&h, ) // look into this
    //
    // // insert monsters
    // for(i = 0; i < d->num_monsters; i++) {
    //     heap_insert(&h, ) // look into this
    // }
    pc_t *character;
    while(d->pc.alive && (character = heap_remove_min(h)) && (character != &d->pc)) {
        if(character->alive) {
            determineCharacterType(character);
            // d->npc[character->position[dim_y]][character->position[dim_x]] = NULL;
            d->npc[character->position[dim_y]][character->position[dim_x]] = *character;
            heap_insert(h, character);
        }
    }

    if(d->pc.alive && character == &d->pc) {
        character = NULL;
        movePC(d);
    }
}
