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

/************** Global Vars & Functions *******************/
std::vector<int> generate_prime_list(int min, int max) {
    std::vector<int> primes = util::sieve_erat(max);

    auto itr = primes.begin();
    while (*itr < min) {
        ++itr;
    }
    primes.erase(primes.begin(), itr);

    return primes;
}

// Simple container class to hold a set of primes in vector and set form.
// Mainly used for prime lookup since I know I have a small set of primes to check.
class PrimeC {
public:
    PrimeC(int max, int min = 0) {
        auto prime_vec = generate_prime_list(min, max);
        prime_set.insert(prime_vec.begin(), prime_vec.end());
    }
    inline
    bool is_prime(int num) { return prime_set.find(num) != prime_set.end(); }

    // Data
    std::set<int> prime_set;
};

// Represents a growing window of primes, simply keeps a sum and length.
// On addition tells caller if the new sum is prime.
class Window {
public:
    Window(PrimeC &primes) : primes(primes) {};
    bool add(int num) {
        nums.push_back(num);
        sum += num;
        return primes.is_prime(sum);
    };
    std::size_t size() {
        return nums.size();
    }

    // Data
    PrimeC &primes;
    std::vector<int> nums;
    int sum = 0;
};

int search_primes(int max, std::size_t window_max=1000) {
    PrimeC primes(max);
    std::vector<int> best;
    int best_sum = 0;

    for (int start : primes.prime_set) {
        Window window(primes);
        for (int next : primes.prime_set) {
            if (next >= start) {
                // Reasonable assumption that run can't go on forever, lowers search time.
                if (window.size() > window_max) {
                    break;
                }

                if (window.add(next) && window.size() > best.size()) {
                    best.clear();
                    best.insert(best.end(), window.nums.begin(), window.nums.end());
                    best_sum = window.sum;
                }
            }
        }
    }

    cout << "Sum is: " << best_sum << " length is: " << best.size() << endl;
    for (auto e : best) {
        cout << e << " ";
    }
    cout << endl;

    return best_sum;
}

TEST(Euler050, GenPrimeList) {
    std::vector<int> primes = generate_prime_list(1000, 10000);
    ASSERT_EQ(primes.front(), 1009);
    ASSERT_EQ(primes.back(), 9973);
}

TEST(Euler050, IsPrime) {
    PrimeC primes(10000);
    ASSERT_TRUE(primes.is_prime(9973));
    ASSERT_FALSE(primes.is_prime(10));
}

TEST(Euler050, WindowAdd) {
    PrimeC primes(10000);
    Window window(primes);

    ASSERT_TRUE(window.add(3));
    ASSERT_EQ(window.sum, 3);
    ASSERT_EQ(window.size(), 1);
}

TEST(Euler050, Search100) {
    ASSERT_EQ(search_primes(100), 41);
}

TEST(Euler050, Search1000) {
    ASSERT_EQ(search_primes(1000), 953);
}

TEST(Euler050, FinalAnswer) {
    int prime_sum = search_primes(1000000);
    cout << "The prime with longest sum: " << prime_sum << endl;
    ASSERT_EQ(prime_sum, 997651);
}
