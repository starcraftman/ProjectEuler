/**
A googol (10100) is a massive number: one followed by one-hundred zeros; 100100 is almost unimaginably
large: one followed by two-hundred zeros. Despite their size, the sum of the digits in each number is only 1.

Considering natural numbers of the form, ab, where a, b < 100, what is the maximum digital sum?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <numeric>
#include <limits>
#include <cmath>

#include "gtest/gtest.h"
#include "util.hpp"
#include "gmpxx.h"
#include "gmp.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
int count_digits(mpz_class &number) {
    std::string digits = number.get_str();
    return std::distance(digits.begin(), digits.end());
}

int count_positive_powers() {
    int count = 0;
    mpz_class number = 0;
    mpz_class base = 0;

    while (++base < 1000) {
        int expo = 0;
        while (++expo < 1000) {
            mpz_pow_ui(number.get_mpz_t(), base.get_mpz_t(), expo);
            int num_digits = count_digits(number);

            if (num_digits == expo) {
                cout << base.get_str() << " " << expo << " = " << number.get_str() << endl;
                count++;
            }
            if (num_digits > expo) {
                break;
            }
        }
    }

    return count;
}

TEST(Euler063, CountDigits) {
    mpz_class digits = 43152;
    ASSERT_EQ(count_digits(digits), 5);
}


TEST(Euler063, FinalSolution) {
    int result = count_positive_powers();
    ASSERT_EQ(result, 49);
    cout << "The number of n digit numbers of form a^n is: " << result << endl;
}
