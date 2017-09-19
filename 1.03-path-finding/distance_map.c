#include <stdint.h>
#include <limits.h>

#include "dungeon.h"
#include "distance_map.h"
#include "heap.h"

typedef struct path {
    heap_node_t *hn;
    uint8_t pos[2];
} distance_path_t;

// had issue with below function receiving a dungeon_t struct
// easiest workaround I found was to make a global variable
dungeon_t *dungeon;

// replacement for corridor_path_cmp
// cost comparison is replaced by distance comparison
static int32_t compare_distance(const void *key, const void *with) {
    return (dungeon->distance_to_pc[((distance_path_t *) key )->pos[dim_y]][((distance_path_t *) key)->pos[dim_x]] -
            dungeon->distance_to_pc[((distance_path_t *) with)->pos[dim_y]][((distance_path_t *) with)->pos[dim_x]]);
}

void create_distance_map(dungeon_t *d, int tunneling)
{
    static distance_path_t path[ROWS][COLS], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;

    // for compare_distance function
    dungeon = d;

    if (!initialized) {
        for (y = 0; y < ROWS; y++) {
            for (x = 0; x < COLS; x++) {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }

    for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLS; x++) {
            d->distance_to_pc[y][x] = 255;
        }
    }

    // set PC distance to 0 bc distance is calc'd to PC
    d->distance_to_pc[d->pc.position[dim_y]][d->pc.position[dim_x]] = 0;

    heap_init(&h, compare_distance, NULL);

    for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLS; x++) {
            // calc distance for only rooms and tunnels
            if (hardnessxy(x, y) == 0 && !tunneling) {
                path[y][x].hn = heap_insert(&h, &path[y][x]);
            }
            // calc distance for any that aren't the boundary
            else if(hardnessxy(x, y) != 255 && tunneling) {
                path[y][x].hn = heap_insert(&h, &path[y][x]);
            }
            else {
                path[y][x].hn = NULL;
            }
        }
    }

    while ((p = heap_remove_min(&h))) {
        p->hn = NULL;

        // same as dijkstra_corridor
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) && (d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x]] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x]] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) && (d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x] - 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x] - 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) && (d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x] + 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x] + 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] + 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) && (d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x]] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x]] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
        }

        // added to cover the corners
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x] - 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x] - 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x] + 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] - 1][p->pos[dim_x] + 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) && (d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x] - 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x] - 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) && (d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x] + 1] > d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1)) {
            d->distance_to_pc[p->pos[dim_y] + 1][p->pos[dim_x] + 1] = d->distance_to_pc[p->pos[dim_y]][p->pos[dim_x]] + 1;
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn);
        }
    }
    heap_delete(&h);
}

void print_distance_map(dungeon_t *d, int tunneling)
{
    int x = 0, y = 0;
    for(y = 0; y < ROWS; y++) {
        for(x = 0; x < COLS; x++) {
            if(mapxy(x, y) == '@') {
                printf("%c", '@');
            }
            else if(hardnessxy(x, y) != 0 && !tunneling) {
                printf("%c", ' ');
            }
            else if(hardnessxy(x, y) == 255) {
                printf("%c", ' ');
            }
            else {
                /* adding '0' turns int to char and % 10 prints *
                 * only the last character in the int           */
                printf("%c", '0' + d->distance_to_pc[y][x] % 10);
            }
        }
        printf("\n");
    }
}
