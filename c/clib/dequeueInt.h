/*
 * dequeueInt.h
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */

#ifndef DEQUEUEINT_H_
#define DEQUEUEINT_H_

typedef enum CODES {
	SUCESS = 0,
	EMPTY_DEQUEUE = 1,
} CODES;

typedef enum BOOL {
	FALSE = 0,
	TRUE
} bool;

typedef struct node node;
typedef int node_type;

typedef struct node {
	node_type val;
	node *next;
	node *prev;
} node_t;

typedef struct dequeue {
	node *start;
	node *end;
	int size;
} dequeue_t;

int dq_get_size(dequeue_t *d);

CODES dq_add_end(dequeue_t *d, node *n);

CODES dq_add_start(dequeue_t *d, node *n);

node *dq_remove_end(dequeue_t *d);

node *dq_remove_start(dequeue_t *d);

dequeue_t *dq_make_dequeue();

node *dq_make_node(node_type i);

node *dq_peek_start(dequeue_t *d);

node *dq_peek_end(dequeue_t *d);

void dq_free_node(node *n);

void dq_free_dequeue(dequeue_t *d);

bool dq_is_empty(dequeue_t *d);

#endif /* DEQUEUEINT_H_ */
