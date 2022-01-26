/**
The number 145 is well known for the property that the sum of the factorial of its digits is equal to 145:

1! + 4! + 5! = 1 + 24 + 120 = 145

Perhaps less well known is 169, in that it produces the longest chain of numbers that link back to 169; it turns out that there are only three such loops that exist:

169 → 363601 → 1454 → 169
871 → 45361 → 871
872 → 45362 → 872

It is not difficult to prove that EVERY starting number will eventually get stuck in a loop. For example,

69 → 363600 → 1454 → 169 → 363601 (→ 1454)
78 → 45360 → 871 → 45361 (→ 871)
540 → 145 (→ 145)

Starting with 69 produces a chain of five non-repeating terms, but the longest non-repeating chain with a starting number below one million is sixty terms.

How many chains, with a starting number below one million, contain exactly sixty non-repeating terms?
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
std::map<num_t, num_t> fact_cache;

inline
num_t factorial(num_t num) {
    if (num <= 1) {
        return 1;
    } else if (fact_cache[num]) {
        return fact_cache[num];
    }

    int copy_num = num;
    num_t answer = 1;
    while (num != 1) {
        answer *= num--;
    }

    fact_cache[copy_num] = answer;
    return answer;
}

num_t digit_factorial_sum(num_t num) {
    num_t sum = 0;
    while (num != 0) {
        sum += factorial(num % 10);
        num /= 10;
    }

    return sum;
}

// Given a number, count how many non-repeating numbers are part of the chain.
num_t count_digit_cycles(num_t num) {
    std::set<num_t> seen;
    seen.insert(num);
    num_t last_size = seen.size();

    do {
        num_t next = digit_factorial_sum(num);;
        seen.insert(next);
        if (last_size == seen.size()) {
            break;
        }
        num = next;
        last_size = seen.size();
    } while (true);
    // A cycle is guaranteed for every number.

    return seen.size();
}

num_t digital_factorial_chains(num_t max, num_t num_terms) {
    num_t cnt = 0;
    num_t num = 0;

    while (++num != max) {
        num_t num_in_cycle = count_digit_cycles(num);
        if (num_in_cycle == num_terms) {
            cnt++;
        }
    }

    return cnt;
}

TEST(Euler074, Factorial) {
    ASSERT_EQ(factorial(0), 1);
    ASSERT_EQ(factorial(1), 1);
    ASSERT_EQ(factorial(4), 24);
    ASSERT_EQ(factorial(5), 120);
}

TEST(Euler074, DigitFactorial) {
    ASSERT_EQ(digit_factorial_sum(145), 145);
}

TEST(Euler074, CountDigitCycle) {
    ASSERT_EQ(count_digit_cycles(169), 3);
    ASSERT_EQ(count_digit_cycles(69), 5);
}

// TODO: This is a bit slow, any room to improve?
TEST(Euler074, DigitFactorialChains) {
    num_t max = 1'000'000;
    num_t cnt = digital_factorial_chains(max, 60);
    cout << "The number of factorial digit chains starting below 1,000,000 that contain exactly 60 unique terms is " << cnt << endl;
    ASSERT_EQ(cnt, 402);
}
