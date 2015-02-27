/*
 * problem010.c
 *
 *  Created on: 2012-04-30
 *      Author: starcraftman
 */
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "primeTest.h"

#define MAX_VAL 2000000

int main(void) {
	bigint sum = 0, i = 0;

	for (i = 0; i < MAX_VAL; ++i)
		if (is_prime(i)) {
			sum += i;
			printf("Prime: %llu\n", i);
		}

	printf("The sum is: %llu\n", sum);
	return 0;
}
