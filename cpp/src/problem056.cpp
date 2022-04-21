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

#include "gtest/gtest.h"
#include "util.hpp"
#include "gmpxx.h"
#include "gmp.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
long sum_digits(const std::string number_str) {
    long sum = 0;

    for (auto digit : number_str) {
        sum += int(digit - '0');
    }

    return sum;
}


long largest_digital_sum() {
    long largest_sum = 0;
    mpz_class result = 0;
    mpz_class base = 0;

    while (++base < 100) {
        int expo = 0;

        while (++expo != 100) {
            mpz_pow_ui(result.get_mpz_t(), base.get_mpz_t(), expo);
            long sum = sum_digits(result.get_str());
            if (sum > largest_sum) {
                largest_sum = sum;
                cout << "New largest sum of digits: " << sum  << endl
                    << "    " << base.get_str() << " ^ " << expo <<  " = " << result.get_str() << endl;
            }
        }

        base += 1;
    }

    return largest_sum;
}


TEST(Euler056, FinalSolution) {
    long result = largest_digital_sum();
    cout << "The largest sum is: \n" << result << endl;
    ASSERT_EQ(result, 972);
}
