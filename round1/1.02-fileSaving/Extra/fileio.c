#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "fileio.h"

// // saves file for loading later
// void fileWriter()
// {
//   char* directory = getenv("HOME");
//   strcat(directory, "/.rld327/dungeon.rld327");
//   printf("%s\n", directory);
//
//   FILE* file = fopen(directory, "w");
//
//   // writing RLG327-S2017
//   char* RLG = "RLG327-S2017";
//   fwrite(RLG, sizeof(RLG), 1, file);
//   printf("Written %s\n", RLG);
//
//   // writing version marker
//   int32_t version = 0;
//   fwrite(&version, sizeof(version), 1, file);
//   printf("Written Version\n");
//
//   // writing size of file
//   struct stat st;
//   stat(directory, &st);
//   int32_t size = st.st_size;
//   printf("%d\n", size);
//   fwrite(&size, sizeof(size), 1, file);
//   if(size > 0)
//     printf("Written File Size");
//
//
//
//   fclose(file);
// }

int fileReader(char *path, dungeon_t d)
{
  char* directory = getenv("HOME");
  char name[12];
  FILE* file;
  unsigned char size[1];
  int row = 0, col = 0;
  //uint32_t hardness[DUNGEON_Y][DUNGEON_X];

  // Reading directory
  strcat(directory, "/.rld327/");
  strcat(directory, path);
  printf("Directory: %s\n", directory);

  if((file = fopen(directory, "r")) != NULL) {
    printf("File Open Success\n");
  }

  else {
    printf("File Opening Failed\n");
    return -1;
  }

  // Reading file name
  printf("Reading Name... "); fflush(file);
  fread(name, 11, 1, file);
  printf("Done. Name = %s\n", name);

  // Reading size of file
  fseek(file, 4, 11);
  printf("Reading Size... "); fflush(file);
  fread(size, 4, 1, file);
  printf("Done. Size = %s\n", size);

  // Reading in dungeon hardness
  while(feof(file))
  {
    unsigned char *holder = 0;
    fread(holder, 4, 1, file);
    d.hardness[row][col] = *holder;
    col++;

    if(col == 159)
      row++; col = 0;

    if(row == 105)
      break;
  }

  printf("File Read\n");

  for(row = 0; row < 105; row++) {
    for(col = 0; col < 160; col++) {
      if(d.hardness[row][col] == 0)
        printf("#");
      else printf(" ");
    }
    printf("\n");
  }

  //fclose(file);

  return 0;
}
