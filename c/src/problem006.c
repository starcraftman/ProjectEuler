/*
 * problem003.c
 *
 *  Created on: 2012-04-28
 *      Author: starcraftman
 */

#include <stdio.h>
#include <math.h>
#include "general.h"
#include <limits.h>

bigint sumOfSquares(int limit) {
	int i = 0;
	bigint sum = 0;

	for (i = 0; i < limit; ++i)
		sum += i * i;

	return sum;
}

bigint squareOfSums(int limit) {
	bigint sum = 0;
	int i = 0;

	for (i = 0; i < limit; ++i)
		sum += i;

	sum *= sum;

	return sum;
}

int main(void) {
	bigint difference;

	difference = squareOfSums(101) - sumOfSquares(101);

	printf("The difference is: %llu\n", difference);
	printf("Max of bigint %llu", ULLONG_MAX);

	return 0;
}
