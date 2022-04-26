/**
The prime 41, can be written as the sum of six consecutive primes:

    41 = 2 + 3 + 5 + 7 + 11 + 13

This is the longest sum of consecutive primes that adds to a prime below one-hundred.

The longest sum of consecutive primes below one-thousand that adds to a prime, contains 21 terms, and is equal to 953.

Which prime, below one-million, can be written as the sum of the most consecutive primes?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
typedef std::vector<std::vector<long> > vec_vec_t;
typedef std::vector<long> vec_t;

/************** Global Vars & Functions *******************/
/**
 * @brief Pass in a vector of numbers, determine they all have same digits.
 *
 * 1) Break the numbers into vectors of digits.
 * 2) While doing so, abort early if any number is of different length than first.
 * 3) Sort digits into order on completion.
 * 4) For all digits in list, move through them 1 digit at a time ensuring all digits identical.
 *
 * @param numbers A vector of numbers of any length.
 * @return True if all numbers have the exact same digits.
 */
bool digits_same(const vec_t &numbers) {
    bool first_number = true;
    vec_vec_t list_digits;
    std::size_t first_size = 0;

    for (auto number : numbers) {
        vec_t digits;
        while (number != 0) {
            digits.push_back(number % 10);
            number /= 10;
        };

        if (first_number) {
            first_size = digits.size();
            first_number = false;
        } else if (digits.size() != first_size) {
            return false;  // Two numbers have different amount of digits.
        }

        std::sort(digits.begin(), digits.end());
        list_digits.push_back(digits);
    }

    // All vectors of digits same length and sorted, check all digits same order too.
    for (std::size_t i = 0; i < first_size; ++i) {
        auto left = list_digits.begin();
        auto right = list_digits.begin();
        right++;

        // Pointing at every pair of digits vectors, all same size.
        while (right != list_digits.end()) {
            if (left->at(i) != right->at(i)) {
                return false;
            }

            right++;
            left++;
        }
    }

    return true;
}

long search_digits(long max) {
    for (long num = 10; num < max; ++num) {
        vec_t numbers = {num * 1, num * 2, num * 3, num * 4, num * 5, num * 6};
        if (digits_same(numbers)) {
            cout << "The found multiples of " << num << endl << "    ";
            for (auto num : numbers) {
                cout << num << " ";
            }
            cout << endl;
            return num;
        }
    }
}

TEST(Euler052, DigitsSame) {
    vec_t numbers = {327, 372, 273};
    ASSERT_TRUE(digits_same(numbers));

    vec_t numbers2 = {32, 372, 273};
    ASSERT_FALSE(digits_same(numbers2));
}

TEST(Euler052, FinalAnswer) {
    long result = search_digits(1000000);
    cout << "The smallest digit with same digits 1x - 6x is " << result << endl;
    ASSERT_EQ(result, 142857);
}
