#include <stdio.h>
#include <endian.h> // this wont compile on OSX
#include <math.h>

int main(int argc, char *argv[]) {
  union {
    int i;
    float f;
    char c[4];
  } u;

  u.i = 0x01020304;

  printf("%#x == %d\nByteOrder: 0x%02hhx0x%02hhx0x%02hhx0x%02hhx0x%02hhx0x%02hhx\n\n", u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htobe32(u.i); // switches from big to little endian

  u.f = M_PI;

  printf("%f: %#x == %d\nByteOrder: 0x%02hhx0x%02hhx0x%02hhx0x%02hhx0x%02hhx0x%02hhx\n\n", u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  f = M_PI;
  printf("%f\n", f);
  *((int *) &f) = htobe32(*((int *) &f));
  /*
    Start with int, so (int) f doesnt change anything
    (int *) &f, no implicit cast happening now (the thing that f points to is an int)
    Must dereference so it gets the bits and not the int ()*(int *) &f)
  */
  printf("%f\n", f);
  *((int *) &f) = be32toh(*((int *) &f));
  printf("%f\n", f);

  return 0;
}
