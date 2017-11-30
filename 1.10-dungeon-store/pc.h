#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"
# include "character.h"
# include "dungeon.h"

typedef enum eq_slot {
  eq_slot_weapon,
  eq_slot_offhand,
  eq_slot_ranged,
  eq_slot_light,
  eq_slot_armor,
  eq_slot_helmet,
  eq_slot_cloak,
  eq_slot_gloves,
  eq_slot_boots,
  eq_slot_amulet,
  eq_slot_lring,
  eq_slot_rring,
  num_eq_slots
} eq_slot_t;

extern const char *eq_slot_name[num_eq_slots];

class pc : public character {
 private:
  void recalculate_speed();
  uint32_t has_open_inventory_slot();
  int32_t get_first_open_inventory_slot();
  object *from_pile(dungeon_t *d, pair_t pos);
  uint32_t gold;
 public:
  pc();
  ~pc();

  object *eq[num_eq_slots];
  object *in[MAX_INVENTORY];

  uint32_t wear_in(uint32_t slot);
  uint32_t remove_eq(uint32_t slot);
  uint32_t drop_in(dungeon_t *d, uint32_t slot, bool sell);
  uint32_t destroy_in(uint32_t slot);
  uint32_t pick_up(dungeon_t *d, bool purchase);
  terrain_type_t known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];
  inline uint32_t get_gold()
  {
    return gold;
  }
  inline void subtract_gold(uint32_t amount)
  {
    gold -= amount;
  }
  inline void add_gold(uint32_t amount)
  {
    gold += amount;
  }
};

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter);
terrain_type_t pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);

#endif
