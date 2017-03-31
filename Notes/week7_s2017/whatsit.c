#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int i;
  char a[4] = "\\|/-";
  char *pad = "----------------------------------------"
              "----------------------------------------";

  for (i = 0; i < 320; i++) {
    usleep(125000);
    printf("\r%s%c", pad + (80 - (i / 4)), a[i % 4]);
    fflush(stdout);
  }

  return 0;
}
