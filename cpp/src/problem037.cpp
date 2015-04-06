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
#include <set>
#include <algorithm>
#include <utility>

#include "boost/assign.hpp"
#include "boost/lambda/lambda.hpp"
#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_int;

/****************** Class Definitions *********************/
class Truncatables {
public:
    Truncatables(u_int max) : max(max) {
        primes = util::simple_sieve(max);
    };

    bool is_prime(u_int num) {
        return std::binary_search(primes.begin(), primes.end(), num);
    }

    bool is_truncatable_prime(u_int num) {
        u_int digits = count_places(num) - 1;
        u_int reversed = util::reverse(num);
        u_int divisor = 10;
        u_int left_trunc = 0;

        while (digits-- != 0) {
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

        for (std::vector<u_int>::const_iterator i = primes.begin();
                i != primes.end(); ++i) {
            if (is_truncatable_prime(*i)) {
                if (*i > 10) {
                    result.push_back(*i);
                }
            }
        }

        return result;
    }
private:
    u_int count_places(u_int num) {
        u_int count = 0;
        while(num != 0) {
            count++;
            num /= 10;
        }

        return count;
    }

    u_int max;
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

