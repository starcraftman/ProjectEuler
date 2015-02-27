/*
 * problem013.c
 *
 *  Created on: 2012-04-29
 *      Author: starcraftman
 */

#include <stdio.h>
#include "general.h"

bigint extractFirst10(const char *buff) {
	int i = 0;
	bigint sum = 0;

	for (i = 0; i < 10; ++i)
		sum = (sum * 10) + *(buff++) - CHAR_VAL_0;

	return sum;
}

int main(void) {
	bigint sum = 0;
	char buffer[50];

	while ( (scanf("%s", buffer) != EOF)) {
		sum += extractFirst10(buffer);
		printf("Buffer: %s, SUM: %llu\n", buffer, sum);
	}
	printf("%llu\n", sum);

	return 0;
}
