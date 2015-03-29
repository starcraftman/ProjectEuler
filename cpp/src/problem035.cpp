/**
The number, 197, is called a circular prime because all rotations of the
digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100:
2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

How many circular primes are there below one million?
 */
/********************* header files ***********************/
/* c++ headers */
#include <iostream> /* input/output objects. */
#include <algorithm>

#include <gtest/gtest.h>
#include "util.hpp"

/**************** namespace declarations ******************/
using std::cout;
using std::endl;

/***************** constants & macros *********************/
static const int MAX_PRIME = 1000000;
static const std::vector<int> primes = util::simple_sieve(MAX_PRIME);

/************** global vars & functions *******************/
bool is_prime(int num) {
    return std::binary_search(primes.begin(), primes.end(), num);
}

int rotate(int num) {
    int right = num % 10, rem = num / 10, places = -1;

    while (num != 0) {
        places += 1;
        num /= 10;
    }

    return right * util::pow(10, places) + rem;
}

bool is_circular_prime(int num) {
    if (!is_prime(num)) {
        return false;
    }

    int rotated = rotate(num);
    while (rotated != num) {
        if (!is_prime(rotated)) {
            return false;
        }
        rotated = rotate(rotated);
    }

    return true;
}

TEST(Euler035, RotateNumber) {
    int num = 197, rotated = 0;

    rotated = rotate(num);
    ASSERT_EQ(719, rotated);
    rotated = rotate(rotated);
    ASSERT_EQ(971, rotated);
}

TEST(Euler035, IsCircular) {
    ASSERT_TRUE(is_circular_prime(197));
    ASSERT_TRUE(is_circular_prime(17));
    ASSERT_FALSE(is_circular_prime(76));
}

TEST(Euler035, FinalAnswer) {
    int count = 0;
    for (int i = 0; i < MAX_PRIME; ++i) {
        if (is_circular_prime(i)) {
            count += 1;
        }
    }
    ASSERT_EQ(55, count);

    cout << "The number of circular primes under "
        << MAX_PRIME << " is: " << count << endl;
}

