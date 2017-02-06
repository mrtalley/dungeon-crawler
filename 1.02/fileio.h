#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

//void fileWriter();
struct dungeon;
typedef struct dungeon dungeon_t;
int fileReader(char* path, dungeon_t d);


#endif
