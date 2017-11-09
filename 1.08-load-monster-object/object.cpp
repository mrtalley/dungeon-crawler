#include <cstdio>
#include <cstdlib>

#include "object.h"
#include "dungeon.h"

void gen_objects(dungeon_t *d)
{
    srand(time(NULL));
    int num_objs = rand() % (20 - 12) + 12;
    d->num_objects = num_objs;
    int rand_y, rand_x, rand_obj;
    int i = 0;

    while(i < num_objs) {
        rand_y = rand() % DUNGEON_Y;
        rand_x = rand() % DUNGEON_X;
        rand_obj = rand() % d->object_descriptions.size();

        if(in_room(d, rand_y, rand_x)) {
            object o;
            o.x_pos = rand_x;
            o.y_pos = rand_y;
            d->object_descriptions.at(rand_obj).create_object_instance(&o);
            d->object_map[rand_y][rand_x] = o;
            i++;
        }
    }
}
