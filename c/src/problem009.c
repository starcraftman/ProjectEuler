/*
 * problem009.c
 *
 *  Created on: 2012-04-30
 *      Author: starcraftman
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "general.h"

#define MAX_VAL 1000

bool is_pythagorean_triplet(int a, int b, int c) {
	if (a > c || b > c)
		return FALSE;

	int lhs = (int) pow(a, 2) + pow(b, 2);
	if (lhs == ((int) pow(c, 2)))
		return TRUE;
	else
		return FALSE;
}

/**
 * C must always be greater than a or b.
 * @param argc
 * @param argv
 * @return
 */
int main(void) {
	int a = 1, b = 1, c = 2;

	for (a = 1; a < MAX_VAL; ++a)
		for (b = a; b < MAX_VAL; ++b)
			for (c = b + 1; c < MAX_VAL; ++c) {
				if (is_pythagorean_triplet(a, b, c) && ((a + b + c) == 1000)) {
					printf("Triplet: %d %d %d, product: %d\n", a, b, c, a * b * c);
				}
			}

	return 0;
}


