/**
A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
                    a2 + b2 = c2

For example, 32 + 42 = 9 + 16 = 25 = 52.

There exists exactly one Pythagorean triplet for which a + b + c = 1000.
Find the product abc.
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
bool is_pythagorean_triplet(int a, int b, int c) {
	if (a > c || b > c) {
		return false;
    }

    return (a * a + b * b) == c * c;
}

TEST(Euler009, IsTriplet) {
    ASSERT_TRUE(is_pythagorean_triplet(3, 4, 5));
    ASSERT_FALSE(is_pythagorean_triplet(3, 4, 6));
}

TEST(Euler009, FinalAnswer) {
    // Smallest triplet to start
    int product = 0;
    const int max = 1000;

    for (int a = 1; a < max; ++a) {
        int bmax = max - a;
	for (int b = a; b < bmax; ++b) {
	    for (int c = b + 1; c < bmax; ++c) {
		if (is_pythagorean_triplet(a, b, c) && ((a + b + c) == 1000)) {
		    product = a * b * c;
                    cout << "The triplet " << a << " " << b << " " << c
                        << " has product " << product << endl;
                    break;
		}
	    }
        }
    }

    ASSERT_EQ(31875000, product);
}

