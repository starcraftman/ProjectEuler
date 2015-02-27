/*
 * problem004.c
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */

/*
 * A palindromic number reads the same both ways.
 * The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.
 * Find the largest palindrome made from the product of two 3-digit numbers.
 *
 */
#include <stdio.h>
#include <string.h>
#include "dequeueInt.h"

#define MIN_VAL 100
#define MAX_VAL 1000

bool is_palindrome2(int i) {
	int reversed = 0, original = i;

	while (i > 0) {
		reversed *= 10;
		reversed += i % 10;
		i /= 10;
	}

	if (reversed == original)
		return TRUE;
	else
		return FALSE;
}

bool is_palindrome(int i) {
	int digit = 0;
	dequeue *d = NULL;
	node *start, *end;

	d = dq_make_dequeue();
	while (i != 0) {
		digit = i % 10;
		i /= 10;
		start = dq_make_node(digit);
		dq_add_end(d, start);
	}

	// Must be even to be palindrome.
	if ( (dq_get_size(d) % 2) != 0) {
		dq_free_dequeue(d);
		return FALSE;
	}

	// Pop front and back of dequeue, if not equal false.
	while (!dq_is_empty(d)) {
		start = dq_peek_start(d);
		end = dq_peek_end(d);
		if (start->val != end->val) {
			dq_free_dequeue(d);
			return FALSE;
		}

		dq_free_node(dq_remove_end(d));
		dq_free_node(dq_remove_start(d));
	}

	dq_free_dequeue(d);
	return TRUE;
}

int main(void) {
	int i, j, largest_pal_so_far = 0, temp_pal = 0;
	for (i = MIN_VAL; i < MAX_VAL; ++i)
		for (j = MIN_VAL; j < MAX_VAL; ++j) {
			temp_pal = i * j;
			if (is_palindrome(temp_pal) && temp_pal > largest_pal_so_far)
				largest_pal_so_far = temp_pal;
		}

	printf("The largest palindrome found was %d\n", largest_pal_so_far);

	return 0;
}
