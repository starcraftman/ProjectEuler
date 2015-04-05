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
using util::pow;

/************** Global Vars & Functions *******************/
bool is_palindrome(std::string &word) {
    std::string rev(word);
    std::reverse(rev.begin(), rev.end());

    // For this problem. leading zeroes don't count
    if (word.at(0) == '0' || rev.at(0) == '0') {
        return false;
    }

    return word == rev;
}

bool is_palindrome(const char * const word) {
    std::string str_word(word);
    return is_palindrome(str_word);
}

std::string to_dec(u_int val) {
    std::stringstream dec;
    dec << val;
    return dec.str();
}

std::string to_bin(u_int val) {
    std::stringstream bin;

    while (val != 0) {
        bin << val % 2;
        val /= 2;
    }

    std::string rev(bin.str());
    std::reverse(rev.begin(), rev.end());
    return rev;
}

bool check_both_bases(u_int val) {
    std::string dec = to_dec(val);
    std::string bin = to_bin(val);

    return is_palindrome(dec) && is_palindrome(bin);
}

TEST(Euler036, IsPalindrome) {
    ASSERT_TRUE(is_palindrome("1001"));
    ASSERT_TRUE(is_palindrome("34443"));
    ASSERT_FALSE(is_palindrome("0330"));
}

TEST(Euler036, Conversions) {
    ASSERT_STREQ("1234", to_dec(1234).c_str());
    ASSERT_STREQ("1010", to_bin(10).c_str());
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

