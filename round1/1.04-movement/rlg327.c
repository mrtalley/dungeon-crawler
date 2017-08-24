#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "dungeon.h"
#include "path.h"
// #include "movement.h"

dungeon_t d;
heap_t h;

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n]--nummon [<monster count>]\n",
          name);

  exit(-1);
}

static int32_t tick_cmp(const void *key, const void *with) {
  return ((int32_t) d.npc[((pc_t *) key)->position[dim_y]][((pc_t *) key)->position[dim_x]].tick -
  (int32_t) d.npc[((pc_t *) with)->position[dim_y]][((pc_t *) with)->position[dim_x]].tick);
}

void npc_define(int numNPC) {
    int i = 0, x = 0, y = 0;
    //srand(time(NULL));
    //heap_init(&h, tick_cmp, NULL);


    // define PC position within the NPC array
    printf("Placing character... "); fflush(stdout);
    x = d.pc.position[dim_x];
    y = d.pc.position[dim_y];
    d.npc[y][x].position[dim_y] = y;
    d.npc[y][x].position[dim_x] = x;
    d.npc[y][x].type = PLAYER;
    d.npc[y][x].speed = 10;
    d.npc[y][x].tick = 1000 / 10;
    d.npc[y][x].alive = 1;
    heap_insert(&h, &d.npc[y][x]);
    printf("done\n");

    printf("Placing monsters... "); fflush(stdout);
    for(i = 0; i < numNPC; i++) {

        x = rand() % DUNGEON_X;
        y = rand() % DUNGEON_Y;

        // define NPC locations and monster types
        if(d.map[y][x] == ter_floor_room) {
          // define NPC abilities
          d.npc[y][x].type = rand() % 2 ? d.npc[y][x].type | INTELLIGENT : d.npc[y][x].type;
          d.npc[y][x].type = rand() % 2 ? d.npc[y][x].type | TELEPATHIC : d.npc[y][x].type;
          d.npc[y][x].type = rand() % 2 ? d.npc[y][x].type | TUNNELING : d.npc[y][x].type;
          d.npc[y][x].type = rand() % 2 ? d.npc[y][x].type | ERRATIC : d.npc[y][x].type;

          // define NPC locations and movement speed
          d.npc[y][x].position[dim_y] = y;
          d.npc[y][x].position[dim_x] = x;
          d.npc[y][x].speed = (rand() % (20-5)) + 5;
          d.npc[y][x].tick = 1000 / d.npc[y][x].speed;
          d.npc[y][x].alive = 1;
          heap_insert(&h, &d.npc[y][x]);
        }
        else i--;
    }

    //print_heap(&h);
    printf("done\n");
}

 unsigned long char_movement(unsigned long ticks) {
   static pc_t *mob;
   static pc_t *character;

   //printf("about to peek\n");
   //printf("peeked\n");

   if(mob == NULL) {
     printf("CHECK IS NULL\n");
     return 0;
   }



     mob = heap_peek_min(&h);


     while(mob != NULL) {
     //printf("%lu\n", ticks);
         if(mob->tick <= ticks) {
           heap_remove_min(&h);
           mob->previousPos[dim_y] = mob->position[dim_y];
           mob->previousPos[dim_x] = mob->position[dim_x];
           mob->position[dim_y]++;
           mob->position[dim_x]++;
           mob->tick = ticks + 1000/mob->speed;

           // move NPC into new position
           d.npc[mob->position[dim_y]][mob->position[dim_x]] = *mob;
           // set previous NPC position to NULL
           d.npc[mob->previousPos[dim_y]][mob->previousPos[dim_x]].alive = 0;
           // set mob equal to new NPC position to be added back to heap
           *mob = d.npc[mob->position[dim_y]][mob->position[dim_x]];

           heap_insert(&h, mob);
         }

//     check = heap_peek_min(&h);
     else if(mob->type == PLAYER && mob->tick <= ticks) {
             character = heap_remove_min(&h);
             character->previousPos[dim_y] = character->position[dim_y];
             character->previousPos[dim_x] = character->position[dim_x];

             int xDir = rand() % 2 ? 1 : -1;
             int yDir = rand() % 2 ? 1 : -1;
             while(d.map[character->position[dim_y] + yDir][character->position[dim_x] + xDir] != ter_floor) {
                 xDir = rand() % 2 ? 1 : -1;
                 yDir = rand() % 2 ? 1 : -1;
             }

             if(d.map[character->position[dim_y] + yDir][character->position[dim_x] + xDir] == ter_floor) {
                 character->position[dim_y] += yDir;
                 character->position[dim_x] += xDir;


                 character->tick = ticks + 1000 / character->speed;

                 // move PC into new position
                 d.npc[character->position[dim_y]][character->position[dim_x]] = *character;
                 // set previous PC position to NULL
                 d.npc[character->previousPos[dim_y]][character->previousPos[dim_x]].alive = 0;
                 // set PC equal to new NPC position to be added back to heap
                 *character = d.npc[character->position[dim_y]][character->position[dim_x]];
                 d.pc.position[dim_y] = character->position[dim_y];
                 d.pc.position[dim_x] = character->position[dim_x];

                 printf("Player has moved");
                 if (character->alive == 1) {
                     heap_insert(&h, character);
                 }

                 render_dungeon(&d);
                 printf("%ld\n", ticks);
                 break;
             }

         }

       else {
         mob = heap_peek_min(&h);
       }
     //ticks++;
   }


   ticks++;
   return ticks;
 }

int main(int argc, char *argv[])
{
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_place_pc, do_place_monsters;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;
  int NPCs;
  unsigned long ticks = 0;

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_place_pc = do_place_monsters = 0;
  do_seed = 1;
  save_file = load_file = NULL;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */

 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            save_file = argv[++i];
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        case 'p':
          if ((!long_arg && argv[i][2])            ||
              (long_arg && strcmp(argv[i], "-pc")) ||
              argc <= i + 2                        ||
              argv[i + 1][0] == '-'                ||
              argv[i + 2][0] == '-') {
            usage(argv[0]);
          }
          do_place_pc = 1;
          if ((d.pc.position[dim_y] = atoi(argv[++i])) < 1 ||
              d.pc.position[dim_y] > DUNGEON_Y - 2         ||
              (d.pc.position[dim_x] = atoi(argv[++i])) < 1 ||
              d.pc.position[dim_x] > DUNGEON_X - 2) {
            fprintf(stderr, "Invalid PC position.\n");
            usage(argv[0]);
          }
          break;
        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon"))||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%d", &NPCs)) {
            usage(argv[0]);
          }
          do_place_monsters = 1;
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  if (!do_place_pc) {
    i = rand() % d.num_rooms;
    d.pc.position[dim_x] = (d.rooms[i].position[dim_x] + (rand() % d.rooms[i].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[i].position[dim_y] + (rand() % d.rooms[i].size[dim_y]));
  }

  printf("PC is at (y, x): %d, %d\n", d.pc.position[dim_y], d.pc.position[dim_x]);
  d.pc.alive = 1;


  dijkstra(&d);
  dijkstra_tunnel(&d);
  // render_distance_map(&d);
  // render_tunnel_distance_map(&d);

  heap_init(&h, tick_cmp, NULL);

  if(do_place_monsters) {
    npc_define(NPCs);
      render_dungeon(&d);
    while(d.pc.alive == 1) {
      //render_dungeon(&d);
      usleep(20000);
      ticks = char_movement(ticks);
    }

      // debug
    pc_t *store = heap_peek_min(&h);
    while(store != NULL) {
        heap_remove_min(&h);
        if(store->type == PLAYER) printf("PLAYER, %d\n", store->tick);
        else printf("%x, %d\n",store->type, store->tick);
        store = heap_peek_min(&h);
    }
  }



  if (do_save) {
    write_dungeon(&d, save_file);
  }

  delete_dungeon(&d);

  return 0;
}
