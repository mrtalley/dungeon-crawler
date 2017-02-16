#include <stdio.h>

#define class1 227
#define class2 228
#define class3 327

/* This is a really bad implementation of min *
 * if actual parameters are not constant      */
#define min(x, y) x < y ? x : y

#define max(x, y) ({ \
  typeof (x) _x = x; \
  typeof (y) _y = y; \
  _x > _y ? _x : _y;
})

int main(int argc, char *argv[])
{
  printf("%d\n", min(3, 5));
  printf("%d\n", min(class2, class1));
  printf("%d\n", max(class1, class3));

  return 0;
}
