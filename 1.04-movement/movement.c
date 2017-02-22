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

dungeon_t *d;

static int32_t tick_cmp(const void *key, const void *with) {
  return ((int32_t) d->npc[((npc_t *) key)->position[dim_y]]
                                        [((npc_t *) key)->position[dim_x]].tick -
          (int32_t) d->npc[((npc_t *) with)->position[dim_y]]
                                        [((npc_t *) with)->position[dim_x]].tick);
}

void npc_define(dungeon_t *dungeon, int numNPC) {
    d = dungeon;
    int i = 0, x = 0, y = 0;
    heap_t h;
    //srand(time(NULL));
    heap_init(&h, tick_cmp, NULL);


    // define PC position within the NPC array
    printf("Placing character... "); fflush(stdout);
    x = d->pc.position[dim_x];
    y = d->pc.position[dim_y];
    d->npc[y][x].position[dim_y] = y;
    d->npc[y][x].position[dim_x] = x;
    d->npc[y][x].type = PLAYER;
    d->npc[y][x].speed = 10;
    d->npc[y][x].tick = 1000 / 10;
    d->npc[y][x].alive = 1;
    heap_insert(&h, &d->npc[y][x]);
    printf("done\n");

    int counter = 0;
    printf("Placing monsters... "); fflush(stdout);
    for(i = 0; i < numNPC; i++) {

        x = rand() % DUNGEON_X;
        y = rand() % DUNGEON_Y;

        // define NPC locations and monster types
        if(d->map[y][x] == ter_floor_room) {
          counter++;
          // define NPC abilities
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | INTELLIGENT : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | TELEPATHIC : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | TUNNELING : d->npc[y][x].type;
          d->npc[y][x].type = rand() % 2 ? d->npc[y][x].type | ERRATIC : d->npc[y][x].type;

          // define NPC locations and movement speed
          d->npc[y][x].position[dim_y] = y;
          d->npc[y][x].position[dim_x] = x;
          d->npc[y][x].speed = (rand() % (20-5)) + 5;
          d->npc[y][x].tick = 1000 / d->npc[y][x].speed;
          d->npc[y][x].alive = 1;
          //heap_insert(&h, &d->npc[y][x]);
        }
        else i--;
    }
    printf("done\n");
    //printf("%d\n", counter);

    // for(int i = 0; i < DUNGEON_Y; i++){
    //   for(int j = 0; j < DUNGEON_X; j++) {
    //     if(d->npc[i][j].type != NULL && d->npc[i][j].type != PLAYER)
    //       printf(" %01x ", d->npc[i][j].type);
    //
    //     else if(d->npc[i][j].type == PLAYER)
    //       printf("@");
    //
    //     else if(d->map[y][x] == ter_floor_room)
    //       printf(".");
    //
    //     else if(d->map[y][x] == ter_floor_hall)
    //       printf("#");
    //
    //     else printf(" ");
    //
    //   }
    //   printf("\n");
    // }
}
