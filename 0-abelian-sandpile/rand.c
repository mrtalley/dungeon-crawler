#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int i, n;

  for (i = 0, n = atoi(argv[1]); i < n; i++) {
    /* The last value (rand() % 5) can produce zeros, which will have no *
     * effect in most implementations.  In my implementation, I place    *
     * sinks where there is a zero salt.                                 */
    printf(" %d %d %d", rand() % 23, rand() % 23, rand() % 5);
  }

  return 0;
}
