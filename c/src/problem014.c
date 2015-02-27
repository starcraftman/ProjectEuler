/*
 * problem014.c
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */
#include <stdio.h>

#define MAX_VAL 1000000

/* Determine the number of iterations starting with n based on rules. */
int apply_rules(unsigned long n) {
	int iterations = 0;

	while (n > 1) {
		if ( (n % 2) == 0)
			n /= 2;
		else
			n = (3 * n) + 1;
		++iterations;
	}

	return iterations;
}

int main(void) {
	unsigned long start = 1;
	int iters = 0, longest = 0;

	for (start = 1; start < MAX_VAL; start++) {
		if ( (iters = apply_rules(start)) > longest) {
			longest = iters;
			printf("Longest chain: starts with %6lu; has length %d\n", start, longest);
		}
	}

	return 0;
}
