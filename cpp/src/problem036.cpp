/**
The decimal number, 585 = 10010010012 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million,
which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base,
may not include leading zeros.)
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
#include <algorithm>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;
using util::u_int;

/************** Global Vars & Functions *******************/
bool is_palindrome(std::string &word) {
    // For this problem. leading zeroes don't count
    if (word.at(0) == '0' || word.at(word.length()-1) == '0') {
        return false;
    }

    std::string::const_iterator front = word.begin(), end = word.end()-1;
    while (front < end) {
        if (*front != *end) {
            return false;
        }
        front++;
        end--;
    }

    return true;
}

bool is_palindrome(const char * const word) {
    std::string str_word(word);
    return is_palindrome(str_word);
}

std::string to_base(u_int val, u_int base) {
    std::string new_val;
    while (val != 0) {
        new_val = char('0' + (val % base)) + dec;
        val /= base;
    }

    return new_val;
}

bool check_both_bases(u_int val) {
    if ((val % 10) == 0) {
        return false;
    }

    std::string dec = to_base(val, 10);
    if (!is_palindrome(dec)) {
        return false;
    }

    std::string bin = to_base(val, 2);
    return is_palindrome(bin);
}

TEST(Euler036, IsPalindrome) {
    ASSERT_TRUE(is_palindrome("1001"));
    ASSERT_TRUE(is_palindrome("34443"));
    ASSERT_FALSE(is_palindrome("0330"));
}

TEST(Euler036, Conversions) {
    ASSERT_STREQ("1234", to_base(1234, 10).c_str());
    ASSERT_STREQ("1010", to_base(10, 2).c_str());
}

TEST(Euler036, FinalAnswer) {
    u_int sum = 0;

    for (u_int i = 1; i < 1000000; ++i) {
        if (check_both_bases(i)) {
            sum += i;
        }
    }

    cout << "Sum is: " << sum << endl;
    ASSERT_EQ(872187, sum);
}

