#ifndef PATH_H
# define PATH_H

# define HARDNESS_PER_TURN 85

typedef class dungeon dungeon_t;

void dijkstra(dungeon_t *d);
void dijkstra_tunnel(dungeon_t *d);

#endif
