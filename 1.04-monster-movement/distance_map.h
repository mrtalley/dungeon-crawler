#ifndef DISTANCE_MAP_H
#define DISTANCE_MAP_H

typedef struct dungeon dungeon_t;

void create_distance_map(dungeon_t *d);
void create_tunnel_distance_map(dungeon_t *d);
void print_distance_map(dungeon_t *d, int tunneling);

#endif
