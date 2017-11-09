#ifndef OBJECT_H
#define OBJECT_H

#include "dice.h"

typedef class dungeon dungeon_t;

class object {
public:
    std::string name, description;
    char type;
    uint32_t color;
    int hit, dodge, defence, weight, speed, attribute, value;
    dice damage;
    int x_pos, y_pos;

    inline int roll_damage() { return damage.roll(); }
};

void gen_objects(dungeon_t *d);

#endif
