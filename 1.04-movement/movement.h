//
// Created by Max Talley on 2/20/17.
//

#ifndef INC_1_04_MOVEMENT_MOVEMENT_H
#define INC_1_04_MOVEMENT_MOVEMENT_H

#define INTELLIGENT 0b00000001
#define TELEPATHIC  0b00000010
#define TUNNELING   0b00000100
#define ERRATIC     0b00001000
#define PLAYER      0b00010000

void npc_define(dungeon_t *d, int numNPC, heap_t h);
void char_movement(dungeon_t *d, heap_t h, unsigned long ticks);

#endif //INC_1_04_MOVEMENT_MOVEMENT_H
