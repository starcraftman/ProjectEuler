/*
 * problem005.c
 *
 *  Created on: 2012-04-28
 *      Author: starcraftman
 */
#include <stdio.h>
#include <string.h>
#include "general.h"

bool divisibleByAll(int n);

int main(void) {
	// Can be calculated by simply taking all prime factorizations multiplied.
	// num = 2 * 3 * 2 * 5 * 7 * 2 * 3 * 11 * 2 * 13 * 2 * 17 * 19;
	bigint num = 1;

	for ( ; ; num++)
		if ( divisibleByAll(num))
			break;

	printf("Num %llu is divisible by 1-20 \n", num);


	return 0;
}

bool divisibleByAll(int n) {
	int i = 0;
	for (i = 1; i <= 20; ++i) {
		if ( (n % i) != 0)
			return FALSE;
	}

	return TRUE;
}
