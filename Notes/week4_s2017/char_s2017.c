#include <stdio.h>
#include <ctype.h>

char *words[] = {
  "america",
  "ballast",
  "cookie",
  "dungeon",
  "elaborate",
  "fabulous",
  "good",
  "heliocentric",
  "igloo",
  "just",
  "kappa",
  "luser",
  "mana",
  "n00b",
  "onomonpia",
  "pteryldactyl",
  "quinoa",
  "raunchy",
  "slipstream",
  "tuberculosis",
  "underwear",
  "vibrulent",
  "wombat",
  "xylophone",
  "yankee",
  "zipper"
};

int main(int argc, char *argv[])
{
  printf("%c is for %s!\n", argv[1][0], words[toupper(argv[1][0]) - 'A']);

  return 0;
}
