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

struct node {
	node_type val;
	node *next;
	node *prev;
};

typedef struct dequeue {
	node *start;
	node *end;
	int size;
} dequeue;

int dq_get_size(dequeue *d);

CODES dq_add_end(dequeue *d, node *n);

CODES dq_add_start(dequeue *d, node *n);

node *dq_remove_end(dequeue *d);

node *dq_remove_start(dequeue *d);

dequeue *dq_make_dequeue();

node *dq_make_node(node_type i);

node *dq_peek_start(dequeue *d);

node *dq_peek_end(dequeue *d);

void dq_free_node(node *n);

void dq_free_dequeue(dequeue *d);

bool dq_is_empty(dequeue *d);

#endif /* DEQUEUEINT_H_ */
