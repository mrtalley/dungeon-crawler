#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>

# include "dims.h"

typedef class dungeon dungeon_t;
typedef class npc npc_t;
typedef class pc pc_t;
typedef class dice_t dice_t;

typedef enum kill_type {
  kill_direct,
  kill_avenged,
  num_kill_types
} kill_type_t;

class character {
  public:
    char symbol;
    pair_t position;
    int32_t speed;
    uint32_t alive;
    /* Characters use to have a next_turn for the move queue.  Now that it is *
     * an event queue, there's no need for that here.  Instead it's in the    *
     * event.  Similarly, sequence_number was introduced in order to ensure   *
     * that the queue remains stable.  Also no longer necessary here, but in  *
     * this case, we'll keep it, because it provides a bit of interesting     *
     * metadata: locally, how old is this character; and globally, how many   *
     * characters have been created by the game.                              */
    uint32_t sequence_number;
    npc_t *npc;
    pc_t *pc;
    uint32_t kills[num_kill_types];
};

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);
uint32_t can_see(dungeon_t *d, character *voyeur, character *exhibitionist);
void character_delete(void *c);

#endif
