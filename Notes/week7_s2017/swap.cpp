#include <iostream>

using namespace std;

void cswap(int *i, int *j)
{
  int tmp;

  tmp = *i;
  *i = *j;
  *j = tmp;
}

void cppswap(int &i, int &j)
{
  int tmp;

  tmp = i;
  i = j;
  j = tmp;
}

int &return_reference(int i)
{
  static int array[10];
  int j;

  for (j = 0; j < 10; j++) {
    cout << array[j] << " ";
  }
  cout << endl;

  return array[i];
}

int main(int argc, char *argv[])
{
  int i, j;

  i = 3;
  j = 5;

  cswap(&i, &j);
  cout << "i: " << i << " j: " << j << endl;

  cppswap(i, j);
  cout << "i: " << i << " j: " << j << endl;

  int &k = return_reference(3);
  k = 6;
  int &l = return_reference(5);
  k = 6; l = 12;
  int &m = return_reference(7);
  k = 1; l = 7; m = 11;
  int &n = return_reference(9);

  return 0;
}
