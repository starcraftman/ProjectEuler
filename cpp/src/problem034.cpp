/**
145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum
of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include <gtest/gtest.h>
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_long;

/************** Global Vars & Functions *******************/
u_long factorial(u_long num) {
    u_long factorial = 1;

    if (num < 2) {
        return 1;
    }

    while (num != 1) {
        factorial *= num;
        num -= 1;
    }

    return factorial;
}

u_long sum_digit_factorials(u_long num) {
    u_long sum = 0;

    while (num != 0) {
        sum += factorial(num % 10);
        num /= 10;
    }

    return sum;
}

TEST(Euler034, FactorialMaker) {
    ASSERT_EQ(120, factorial(5));
}

TEST(Euler034, SumDigitFactorials) {
    ASSERT_EQ(145, sum_digit_factorials(145));
}

TEST(Euler034, FinalAnswer) {
    u_long sum = 0, max = factorial(9) * 5;

    for (u_long i = 3; i != max; ++i) {
        if (sum_digit_factorials(i) == i) {
            sum += i;
            cout << "Number match: " << i << endl;
        }
    }

    ASSERT_EQ(40730, sum);
    cout << "The sum is: " << sum << endl;
}

