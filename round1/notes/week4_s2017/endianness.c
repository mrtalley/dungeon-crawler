#include <endian.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
  union {
    int i;
    float f;
    char c[4];
  } u;
  float f;

  u.i = 0x01020304;

  printf("%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htobe32(u.i);

  printf("%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = be32toh(u.i);

  printf("%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htole32(u.i);

  printf("%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.f = M_PI;

  printf("%f\n%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.f = htobe32(u.f);
  printf("%f\n%#x == %d\nLittle endian: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.f = be32toh(u.f);
  printf("%f\n%#x == %d\nHost order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.f = M_PI;
  printf("%f\n%#x == %d\nByte order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = htobe32(u.i);
  printf("%f\n%#x == %d\nLittle endian: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  u.i = be32toh(u.i);
  printf("%f\n%#x == %d\nHost order: 0x%02hhx%02hhx%02hhx%02hhx\n\n",
         u.f, u.i, u.i, u.c[0], u.c[1], u.c[2], u.c[3]);

  f = M_PI;
  printf("%f\n", f);
  *((int *) &f) = htobe32(*((int *) &f));
  printf("%f\n", f);
  *((int *) &f) = be32toh(*((int *) &f));
  printf("%f\n", f);
  

  return 0;
}
