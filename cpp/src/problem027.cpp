/**
Euler discovered the remarkable quadratic formula:

n² + n + 41

It turns out that the formula will produce 40 primes for the consecutive values
n = 0 to 39. However, when n = 40, 402 + 40 + 41 = 40(40 + 1) + 41 is divisible
by 41, and certainly when n = 41, 41² + 41 + 41 is clearly divisible by 41.

The incredible formula  n² − 79n + 1601 was discovered, which produces 80 primes
for the consecutive values n = 0 to 79.
The product of the coefficients, −79 and 1601, is −126479.

Considering quadratics of the form:

    n² + an + b, where |a| < 1000 and |b| < 1000

    where |n| is the modulus/absolute value of n
    e.g. |11| = 11 and |−4| = 4

Find the product of the coefficients, a and b, for the quadratic expression that
produces the maximum number of primes for consecutive values of n,
starting with n = 0.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <algorithm>
#include <climits>

#include <gtest/gtest.h>
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/***************** Constants & Macros *********************/
// Worst case: 1000^2 + 1000 * 1000 + 1000
static const util::u_long PRIME_MAX = 2001000;
static const std::vector<util::u_long> primes = util::sieve_erat(PRIME_MAX);

/****************** Class Definitions *********************/
class Result {
public:
    Result() : a(0), b(0), most_primes(0) {};
    void print() {
        cout << "The values are: a " << a << " b " << b << " their product is: "
            << a * b << endl;
        cout << "They produced " << most_primes << " consecutive primes." << endl;
    }
    int a, b, most_primes;
};

/************** Global Vars & Functions *******************/
util::u_long euler_val(int n, int a, int b) {
    return (n * n) + (a * n) + b;
}

int find_consec_primes(int a, int b) {
    int count = 0, n = 0;
    util::u_long euler = 0;

    euler = euler_val(n, a, b);
    while(std::binary_search(primes.begin(), primes.end(), euler)) {
        count++;
        n++;
        euler = euler_val(n, a, b);
    }

    return count;
}

TEST(Euler027, EulerGeneration) {
    ASSERT_EQ(41, euler_val(0, 0, 41));
    ASSERT_EQ(43, euler_val(1, 1, 41));
}

TEST(Euler027, EulerConsecutives) {
    ASSERT_EQ(80, find_consec_primes(-79, 1601));
}

TEST(Euler027, FinalAnswer) {
    Result res;
    int num_primes = 0;
    for (int a = -999; a < 1000; ++a) {
        for (int b = -999; b < 1000; ++b) {
            num_primes = find_consec_primes(a, b);
            if (num_primes > res.most_primes) {
                res.most_primes = num_primes;
                res.a = a;
                res.b = b;
                res.print();
            }
        }
    }
}

