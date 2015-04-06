/**
The number 3797 has an interesting property. Being prime itself, it is possible
to continuously remove digits from left to right, and remain prime at each
stage: 3797, 797, 97, and 7. Similarly we can work from right to left: 3797,
379, 37, and 3.

Find the sum of the only eleven primes that are both truncatable from left to
right and right to left.

NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>
#include <algorithm>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_int;

/****************** Class Definitions *********************/
class Truncatables {
public:
    Truncatables(u_int max) {
        primes = util::simple_sieve(max);
    };

    bool is_prime(u_int num) {
        return std::binary_search(primes.begin(), primes.end(), num);
    }
    bool is_truncatable_prime(u_int num) {
        u_int reversed = util::reverse(num);
        u_int divisor = 10;
        u_int left_trunc = 0;

        while (divisor < num) {
            u_int right_trunc = num % divisor;
            divisor *= 10;
            if (!is_prime(right_trunc)) {
                return false;
            }

            left_trunc *= 10;
            left_trunc += reversed % 10;
            reversed /= 10;
            if (!is_prime(left_trunc)) {
                return false;
            }
        }

        return true;
    }
    std::vector<u_int> truncatable_primes() {
        std::vector<u_int> result;

        std::vector<u_int>::const_iterator itr = primes.begin();
        while (*(++itr) < 10);
        for (; itr != primes.end(); ++itr) {
            if (is_truncatable_prime(*itr)) {
                result.push_back(*itr);
            }
        }

        return result;
    }
private:
    std::vector<u_int> primes;
};

/************** Global Vars & Functions *******************/
TEST(Euler037, Truncatables) {
    Truncatables trunc(100);
    ASSERT_TRUE(trunc.is_prime(13));
    ASSERT_TRUE(trunc.is_prime(61));
}

TEST(Euler037, TruncatablePrime) {
    Truncatables trunc(10000);
    ASSERT_TRUE(trunc.is_truncatable_prime(3797));
}

TEST(Euler037, FinalAnswer) {
    Truncatables trunc(1000000);
    std::vector<u_int> result = trunc.truncatable_primes();
    u_int sum = 0;

    cout << "The primes: " << endl;
    for (std::vector<u_int>::const_iterator i = result.begin();
            i != result.end(); ++i) {
        sum += *i;
        cout << *i << ", ";
    }
    cout << endl;

    cout << "The sum: " << sum << endl;
    ASSERT_EQ(748317, sum);
}

