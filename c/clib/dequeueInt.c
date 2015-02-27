/**
 * Dequeue primarily targetted at ints.
 */
#include <stdlib.h>
#include "dequeueInt.h"

int dq_get_size(dequeue *d) {
	return d->size;
}

CODES dq_add_end(dequeue *d, node *n) {
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

CODES dq_add_start(dequeue *d, node *n) {
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

node *dq_remove_end(dequeue *d) {
	node *n = NULL;
	if (d->end == NULL)
		return NULL;
	else if (d->end == d->start) { // Only one node left.
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

node *dq_remove_start(dequeue *d) {
	node *n = NULL;
	if (d->start == NULL)
		return NULL;
	else if (d->end == d->start) { // Only one node left.
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

dequeue *dq_make_dequeue() {
	dequeue *q = malloc(sizeof(dequeue));

	q->size = 0;
	q->start = NULL;
	q->end = NULL;

	return q;
}

node *dq_make_node(node_type i) {
	node *n = malloc(sizeof(node));

	n->val = i;
	n->next = NULL;
	n->prev = NULL;

	return n;
}

bool dq_is_empty(dequeue *d) {
	if (d->end != d->start)
		return FALSE;
	else if (d->start == NULL)
		return TRUE;

	return FALSE;
}

node *dq_peek_start(dequeue *d) {
	return d->start;
}

node *dq_peek_end(dequeue *d) {
	return d->end;
}

void dq_free_node(node *n) {
	free(n);
}

void dq_free_dequeue(dequeue *d) {
	node *n ;

	while ( (n = dq_remove_end(d)) != NULL)
		dq_free_node(n);

	free(d);
}
