/*
 * primeTest.c
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "primeTest.h"

bool is_prime(bigint i) {
	bigint factor = 0, root_i = (bigint) sqrt( (double) i);

	// Not considered prime.
	if (i == 0 || i == 1)
		return FALSE;

	if (i == 2)
		return TRUE;

	// Ignore even numbers.
	if ( (i % 2) == 0)
		return FALSE;

	// Trial the odds.
	for (factor = 3; factor <= root_i; factor += 2)
		if ( (i % factor) == 0)
			return FALSE;

	return TRUE;
}

