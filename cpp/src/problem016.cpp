/**
2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?
*/
/********************* Header Files ***********************/
#include <iostream> /* Input/output objects. */
#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
#include <cstdlib> /* atof, rand, malloc... */

#include "boost/multiprecision/cpp_int.hpp"
#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
namespace multi = boost::multiprecision;

/************** Global Vars & Functions *******************/
TEST(Euler016, FinalAnswer) {
    multi::cpp_int base(2);

    for (int i = 1; i < 1000; ++i) {
        base *= 2;
    }

    std::stringstream ss;
    ss << base;
    std::string n_str = base.str();

    int sum = 0;
    for (std::string::const_iterator itr = n_str.begin(); itr != n_str.end(); ++itr) {
        char val = *itr;
        sum += std::atoi(&val);
    }

    cout << "Result of 2^1000 is : " << endl << n_str << endl << endl;
    cout << "Sum of those digits is: " << sum << endl;
    ASSERT_EQ(1366, sum);
}

