#ifndef MOVEMENT_H
#define MOVEMENT_H

void intelligence(monster_t *monster, uint8_t distance_map[ROWS][COLS]);
int genMonsterCode();
void placeMonster(dungeon_t *d, int *y, int *x);
int determineMonsterTraits(int monsterCode);
void moveCharacter(dungeon_t *d, heap_t h);

#endif
