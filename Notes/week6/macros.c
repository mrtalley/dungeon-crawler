#include <stdio.h>

// concatenation operator is ##
#define concat(x,y) x ## y
#define to_string(x) #x

int main(int argc, char *argv[]) {
  printf("%d\n", concat(1,2)); // returns 12
  printf("%d\n", concat(1,2) + 3); // returns 15
  printf("%s\n", to_string(CS 327)); // returns CS 327 as a string

  return 0;
}
