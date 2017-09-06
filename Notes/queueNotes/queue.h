#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_node {
  int value;
  struct queue_node_t *next;
} queue_node_t;

typedef struct queue {
  int size;
  queue_node_t *head, *tail;
} queue_t;

int queue_init(queue_t *q);
int queue_delete(queue_t *q, int v);
int queue_add(queue_t *q, int *v);
int queue_remove(queue_t *q, int *v);
int queue_peek(queue_t *q);
int queue_size(queue_t *q);

#endif
