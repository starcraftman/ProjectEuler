/**
A number chain is created by continuously adding the square of the digits in a number to form a new number until it has been seen before.

For example,

44 → 32 → 13 → 10 → 1 → 1
85 → 89 → 145 → 42 → 20 → 4 → 16 → 37 → 58 → 89

Therefore any chain that arrives at 1 or 89 will become stuck in an endless loop. What is most amazing is that EVERY starting number will eventually arrive at 1 or 89.

How many starting numbers below ten million will arrive at 89?
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
#include <algorithm>
#include <numeric>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
typedef std::uint64_t num_t;
std::map<num_t, num_t> cache = {
    {1, 1},
    {89, 89},
};

inline
num_t square_digits(num_t num) {
    num_t new_num = 0;

    while (num != 0) {
        num_t temp = num % 10;
        new_num += temp * temp;
        num /= 10;
    }

    return new_num;
}

num_t search_square_numbers(num_t max) {
    num_t cnt_89 = 0;

    num_t num = 1; // 1 is precomputed
    while (++num != max) {
        num_t copy = num;
        while (copy != 1 && copy != 89) {
            // Already solved
            if (cache[copy] != 0) {
                copy = cache[copy];
                break;
            }
            copy = square_digits(copy);
        }

        cache[num] = copy;
        if (copy == 89) {
            ++cnt_89;
        }
    }

    return cnt_89;
}

TEST(Euler092, SquareDigits) {
    ASSERT_EQ(square_digits(89), 145);
}

TEST(Euler092, DigitsTo89) {
    num_t max = 10'000'000;
    num_t cnt = search_square_numbers(max);
    cout << "Number of square chains leading to 89 under ten million: " << cnt << endl;
    ASSERT_EQ(cnt, 8'581'146);
}
