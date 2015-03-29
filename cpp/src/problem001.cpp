/**
If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
TEST(Euler001, FinalAnswer) {
	int sum = 0;

	for (int i = 3; i < 1000; ++i) {
		if ((i % 3) == 0 || (i % 5) == 0) {
			sum += i;
        }
	}

    cout << "The sum of all numbers up to 1000 divisible by 3 or 5:" <<
        endl << sum << endl;

    ASSERT_EQ(233168, sum);
}

