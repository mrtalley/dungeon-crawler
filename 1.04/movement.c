#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#include "dungeon.h"
#include "path.h"
#include "movement.h"
#include "heap.h"

#define INTELLIGENT 0b00000001
#define TELEPATHIC  0b00000010
#define TUNNELING   0b00000100
#define ERRATIC     0b00001000
#define PLAYER      0b00010000

void npc_define(dungeon_t *d, int numNPC) {
    int i = 0, x = 0, y = 0;
    heap_t h;
    //srand(time(NULL));
    heap_init(&h, 1000 / d->npc[y][x].speed, NULL);


    // define PC position within the NPC array
    printf("Placing character... "); fflush(stdout);
    x = d->pc.position[dim_x];
    y = d->pc.position[dim_y];
    d->npc[y][x].position[dim_y] = y;
    d->npc[y][x].position[dim_x] = x;
    d->npc[y][x].type = PLAYER;
    d->npc[y][x].speed = 10;
    d->npc[y][x].alive = 1;
    heap_insert(&h, &d->npc[y][x]);
    printf("done\n");

    printf("Placing monsters... "); fflush(stdout);
    for(i = 0; i < numNPC; i++) {
        x = rand() % DUNGEON_X;
        y = rand() % DUNGEON_Y;

        // define NPC locations and monster types
        if(d->map[y][x] == ter_floor_room) {
          // define NPC abilities
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | INTELLIGENT : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | TELEPATHIC : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | TUNNELING : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | ERRATIC : d->npc[y][x].type;

          // define NPC locations and movement speed
          d->npc[y][x].position[dim_y] = y;
          d->npc[y][x].position[dim_x] = x;
          d->npc[y][x].speed = (rand() % (20-5)) + 5;
          d->npc[y][x].alive = 1;
          heap_insert(&h, &d->npc[y][x]);
        }
        else i--;
    }
    printf("done\n");
}
