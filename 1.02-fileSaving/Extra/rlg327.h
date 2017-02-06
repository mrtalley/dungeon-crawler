#ifndef RLG327_H
# define RLG327_H

#include <stdint.h>
#include "heap.h"

#define DUNGEON_X              160
#define DUNGEON_Y              105
#define MIN_ROOMS              25
#define MAX_ROOMS              40
#define ROOM_MIN_X             7
#define ROOM_MIN_Y             5
#define ROOM_MAX_X             20
#define ROOM_MAX_Y             15

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

/* Returns true if random float in [0,1] is less than *
 * numerator/denominator.  Uses only integer math.    */
# define rand_under(numerator, denominator) \
  (rand() < ((RAND_MAX / denominator) * numerator))

/* Returns random integer in [min, max]. */
# define rand_range(min, max) ((rand() % (((max) + 1) - (min))) + (min))

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

/*
  ENUM: a list of constant integer values
*/
typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_wall,
  ter_wall_immutable,
  ter_floor,
  ter_floor_room,
  ter_floor_hall,
} terrain_type_t; // this is probably the var to use for each of these

// declared later like an object
typedef struct room {
  pair_t position;
  pair_t size;
} room_t;

// declared later like an object
typedef struct dungeon {
  uint32_t num_rooms;
  room_t rooms[MAX_ROOMS];
  terrain_type_t map[DUNGEON_Y][DUNGEON_X];
  /* Since hardness is usually not used, it would be expensive to pull it *
   * into cache every time we need a map cell, so we store it in a        *
   * parallel array, rather than using a structure to represent the       *
   * cells.  We may want a cell structure later, but from a performanace  *
   * perspective, it would be a bad idea to ever have the map be part of  *
   * that structure.  Pathfinding will require efficient use of the map,  *
   * and pulling in unnecessary data with each map cell would add a lot   *
   * of overhead to the memory system.                                    */

   /* 8 bit unsigned int -- all bits dedicated
   to the number bc its only positive */
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
} dungeon_t;



static uint32_t in_room(dungeon_t *d, int16_t y, int16_t x);
static int32_t corridor_path_cmp(const void *key, const void *with);
static void dijkstra_corridor(dungeon_t *d, pair_t from, pair_t to);
static void dijkstra_corridor_inv(dungeon_t *d, pair_t from, pair_t to);
static int connect_two_rooms(dungeon_t *d, room_t *r1, room_t *r2);
static int create_cycle(dungeon_t *d);
static int connect_rooms(dungeon_t *d);
static int smooth_hardness(dungeon_t *d);
static int empty_dungeon(dungeon_t *d);
static int place_rooms(dungeon_t *d);
static int make_rooms(dungeon_t *d);
int gen_dungeon(dungeon_t *d);
void render_dungeon(dungeon_t *d);
void delete_dungeon(dungeon_t *d);
void init_dungeon(dungeon_t *d);


#endif
