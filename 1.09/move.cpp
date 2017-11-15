#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"
#include "io.h"
#include "npc.h"

void do_combat(dungeon_t *d, character *atk, character *def)
{
  int can_see_atk, can_see_def;
  const char *organs[] = {
    "liver",
    "pancreas",
    "heart",
    "brain",
    "eye",
    "arm",
    "leg",
    "intestines",
    "gall bladder",
    "lungs",
    "hand",
    "foot",
    "spinal cord",
    "pituitary gland",
    "thyroid",
    "tongue",
    "bladder",
    "diaphram",
    "frontal lobe",
    "hippocampus",
    "stomach",
    "pharynx",
    "esophagus",
    "trachea",
    "urethra",
    "spleen",
    "cerebellum",
    "ganglia",
    "ear",
    "subcutaneous tissue"
  };
  
  if (def->alive) {
    def->alive = 0;
    charpair(def->position) = NULL;
    
    if (def != d->PC) {
      d->num_monsters--;
    } else {
      io_queue_message("As %s%s eats your %s,", is_unique(atk) ? "" : "the ",
                       atk->name, organs[rand() % (sizeof (organs) /
                                                   sizeof (organs[0]))]);
      io_queue_message("   ...you wonder if there is an afterlife.");
      /* Queue an empty message, otherwise the game will not pause for *
       * player to see above.                                          */
      io_queue_message("");
    }
    atk->kills[kill_direct]++;
    atk->kills[kill_avenged] += (def->kills[kill_direct] +
                                  def->kills[kill_avenged]);
  }

  if (atk == d->PC) {
    io_queue_message("You smite %s%s!", is_unique(def) ? "" : "the ", def->name);
  }

  can_see_atk = can_see(d, character_get_pos(d->PC),
                        character_get_pos(atk), 1, 0);
  can_see_def = can_see(d, character_get_pos(d->PC),
                        character_get_pos(def), 1, 0);

  if (atk != d->PC && def != d->PC) {
    if (can_see_atk && !can_see_def) {
      io_queue_message("%s%s callously murders some poor, "
                       "defenseless creature.",
                       is_unique(atk) ? "" : "The ", atk->name);
    }
    if (can_see_def && !can_see_atk) {
      io_queue_message("Something kills %s%s.",
                       is_unique(def) ? "" : "the helpless ", def->name);
    }
    if (can_see_atk && can_see_def) {
      io_queue_message("You watch in abject horror as %s%s "
                       "gruesomely murders %s%s!",
                       is_unique(atk) ? "" : "the ", atk->name,
                       is_unique(def) ? "" : "the ", def->name);
    }
  }
}

void move_character(dungeon_t *d, character *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x]))) {
    do_combat(d, c, charpair(next));
  } else {
    /* No character in new position. */

    d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character_map[c->position[dim_y]][c->position[dim_x]] = c;
  }

  if (c == d->PC) {
    pc_reset_visibility(d->PC);
    pc_observe_terrain(d->PC, d);
  }
}

void do_moves(dungeon_t *d)
{
  pair_t next;
  character *c;
  event_t *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */

  if (pc_is_alive(d)) {
    /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
    e = (event_t *) malloc(sizeof (*e));
    e->type = event_character_turn;
    /* The next line is buggy.  Monsters get first turn before PC.  *
     * Monster gen code always leaves PC in a monster-free room, so *
     * not a big issue, but it needs a better solution.             */
    e->time = d->time + (1000 / d->PC->speed)
;
    e->sequence = 0;
    e->c = d->PC;
    heap_insert(&d->events, e);
  }

  while (pc_is_alive(d) &&
         (e = (event_t *) heap_remove_min(&d->events)) &&
         ((e->type != event_character_turn) || (e->c != d->PC))) {
    d->time = e->time;
    if (e->type == event_character_turn) {
      c = e->c;
    }
    if (!c->alive) {
      if (d->character_map[c->position[dim_y]][c->position[dim_x]] == c) {
        d->character_map[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != d->PC) {
        event_delete(e);
      }
      continue;
    }

    npc_next_pos(d, (npc *) c, next);
    move_character(d, c, next);

    heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
  }

  io_display(d);
  if (pc_is_alive(d) && e->c == d->PC) {
    c = e->c;
    d->time = e->time;
    /* Kind of kludgey, but because the PC is never in the queue when   *
     * we are outside of this function, the PC event has to get deleted *
     * and recreated every time we leave and re-enter this function.    */
    e->c = NULL;
    event_delete(e);
    io_handle_input(d);
  }
}

void dir_nearest_wall(dungeon_t *d, character *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2) {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2) {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t against_wall(dungeon_t *d, character *c)
{
  return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon_t *d, character *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

static void new_dungeon_level(dungeon_t *d, uint32_t dir)
{
  /* Eventually up and down will be independantly meaningful. *
   * For now, simply generate a new dungeon.                  */

  switch (dir) {
  case '<':
  case '>':
    new_dungeon(d);
    break;
  default:
    break;
  }
}


uint32_t move_pc(dungeon_t *d, uint32_t dir)
{
  pair_t next;
  uint32_t was_stairs = 0;

  next[dim_y] = d->PC->position[dim_y];
  next[dim_x] = d->PC->position[dim_x];


  switch (dir) {
  case 1:
  case 2:
  case 3:
    next[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    next[dim_y]--;
    break;
  }
  switch (dir) {
  case 1:
  case 4:
  case 7:
    next[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    next[dim_x]++;
    break;
  case '<':
    if (mappair(d->PC->position) == ter_stairs_up) {
      was_stairs = 1;
      new_dungeon_level(d, '<');
    }
    break;
  case '>':
    if (mappair(d->PC->position) == ter_stairs_down) {
      was_stairs = 1;
      new_dungeon_level(d, '>');
    }
    break;
  }

  if (was_stairs) {
    return 0;
  }

  if ((dir != '>') && (dir != '<') && (mappair(next) >= ter_floor)) {
    move_character(d, d->PC, next);
    dijkstra(d);
    dijkstra_tunnel(d);

    return 0;
  }

  return 1;
}
