#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int i;
  char a[4] = "\\|/-";

  for (i = 0; i < 320; i++) {
    usleep(125000);
    if (i && !(i % 4)) {
      printf("-");
    }
    if (i) {
      printf("\x1b[1D");
    }
    printf("%c", a[i % 4]);
    fflush(stdout);
  }

  return 0;
}
