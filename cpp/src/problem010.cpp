/**
The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.
*/
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <algorithm>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_long;

/************** Global Vars & Functions *******************/
TEST(Euler010, FinalAnswer) {
	u_long sum = 0;
	std::vector<u_long> primes = util::simple_sieve(2000000UL);
	for (std::vector<u_long>::const_iterator i = primes.begin();
			i != primes.end(); ++i) {
		sum += *i;
	}

	cout << "Sum of primes under 2 million " << sum << endl;
	ASSERT_EQ(142913828922, sum);
}

