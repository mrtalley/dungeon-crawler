#include <stdio.h>
#include <endian.h>
#include <math.h>

int main(int argc, char *argv[])
{
  union {  /* Compiler allocates 4 bytes -- enough space for largest field */
    int i;
    float f;
    char c[4];
  } u;
  float f;

  u.i = 0x01020304;

  printf("%#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htobe32(u.i);

  printf("%#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = be32toh(u.i);

  printf("%#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.f = M_PI;

  printf("%f: %#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htobe32(u.i);

  printf("%f: %#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = be32toh(u.i);

  printf("%f: %#x == %d\nByte order: 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);


  f = M_PI;
  printf("%f\n", f);
  printf("%d != %d\n", (int) f, *((int *) &f));
  *((int *) &f) = htobe32(*((int *) &f));
  printf("%f\n", f);
  *((int *) &f) = be32toh(*((int *) &f));
  printf("%f\n", f);

  return 0;
}
