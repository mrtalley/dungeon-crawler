#ifndef MOVEMENT_H
#define MOVEMENT_H

void intelligence(monster_t *monster, uint8_t distance_map[ROWS][COLS]);
int genMonsterCode();
void placeMonster(dungeon_t *d, int i);
int determineMonsterTraits(int monsterCode);

#endif
