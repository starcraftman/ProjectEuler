/**
 * Dequeue primarily targetted at ints.
 */
#include <stdlib.h>
#include "dequeueInt.h"

int dq_get_size(dequeue_t *d) {
	return d->size;
}

CODES dq_add_end(dequeue_t *d, node_t *n) {
	if (d->end == NULL) {
		d->start = n;
		d->end = n;
		d->size = 1;
	} else {
		n->prev = d->end;
		d->end->next = n;
		d->end = n;
		d->size++;
	}

	return SUCESS;
}

CODES dq_add_start(dequeue_t *d, node_t *n) {
	if (d->start == NULL) {
		d->start = n;
		d->end = n;
		d->size = 1;
	} else {
		n->next = d->start;
		d->start->prev = n;
		d->start = n;
		d->size++;
	}

	return SUCESS;
}

node_t *dq_remove_end(dequeue_t *d) {
	node_t *n = NULL;
	if (d->end == NULL)
		return NULL;
	else if (d->end == d->start) { // Only one node_t left.
		n = d->end;
		d->end = NULL;
		d->start = NULL;
	} else { // More than one.
		n = d->end;
		d->end->prev->next = NULL;
		d->end = d->end->prev;
		n->prev = NULL;
	}
	d->size--;
	return n;
}

node_t *dq_remove_start(dequeue_t *d) {
	node_t *n = NULL;
	if (d->start == NULL)
		return NULL;
	else if (d->end == d->start) { // Only one node_t left.
		n = d->start;
		d->start = NULL;
		d->end = NULL;
	} else { // More than one.
		n = d->start;
		d->start->next->prev = NULL;
		d->start = d->start->next;
		n->next = NULL;
	}
	d->size--;
	return n;
}

dequeue_t *dq_make_dequeue() {
	dequeue_t *q = (dequeue_t *)malloc(sizeof(dequeue_t));

	q->size = 0;
	q->start = NULL;
	q->end = NULL;

	return q;
}

node_t *dq_make_node(node_type i) {
	node_t *n = (node_t *)malloc(sizeof(node_t));

	n->val = i;
	n->next = NULL;
	n->prev = NULL;

	return n;
}

bool dq_is_empty(dequeue_t *d) {
	if (d->end != d->start)
		return FALSE;
	else if (d->start == NULL)
		return TRUE;

	return FALSE;
}

node_t *dq_peek_start(dequeue_t *d) {
	return d->start;
}

node_t *dq_peek_end(dequeue_t *d) {
	return d->end;
}

void dq_free_node(node_t *n) {
	free(n);
}

void dq_free_dequeue(dequeue_t *d) {
	node_t *n ;

	while ( (n = dq_remove_end(d)) != NULL)
		dq_free_node(n);

	free(d);
}
