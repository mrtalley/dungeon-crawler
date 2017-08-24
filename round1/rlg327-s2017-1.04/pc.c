#include <stdlib.h>

#include "string.h"

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"

void pc_delete(pc_t *pc)
{
  if (pc) {
    free(pc);
  }
}

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->pc.alive;
}

void place_pc(dungeon_t *d)
{
  d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));
}

void config_pc(dungeon_t *d)
{
  memset(&d->pc, 0, sizeof (d->pc));
  d->pc.symbol = '@';

  place_pc(d);

  d->pc.speed = PC_SPEED;
  d->pc.alive = 1;
  d->pc.sequence_number = 0;
  d->pc.pc = calloc(1, sizeof (*d->pc.pc));
  d->pc.npc = NULL;
  d->pc.kills[kill_direct] = d->pc.kills[kill_avenged] = 0;

  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, &d->pc)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
  //   if (!against_wall(d, &d->pc) && ((rand() & 0x111) == 0x111)) {
  //     dir[dim_x] = (rand() % 3) - 1;
  //     dir[dim_y] = (rand() % 3) - 1;
  //   } else {
  //     dir_nearest_wall(d, &d->pc, dir);
  //   }
  // }else {
  //   /* And after we've been there, let's head toward the center of the map. */
  //   if (!against_wall(d, &d->pc) && ((rand() & 0x111) == 0x111)) {
  //     dir[dim_x] = (rand() % 3) - 1;
  //     dir[dim_y] = (rand() % 3) - 1;
  //   } else {
  //     dir[dim_x] = ((d->pc.position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
  //     dir[dim_y] = ((d->pc.position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
  //   }

    #define ESC 27

    char key;
    static int mode = ESC;
    int nextX = 0, nextY = 0;
    int pc_x = d->pc.position[dim_x], pc_y = d->pc.position[dim_y];

    do {
       key = getch();
    } while (key != 'k' && key != '8' && key != 'l' && key != '6' && key != 'j' && key != '2' && key != 'h' && key != '4' && key != 'y' && key != '7' && key != 'u' && key != '9' && key != 'n' && key != '3' && key != 'b' && key != '1' && key != 'Q' && key != ESC && key != '>' && key != '<' && key != 'L' && key != ' ' && key != 'w' && key != 'a' && key != 's' && key != 'd');

    // enter look mode
    if(key == 'L') {

    }

    // go down stairs
    else if(key == '>') {
      if(mapxy(pc_x, pc_y) == ter_floor_stair_down) {
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        config_pc(d);
        gen_monsters(d);
      }
    }

    // go up stairs
    else if(key == '<') {
      if(mapxy(pc_x, pc_y) == ter_floor_stair_up) {
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        config_pc(d);
        gen_monsters(d);
      }
    }

    // up
    else if((key == 'k' || key == '8' || key == 'w') && mode == ESC) {
      nextY -= 1;
    }
    // right
    else if((key == 'l' || key == '6' || key == 'd') && mode == ESC) {
      nextX += 1;
    }
    // down
    else if((key == 'j' || key == '2' || key == 's') && mode == ESC) {
      nextY += 1;
    }
    // left
    else if((key == 'h' || key == '4' || key == 'a') && mode == ESC) {
      nextX -= 1;
    }
    // upper left
    else if((key == 'y' || key == '7') && mode == ESC) {
      nextY -= 1;
      nextX -= 1;
    }
    // upper right
    else if((key == 'u' || key == '9') && mode == ESC) {
      nextY -= 1;
      nextX += 1;
    }
    // lower right
    else if((key == 'n' || key == '3') && mode == ESC) {
      nextY += 1;
      nextX += 1;
    }
    // lower left
    else if((key == 'b' || key == '1') && mode == ESC) {
      nextY += 1;
      nextX -= 1;
    }

    else if(key == 'Q') d->pc.alive = 0;

    if(mapxy(nextX + pc_x, nextY + pc_y) == ter_floor_hall || mapxy(nextX + pc_x, nextY + pc_y) == ter_floor_room || mapxy(nextX + pc_x, nextY + pc_y) == ter_floor_stair_up || mapxy(nextX + pc_x, nextY + pc_y) == ter_floor_stair_down) {
      dir[dim_y] += nextY;
      dir[dim_x] += nextX;
    }

  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->pc.position[dim_x] + dir[dim_x],
              d->pc.position[dim_y] + dir[dim_y]) &&
      ((charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

void look_mode_true(dungeon_t *d) {
  pair_t p;
  clear();
  int cam_y, cam_x, random;
  int y_pos = 1, x_pos = 0;
  cam_y = d->camera_pos[dim_y] = d->pc.position[dim_y];
  cam_x = d->camera_pos[dim_x] = d->pc.position[dim_x]
  srand((unsigned) NULL);

  // only prints the 80 x 21 window around the pc
  for (p[dim_y] = pc_y - 10; p[dim_y] < pc_y + 10; p[dim_y]++) {
    for (p[dim_x] = pc_x - 40; p[dim_x] < pc_x + 39; p[dim_x]++) {
      random = rand() % 2;

      if(p[dim_y] < 0 || p[dim_y] >= DUNGEON_Y || p[dim_x] < 0 || p[dim_x] >= DUNGEON_X) {
        mvaddch(y_pos, x_pos, ' ');
      }

      else if (charpair(p)) {
        mvaddch(y_pos, x_pos, charpair(p)->symbol);
      }

      else {
        switch (mappair(p)) {
        case ter_wall:
        case ter_wall_immutable:
          mvaddch(y_pos, x_pos, ' ');
          break;
        case ter_floor:
        case ter_floor_room:
          mvaddch(y_pos, x_pos, '.');
          break;
        case ter_floor_hall:
          mvaddch(y_pos, x_pos, '#');
          break;
        case ter_floor_stair_up:
          mvaddch(y_pos, x_pos, '<');
          break;
        case ter_floor_stair_down:
          mvaddch(y_pos, x_pos, '>');
          break;
        case ter_debug:
          mvaddch(y_pos, x_pos, '*');
          fprintf(stderr, "Debug character at %d, %d\n", p[dim_y], p[dim_x]);
          break;
        }
      }
      //mvaddch(y_pos,x_pos,'.');
      x_pos++;
    }
    y_pos++;
    x_pos = 0;
  }
  refresh();
}

uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->pc.position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->pc.position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->pc.position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->pc.position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}
