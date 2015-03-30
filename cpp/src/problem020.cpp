/**
n! means n × (n − 1) × ... × 3 × 2 × 1

For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!
 */
/********************* Header Files ***********************/
#include <iostream> /* Input/output objects. */
#include <sstream>
#include <string> /* C++ String class. */
#include <cstdlib>

#include "boost/multiprecision/cpp_int.hpp"
#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;

/************** Global Vars & Functions *******************/
long sum_digits(std::string num) {
    long sum = 0;

    for (std::string::const_iterator itr = num.begin(); itr != num.end(); ++itr) {
        std::string temp;
        temp += *itr;
        sum += std::atoi(temp.c_str());
    }

    return sum;
}

TEST(Euler020, SumDigits) {
    string num("12345");
    ASSERT_EQ(15, sum_digits(num));
}

TEST(Euler020, FinalAnswer) {
    int multiplier = 100;
    boost::multiprecision::cpp_int factorial(1);

    while (multiplier != 1) {
        factorial *= multiplier--;
    }

    std::stringstream ss;
    ss << factorial;

    int sum = sum_digits(ss.str());
    cout << "The factorial is: " << endl << factorial << endl
        << "The sum of digits is: " << sum << endl;
    ASSERT_EQ(648, sum);
}

