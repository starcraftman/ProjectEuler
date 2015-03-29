/**
A palindromic number reads the same both ways. The largest palindrome made
from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
int reverse(int num) {
	int reversed = 0;

	while (num > 0) {
		reversed *= 10;
		reversed += num % 10;
		num /= 10;
	}

    return reversed;
}

bool is_palindrome(int num) {
    return reverse(num) == num;
}

TEST(Euler004, ReverseNum) {
    int num = 906609;

    ASSERT_EQ(num, reverse(num));
}

TEST(Euler004, FinalAnswer) {
    int palindrome = 0, min = 100, max = 1000;

    for (int i = min; i < max; ++i) {
        for (int j = min; j < max; ++j) {
            int product = i * j;
            if (is_palindrome(product) && product > palindrome) {
                palindrome = product;
            }
        }
    }

    cout << "The largest palindrome made of two 3 digit numbers is: "
        << palindrome << endl;

    ASSERT_EQ(906609, palindrome);
}

