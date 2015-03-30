/**
The following iterative sequence is defined for the set of positive integers:

n → n/2 (n is even)
n → 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:
13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1

It can be seen that this sequence (starting at 13 and finishing at 1) 
contains 10 terms. Although it has not been proved yet (Collatz Problem),
it is thought that all starting numbers finish at 1.

Which starting number, under one million, produces the longest chain?

NOTE: Once the chain starts the terms are allowed to go above one million.
*/
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
int collatz_len(unsigned long n) {
	// The first number is part of the chain
	int len = 1;

	while (n > 1) {
		if ( (n % 2) == 0) {
			n /= 2;
		} else {
			n = (3 * n) + 1;
		}
		++len;
	}

	return len;
}

TEST(Euler014, CollatzSequence) {
	ASSERT_EQ(10, collatz_len(13));
}

TEST(Euler010, FinalAnswer) {
	int len = 0, longest = 0, longest_start = 0;

	for (int start = 1; start < 1000000; start++) {
		len = collatz_len(start);
		if (len > longest) {
			longest = len;
			longest_start = start;
		}
	}

	cout << "Longest chain starts with " << longest_start
		<< " and has length " << longest << endl;

	ASSERT_EQ(837799, longest_start);
}

