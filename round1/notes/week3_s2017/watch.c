#include <stdio.h>

int main(int argc, char *argv[])
{
  int a[100];
  int i;
  int b = 777;

  for (i = 0; i < 105; i++) {
    a[i] = i;
  }

  printf("%d\n", b);

  return 0;
}
