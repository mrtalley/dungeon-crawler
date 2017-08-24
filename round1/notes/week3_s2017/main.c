#include <stdio.h>

#include "stack.h"

int main(int argc, char *argv[])
{
  stack_t s;
  int i;

  stack_init(&s);

  stack_push(&s, 1);
  stack_push(&s, 2);
  stack_push(&s, 3);
  stack_push(&s, 4);
  stack_push(&s, 5);

  while (stack_size(&s)) {
    stack_pop(&s, &i);
    printf("%d\n", i);
  }

  stack_push(&s, 6);
  stack_push(&s, 7);
  stack_push(&s, 8);
  stack_push(&s, 9);
  stack_push(&s, 10);
 
  stack_pop(&s, &i);
  printf("%d\n", i);

  stack_pop(&s, &i);
  printf("%d\n", i);

  stack_pop(&s, &i);
  printf("%d\n", i);

  stack_delete(&s);

  if (stack_pop(&s, &i)) {
    fprintf(stderr, "Error!\n");
  }

  return 0;
}
