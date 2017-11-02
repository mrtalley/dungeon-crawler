#ifndef FILEIO_H
#define FILEIO_H

enum Dice {
    base,
    dice,
    sides
};

typedef class dungeon dungeon_t;

void parse_monster_file(dungeon_t *d);
void print_monster_descriptions(dungeon_t *d);

#endif
