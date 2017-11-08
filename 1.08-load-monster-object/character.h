#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>

# include "dims.h"
#include "dice.h"

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
    int32_t hitpoints;
    dice damage;
    uint32_t alive;
    /* Characters use to have a next_turn for the move queue.  Now that it is *
     * an event queue, there's no need for that here.  Instead it's in the    *
     * event.  Similarly, sequence_number was introduced in order to ensure   *
     * that the queue remains stable.  Also no longer necessary here, but in  *
     * this case, we'll keep it, because it provides a bit of interesting     *
     * metadata: locally, how old is this character; and globally, how many   *
     * characters have been created by the game.                              */
    uint32_t sequence_number;
    uint32_t kills[num_kill_types];
};

class dungeon;

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);
uint32_t can_see(dungeon *d, pair_t voyeur, pair_t exhibitionist, int is_pc);
void character_delete(character *c);
int16_t *character_get_pos(character *c);
int16_t character_get_y(const character *c);
int16_t character_set_y(character *c, int16_t y);
int16_t character_get_x(const character *c);
int16_t character_set_x(character *c, int16_t x);
uint32_t character_get_next_turn(const character *c);
void character_die(character *c);
int character_is_alive(const character *c);
void character_next_turn(character *c);
void character_reset_turn(character *c);
char character_get_symbol(const character *c);
uint32_t character_get_speed(const character *c);
uint32_t character_get_dkills(const character *c);
uint32_t character_get_ikills(const character *c);
uint32_t character_increment_dkills(character *c);
uint32_t character_increment_ikills(character *c, uint32_t k);

#endif
