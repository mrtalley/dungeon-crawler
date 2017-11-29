#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>
# include <vector>
# include <cstdlib>

# include "dims.h"
# include "utils.h"

typedef enum kill_type {
  kill_direct,
  kill_avenged,
  num_kill_types
} kill_type_t;

class dice;

class character {
 public:
  virtual ~character() {}
  char symbol;
  pair_t position;
  int32_t speed;
  uint32_t alive;
  std::vector<uint32_t> color;
  uint32_t hp;
  const dice *damage;
  const char *name;
  /* Characters use to have a next_turn for the move queue.  Now that it is *
   * an event queue, there's no need for that here.  Instead it's in the    *
   * event.  Similarly, sequence_number was introduced in order to ensure   *
   * that the queue remains stable.  Also no longer necessary here, but in  *
   * this case, we'll keep it, because it provides a bit of interesting     *
   * metadata: locally, how old is this character; and globally, how many   *
   * characters have been created by the game.                              */
  uint32_t sequence_number;
  uint32_t kills[num_kill_types];
  inline uint32_t get_color() { return color[rand_range(0, color.size() - 1)]; }
  inline char get_symbol() { return symbol; }
};

class dungeon;

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);
/* can_see() is a bit overloaded.  is_pc controls the range (NPCs can see    *
 * farther than the PC.  learn controls whether the PC should learn terrain. */
uint32_t can_see(dungeon *d, pair_t voyeur, pair_t exhibitionist,
                 int is_pc, int learn);
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
const char *character_get_name(const character *c);

#endif
