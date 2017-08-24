#ifndef PATH_H
# define PATH_H

typedef struct dungeon dungeon_t;

void dijkstra(dungeon_t *d);
void dijkstra_tunnel(dungeon_t *d);

#endif
