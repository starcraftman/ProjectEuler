/**
The sum of the squares of the first ten natural numbers is,
12 + 22 + ... + 102 = 385

The square of the sum of the first ten natural numbers is,
(1 + 2 + ... + 10)2 = 552 = 3025

Hence the difference between the sum of the squares of the first ten
natural numbers and the square of the sum is 3025 − 385 = 2640.

Find the difference between the sum of the squares of the first one
hundred natural numbers and the square of the sum.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_long;

/************** Global Vars & Functions *******************/
u_long sum_of_squares(u_long limit) {
	u_long  sum = 0;

	for (u_long i = 1; i <= limit; ++i) {
		sum += i * i;
	}

	return sum;
}

u_long square_of_sum(u_long limit) {
	u_long sum = 0;

	for (u_long i = 0; i <= limit; ++i) {
		sum += i;
	}

	sum *= sum;

	return sum;
}

TEST(Euler006, SumOfSquares) {
	ASSERT_EQ(385, sum_of_squares(10));
}

TEST(Euler006, SquareOfSum) {
	ASSERT_EQ(3025, square_of_sum(10));
}

TEST(Euler006, FinalAnswer) {
	u_long difference = square_of_sum(100) - sum_of_squares(100);

	cout << "The difference is: " << difference << endl;

	ASSERT_EQ(25164150, difference);
}

