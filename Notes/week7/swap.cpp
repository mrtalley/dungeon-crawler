#include <iostream>

using namespace std;

void ccswap(int *i, int *j) {
  int tmp;

  tmp = *i;
  *i = *j;
  *j = tmp;
}

// in c++
void cppswap(int &i, int &j) {
  int tmp;
  tmp = i;
  i = j;
  j = tmp;
}

int main(int argc, char *argv[]) {
  int i, j;
  i = 3;
  j = 5;
  cswap(&i, &j);
  cout << "i: " << i << "j: " << j;

  return 0;
}
