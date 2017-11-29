#ifndef OBJECT_H
# define OBJECT_H

# include <string>

# include "descriptions.h"
# include "dims.h"

class object {
 private:
  const std::string &name;
  const std::string &description;
  object_type_t type;
  uint32_t color;
  pair_t position;
  const dice &damage;
  int32_t hit, dodge, defence, weight, speed, attribute, value;
  bool seen;
  object *next;
 public:
  object(const object_description &o, pair_t p, object *next);
  ~object();
  inline int32_t get_damage_base() const
  {
    return damage.get_base();
  }
  inline int32_t get_damage_number() const
  {
    return damage.get_number();
  }
  inline int32_t get_damage_sides() const
  {
    return damage.get_sides();
  }
  char get_symbol();
  uint32_t get_color();
  const char *get_name();
  int32_t get_speed();
  int32_t roll_dice();
  int32_t get_type();
  bool have_seen() { return seen; }
  void has_been_seen() { seen = true; }
  int16_t *get_position() { return position; }
  uint32_t is_equipable();
  uint32_t is_removable();
  uint32_t is_dropable();
  uint32_t is_destructable();
  int32_t get_eq_slot_index();
  void to_pile(dungeon_t *d, pair_t location);
  inline object *get_next() { return next; }
  inline void set_next(object *n) { next = n; }
  const char *get_description() { return description.c_str(); }
};

void gen_objects(dungeon_t *d);
char object_get_symbol(object *o);
void destroy_objects(dungeon_t *d);

#endif
