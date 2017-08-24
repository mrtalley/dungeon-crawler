#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(int *a, int n)
{
  int i, j, t;

  for (i = 1; i < n; i++) {
    for (t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
      a[j + 1] = a[j];
    }
    a[j + 1] = t;
  }
}

void generic_insertion_sort(void *data, int n, size_t s,
                            int (*compare)(const void *, const void *))
{
  int i, j;
  void *t;
  char *a;

  t = malloc(s);
  a = data;  /* a needs to be characters for byte addressing */

  for (i = 1; i < n; i++) {
    for (memcpy(t, a + (s * i), s), j = i - 1;
         j > -1 && compare(a + (s * j), t) > 0;
         j--) {
      memcpy(a + (s * (j + 1)), a + (s * j), s);
    }
    memcpy(a + (s * (j + 1)), t, s);
  }

  free(t);
}

int compare_int(const void *first, const void *second)
{
  return (*((int *) first) - *((int *) second));
}

int compare_string(const void *first, const void *second)
{
  return strcmp(*((char **) first), *((char **) second));
}

int main(int argc, char *argv[])
{
  int a[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  int i;
  char *s[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "zero",
  };

  for (i = 0; i < sizeof (a) / sizeof (a[0]); i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  /*  insertion_sort(a, sizeof (a) / sizeof (a[0])); */
  generic_insertion_sort(a, sizeof (a) / sizeof (a[0]),
                         sizeof (a[0]), compare_int);

  for (i = 0; i < sizeof (a) / sizeof (a[0]); i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  for (i = 0; i < sizeof (s) / sizeof (s[0]); i++) {
    printf("%s ", s[i]);
  }
  printf("\n");
 
  /*
  generic_insertion_sort(s, sizeof (s) / sizeof (s[0]),
                         sizeof (s[0]), compare_string);
  */
  qsort(s, sizeof (s) / sizeof (s[0]), sizeof (s[0]), compare_string);

  for (i = 0; i < sizeof (s) / sizeof (s[0]); i++) {
    printf("%s ", s[i]);
  }
  printf("\n"); 

  char *key = "five";

  /* The problem with the way we made this call in class, passing the       *
   * string literal "seven" as the search key was that it was subsequently  *
   * passed to compare string as a char *, but treated there are a char **. *
   * There are two solutions to this.  The first is what I've done here.    *
   * Store the address of your key in a variable so that you can pass its   *
   * address.  The second is to pass a comparitor that expects a char * in  *
   * the first argument and a char ** in the second.                        */
  printf("%s\n", *(char **) bsearch(&key, s, sizeof (s) / sizeof (s[0]),
                                  sizeof (s[0]), compare_string));

  return 0;
}
