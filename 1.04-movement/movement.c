#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#include "dungeon.h"
#include "path.h"
#include "movement.h"
#include "heap.h"

dungeon_t *d;

static int32_t tick_cmp(const void *key, const void *with) {
  return ((int32_t) d->npc[((pc_t *) key)->position[dim_y]]
                                        [((pc_t *) key)->position[dim_x]].tick -
          (int32_t) d->npc[((pc_t *) with)->position[dim_y]]
                                        [((pc_t *) with)->position[dim_x]].tick);
}

void npc_define(dungeon_t *dungeon, int numNPC, heap_t h) {
    d = dungeon;
    int i = 0, x = 0, y = 0;
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
          d->npc[y][x].tick = 1000 / d->npc[y][x].speed;
          d->npc[y][x].alive = 1;
          heap_insert(&h, &d->npc[y][x]);
        }
        else i--;
    }
    printf("done\n");
}

 void char_movement(dungeon_t *d, heap_t h, unsigned long ticks) {
   static pc_t *check;
   static pc_t *mob;
   static pc_t *character;

   check = heap_peek_min(&h);

   while(check->type != PLAYER) {
     if(check->tick > ticks) {
       mob = heap_remove_min(&h);
       mob->position[dim_y]++;
       mob->position[dim_x]++;
       mob->tick = ticks + 1000/mob->speed;
       heap_insert(&h, mob);
       check = heap_peek_min(&h);
     }

     if(check->type == PLAYER && check->tick > ticks) {
       character = heap_remove_min(&h);
       character->position[dim_y]++;
       character->position[dim_x]++;
       character->tick = ticks + 1000/character->speed;
       printf("Player has moved");
       render_dungeon(&d);
       printf("%ld\n", ticks);
     }
   }


 }
