/** Project Euler problem 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general.h"

#define MAX_VAL 4000000

typedef struct fibHold {
	bigint two_back;
	bigint one_back;
	bigint latest;
} FIB;

/**
 * Returns the next fibonacci number.
 * @param fibo
 */
void nextFibonacci(FIB *fibo) {
	fibo->latest = fibo->one_back + fibo->two_back;
	fibo->two_back = fibo->one_back;
	fibo->one_back = fibo->latest;
}

int main(void) {
	FIB fibo;
	FIB *fibPtr = &fibo;
	bigint sum = 0;

	memset(&fibo, '\0', sizeof(fibo));
	fibPtr->one_back = 1;

	for (fibPtr->latest = 0; fibPtr->latest <= MAX_VAL ; ) {
		nextFibonacci(fibPtr);
		if ( (fibPtr->latest % 2) == 0)
			sum += fibPtr->latest;
	}

	printf("Sum of even fibonacci numbers under %d is %llu\n", MAX_VAL, sum);

	return 0;
}
