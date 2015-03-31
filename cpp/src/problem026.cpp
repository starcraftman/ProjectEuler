/**
A unit fraction contains 1 in the numerator. The decimal representation
of the unit fractions with denominators 2 to 10 are given:

    1/2 =   0.5
    1/3 =   0.(3)
    1/4 =   0.25
    1/5 =   0.2
    1/6 =   0.1(6)
    1/7 =   0.(142857)
    1/8 =   0.125
    1/9 =   0.(1)
    1/10    =   0.1

Where 0.1(6) means 0.166666..., and has a 1-digit recurring cycle.
It can be seen that 1/7 has a 6-digit recurring cycle.

Find the value of d < 1000 for which 1/d contains the longest recurring
cycle in its decimal fraction part.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
#include <algorithm>
#include <utility>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;
using util::u_int;

/****************** Class Definitions *********************/
class DivResult {
public:
    DivResult() : quotient("empty"), is_cycle(false) {};
    DivResult(std::string quotient, bool cycle) :
        quotient(quotient), is_cycle(cycle) {};

    std::string quotient;
    bool is_cycle;
};

/************** Global Vars & Functions *******************/
// Returns "" if no cycle, else the cycle's longest chain
string cycle_check(string num) {
    string longest;
    std::string::size_type sub_len = 0;

    while (sub_len++ != (num.size() / 2)) {
        std::string::size_type start = -1;
        while (start++ != (num.size() - sub_len - 1)) {
            std::string first = num.substr(start, sub_len);
            std::string second = num.substr(start + sub_len, sub_len);
            if (first == second) {
                longest = first;
                break;
            }
        }

        if (longest.size() > 0) {
            break;
        }
    }

    return longest;
}

string cycle_check(const char * const num) {
    std::string num_s(num);
    return cycle_check(num_s);
}

DivResult divide(u_int dividend, u_int divisor) {
    std::stringstream quotient;
    u_int max_digits = 1000;

    while (dividend != 0 && max_digits-- != 0) {
        while (dividend < divisor) {
            dividend *= 10;
        }

        u_int times = dividend / divisor;
        quotient << times;
        dividend -= times * divisor;

        std::string cycle = cycle_check(quotient.str());
        if (cycle.length() != 0) {
            return DivResult(cycle, true);
        }
    }

    return DivResult(quotient.str(), false);
}

TEST(Euler026, CheckNoCycle) {
    ASSERT_STREQ("", cycle_check("").c_str());
    ASSERT_STREQ("", cycle_check("125").c_str());
}

TEST(Euler026, CheckOneDigitCycle) {
    ASSERT_STREQ("", cycle_check("6").c_str());
    ASSERT_STREQ("6", cycle_check("66").c_str());
    ASSERT_STREQ("6", cycle_check("666").c_str());
    ASSERT_STREQ("6", cycle_check("6666").c_str());
    ASSERT_STREQ("6", cycle_check("66666").c_str());
}

TEST(Euler026, CheckMoreDigitCycles) {
    ASSERT_STREQ("", cycle_check("1").c_str());
    ASSERT_STREQ("", cycle_check("14").c_str());
    ASSERT_STREQ("", cycle_check("142").c_str());
    ASSERT_STREQ("", cycle_check("1428").c_str());
    ASSERT_STREQ("", cycle_check("14285").c_str());
    ASSERT_STREQ("", cycle_check("142857").c_str());
    ASSERT_STREQ("", cycle_check("1428571").c_str());
    ASSERT_STREQ("", cycle_check("14285714").c_str());
    ASSERT_STREQ("", cycle_check("142857142").c_str());
    ASSERT_STREQ("", cycle_check("1428571428").c_str());
    ASSERT_STREQ("", cycle_check("14285714285").c_str());
    ASSERT_STREQ("142857", cycle_check("142857142857").c_str());
    ASSERT_STREQ("142857", cycle_check("1428571428571").c_str());
    ASSERT_STREQ("142857", cycle_check("14285714285714").c_str());
}

TEST(Euler026, CycleAfterFirstDigit) {
    ASSERT_STREQ("6", cycle_check("41666").c_str());
}

TEST(Euler026, DivideNoCycles) {
    DivResult res = divide(1, 2);
    ASSERT_STREQ("5", res.quotient.c_str());
    ASSERT_FALSE(res.is_cycle);

    res = divide(1, 8);
    ASSERT_STREQ("125", res.quotient.c_str());
    ASSERT_FALSE(res.is_cycle);
}

TEST(Euler026, DivideCycles) {
    DivResult res = divide(1, 3);
    ASSERT_STREQ("3", res.quotient.c_str());
    ASSERT_TRUE(res.is_cycle);

    res = divide(1, 7);
    ASSERT_STREQ("142857", res.quotient.c_str());
    ASSERT_TRUE(res.is_cycle);
}

TEST(Euler026, FinalAnswer) {
    DivResult res, longest;
    int divisor = 0;
    for (int i = 2; i < 1000; ++i) {
        cout << 1 << " / " << i << endl;
        res = divide(1, i);
        cout << "Result: " << res.is_cycle << " " << res.quotient << endl;
        if (res.is_cycle && res.quotient.length() > longest.quotient.length()) {
            longest.quotient = res.quotient;
            divisor = i;
        }
    }

    cout << "The longest cycle is: " << longest.quotient << endl;
    cout << "The divisor was: " << divisor << endl;
}

