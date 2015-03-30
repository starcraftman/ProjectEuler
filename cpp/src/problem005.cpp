/**
2520 is the smallest number that can be divided by each of
the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible
by all of the numbers from 1 to 20?
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
bool divisible_up_to(u_long num, u_long max) {
	for (u_long i = 2; i <= max; ++i) {
		if ((num % i) != 0)
			return false;
	}

	return true;
}

TEST(Euler005, DivisibleByN) {
	ASSERT_FALSE(divisible_up_to(2519, 10));
	ASSERT_TRUE(divisible_up_to(2520, 10));
}

/*
 * Doing by trial is PAINFUL. If I did a more rigorous factorization, I could
 * get it in one product.
 *
 * Instead I'll reason this out.
 * Imagine that any number that is a multiple of 1-20 MUST
 * be a multiple of all primes of 1-20. That is, at least the number is:
 *		2 * 3 * 5 * 7 * 11 * 13 * 17 * 19
 *
 *	We didn't account for composites...
 *		4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20
 *
 * HOWEVER, composites have common factors, like:
 *	4 -> 2 * 2, 8 -> 2 * 2 * 2, 16 -> 2 * 2 * 2 * 2
 *
 *	Looking at this, we can see that since we are multiplying these
 *	we can cancel all but three 2s. We can apply similar logic to rest
 *	of the composites and cancel factors already multiplied earlier!
 *	For instance: 6 -> both 2 & 3 already mutliplied to cancle it.
 *
 *	Composites:
 *	4 -> 2 * (x) (these numbers are dropped from product)
 *	6 -> (2) * (3)
 *	8 -> 2 * (2) * (2)
 *	9 -> 3 * (3)
 *	10 -> 2 * (5)
 *	12 -> 2 * (6)
 *	14 -> 2 * (7)
 *	15 -> (3) * (5)
 *	16 -> 2 * (8)
 *	18 -> 2 * (9)
 *	20 -> 2 * (10)
 *
 * I'm sure I overshot it by some multiple of 2 (cuz I'm being simple/quick)
 * , so I'll use a loop to keep dividing by 2 until I am no longer divisable.
 * Number to divide is:
 * 2 * 3 * 2 * 5 * 7 * 2 * 3 * 2 * 11 * 2 * 13 * 2 * 2 * 17 * 2 * 18 * 2
 */
TEST(Euler005, FinalAnswer) {
	u_long num = 2 * 3 * 2 * 5 * 7 * 2 * 3 * 2 * 11;
	num *= 2 * 13 * 2 * 2 * 17 * 2 * 19 * 2;
	cout << "Starting product is " << num << endl;

	u_long last_num = num;
	while (divisible_up_to(num, 20)) {
		last_num = num;
		num /=2;
	}

	cout << "Num " << last_num << " is divisible by 1-20." << endl;
	ASSERT_EQ(232792560, last_num);
}

