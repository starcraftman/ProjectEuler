/**
By listing the first six prime numbers:
2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
TEST(Euler007, FinalAnswer) {
	std::vector<int> primes = util::sieve_erat(1000000);
	int prime = primes[10001-1];

	cout << "The prime is: " << prime << endl;

	ASSERT_EQ(104743, prime);
}

