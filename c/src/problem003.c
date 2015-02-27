/*
 * problem003.c
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */
#include <stdio.h>
#include <math.h>
#include "primeTest.h"

#define NUMBER 600851475143

int main(void) {
	bigint num = NUMBER;
	bigint root_num = (bigint) sqrt( (double) num);

	while (root_num > 1) {
		if (is_prime(root_num) && ((num % root_num) == 0)) {
			printf("The largets prime factor: %llu\n", root_num);
			break;
		}
		--root_num;
	}

	return 0;
}
