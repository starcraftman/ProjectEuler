/*
 ============================================================================
 Name        : ProjectEuler.c
 Author      : starcraft.man
 Version     :
 Copyright   : Public Domain
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "general.h"

int multipleThreeOrFive(int n) {
	return ( (n % 3) == 0 || (n % 5) == 0);
}

int main(void) {
	long sum = 0;
	int i = 0;

	for (i = 0; i < 1000; ++i) {
		if (multipleThreeOrFive(i))
			sum += i;
	}

	printf("The sum of all numbers up to 1000 divisible by 3 or 5:\n%ld\n", sum);

	return 0;
}
