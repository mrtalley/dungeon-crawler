#include <stdlib.h>

#include "stack.h"

int stack_init(stack_t *s)
{
  s->size = 0;
  s->top = NULL;

  return 0;
}

int stack_delete(stack_t *s)
{
  stack_node_t *n;

  /* This works just fine
  int i;
  while (s->size) {
    stack_pop(s, &i);
  }
  */

  for (n = s->top; n; n = s->top) {
    s->top = s->top->next;
    free(n);
  }

  return 0;
}

int stack_push(stack_t *s, int v)
{
  stack_node_t *n;

  if (!(n = malloc(sizeof (*n)))) {
    return 1;
  }

  n->value = v;
  n->next = s->top;
  s->top = n;
  s->size++;

  return 0;
}

int stack_pop(stack_t *s, int *v)
{
  stack_node_t *n;

  if (!s->top /* or !s->size */) {
    return 1;
  }

  n = s->top;
  s->top = s->top->next;
  s->size--;
  *v = n->value;
  free(n);

  return 0;
}

int stack_size(stack_t *s)
{
  return s->size;
}

int stack_is_empty(stack_t *s)
{
  return !s->size;
}

