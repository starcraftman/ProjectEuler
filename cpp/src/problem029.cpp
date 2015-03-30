/**
Find all distinct numbers of form: a**b for:
2 <= a, b <= 100
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <set>
#include "gtest/gtest.h"
#include "boost/multiprecision/cpp_int.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
typedef boost::multiprecision::cpp_int big_int;

/************** Global Vars & Functions *******************/
big_int pow(big_int &base, int exp) {
    if (exp == 1) {
        return base;
    } else if ((exp % 2) == 0) {
        big_int temp = pow(base, exp/2);
        return temp * temp;
    } else {
        big_int temp = pow(base, (exp - 1)/2);
        return temp * temp * base;
    }
}

TEST(Euler029, TestPowerFunc) {
    big_int base(2);

    ASSERT_EQ(2, pow(base, 1));
    ASSERT_EQ(4, pow(base, 2));
    ASSERT_EQ(8, pow(base, 3));
    ASSERT_EQ(16, pow(base, 4));
    ASSERT_EQ(2048, pow(base, 11));
    ASSERT_EQ(2048 * 8, pow(base, 14));
}

TEST(Euler029, DistinctSequence) {
    std::set<big_int> collect;

    for (int i = 2; i <= 100; ++i) {
        big_int base(i);
        for (int j = 2; j <= 100; ++j) {
            collect.insert(pow(base, j));
        }
    }

    ASSERT_EQ(9183, collect.size());
    cout << "The number of terms is: " << collect.size() << endl;
}

