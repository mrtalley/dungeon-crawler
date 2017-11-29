#ifndef PC_H
# define PC_H

# include <stdint.h>
# include <vector>

# include "dims.h"
# include "character.h"
# include "dungeon.h"
# include "object.h"

#define EQUIPSLOTS 12
#define CARRYSLOTS 10

class object;

class pc : public character {
  public:
    ~pc() {}
    terrain_type_t known_terrain[DUNGEON_Y][DUNGEON_X];
    uint8_t visible[DUNGEON_Y][DUNGEON_X];
    /* equipment is what the pc is wearing / weilding  */
    object equipment[EQUIPSLOTS];
    /* carry is the inventory  */
    std::vector<object> carry;
    object *get_equipment(char key);
    void set_equipment(object *o);
    // add function here to set an equipment slot
    bool has_open_carry_slots();
    object *get_carry(char key);
    object remove_carry(char key);
    int set_carry(object *o);
    void del_carry(dungeon_t *d, char key);
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
