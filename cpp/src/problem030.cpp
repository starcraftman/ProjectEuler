/**
Surprisingly there are only three numbers that can be written as the sum of
fourth powers of their digits:

    1634 = 14 + 64 + 34 + 44
    8208 = 84 + 24 + 04 + 84
    9474 = 94 + 44 + 74 + 44

As 1 = 14 is not a sum it is not included.

The sum of these numbers is 1634 + 8208 + 9474 = 19316.

Find the sum of all the numbers that can be written as the sum of fifth
powers of their digits.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <gtest/gtest.h>

#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
util::u_int sum_fifth_powers(util::u_int num) {
    util::u_int total = 0, digit = 0;

    while (num != 0) {
        digit = num % 10;
        num /= 10;
        total += util::pow(digit, 5);
    }

    return total;
}

TEST(Euler030, SumFifthPowers) {
    ASSERT_EQ(4425, sum_fifth_powers(12345));
}

TEST(Euler030, FinalAnswer) {
    u_int sum = 0, up_bound = 5 * util::pow(9, 5);

    for (util::u_int i = 2; i < up_bound; ++i) {
        if (sum_fifth_powers(i) == i) {
            sum += i;
            cout << "Found number " << i << endl;
            cout << "Current sum " << sum << endl;
        }
    }

    cout << "Sum of the numbers " << sum << endl;
}

