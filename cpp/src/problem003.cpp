/**
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_long;

/************** Global Vars & Functions *******************/
TEST(Euler003, FinalAnswer) {
    u_long num = 600851475143, largest_prime = 0;
    std::vector<u_long> primes = util::find_divisors(num, true);

    for (std::vector<u_long>::const_reverse_iterator i = primes.rend();
            i != primes.rbegin(); ++i) {
        if (util::is_prime(*i)) {
            largest_prime = *i;
            break;
        }
    }

    cout << "The largest prime factor is: " << largest_prime << endl;

    ASSERT_EQ(6857, largest_prime);
}

