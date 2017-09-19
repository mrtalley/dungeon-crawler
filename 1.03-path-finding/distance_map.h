#ifndef DISTANCE_MAP_H
#define DISTANCE_MAP_H

typedef struct dungeon dungeon_t;

void create_distance_map(dungeon_t *d, int tunneling);
void print_distance_map(dungeon_t *d, int tunneling);

#endif
