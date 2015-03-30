/**
 Problem Summary:
Perfect Number: Sum of divisors = number
i.e. 28 = 1 + 2 + 4 + 7 + 14

Abundants Number: Sum of divisors > number
i.e. 1 + 2 + 3 + 4 + 5 = 16 (16 > 12)

All numbers >= 28124 can be written as sum of 2 abundant numbers.

Q: Find sum of positive numbers that cannot be written as sum of two abundant numbers.
Steps:
- Find all abundants < MAX
- Make function test if sum of two abundants possible.
- Iterate numbers from 1..MAX, store numbers that fail to sum.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>
#include <set> // multiset for multiple keys allowed.
#include <algorithm>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_int;
using util::u_long;

/******************* Type Definitions *********************/
typedef std::vector<u_int> abs_t;
u_int sum_divisors(u_int);

/***************** Constants & Macros *********************/
const u_int MAX_ABUNDANT = 28124;

/****************** Class Definitions *********************/
class Abundants {
public:
    Abundants(u_int max): max(max) {};

    void find_abundants(void) {
        for (u_int i = 1; i < max; ++i) {
            if (sum_divisors(i) > i) {
                abundants.push_back(i);
            }
        }
    }

    void find_sum_two_abundants(void) {
        int offset = 0;
        for (abs_t::const_iterator i = abundants.begin(); i != abundants.end(); ++i) {
            if (*i >= max) {
                break;
            }

            for (abs_t::const_iterator j = abundants.begin() + offset; j != abundants.end(); ++j) {
                if ((*i +*j) > max) {
                    break;
                }

                sum_of_two_abundants.insert(*i + *j);
            }

            offset++;
        }
    }

    u_long collect_difference(void) {
        std::vector<u_int> all;
        for (u_int i = 1; i < max; ++i) {
            all.push_back(i);
        }

        std::vector<u_int> diff;
        std::set_difference(all.begin(), all.end(), sum_of_two_abundants.begin(),
                sum_of_two_abundants.end(), std::back_inserter(diff));

        u_long sum = 0;
        for (std::vector<u_int>::const_iterator itr = diff.begin(); itr != diff.end(); ++itr) {
            sum += *itr;
        }

        return sum;
    }

    bool is_abundant(u_int val) {
        return std::binary_search(abundants.begin(), abundants.end(), val);
    }

    void trace() {
        cout << "Number of abundants: " << abundants.size() << endl;
        int cnt = 0;
        for (abs_t::const_iterator i = abundants.begin(); i != abundants.end(); ++i) {
            cout << *i << ", ";
            if (++cnt == 15) {
                cout << endl;
                cnt = 0;
            }
        }
        cout << endl;

        cout << "Sum of two abundants:" << endl;
        for (std::set<u_int>::const_iterator i = sum_of_two_abundants.begin(); i != sum_of_two_abundants.end(); ++i) {
            cout << *i << endl;
        }
    }

private:
    u_int max;
    abs_t abundants;
    std::set<u_int> sum_of_two_abundants;
};

/************** Global Vars & Functions *******************/
u_int sum_divisors(u_int dividend) {
    u_int sum = 0;

    std::vector<u_int> divs = util::find_divisors(dividend, true);
    for (std::vector<u_int>::const_iterator i = divs.begin(); i != divs.end(); ++i) {
        sum += *i;
    }

    return sum;
}

TEST(Euler023, SumDivisors) {
    ASSERT_EQ(284, sum_divisors(220));
    ASSERT_EQ(220, sum_divisors(284));
}

TEST(Euler023, FindFirstAbundants) {
    Abundants abs(13);
    abs.find_abundants();

    ASSERT_FALSE(abs.is_abundant(1));
    ASSERT_TRUE(abs.is_abundant(12));
}

TEST(Euler023, CheckCollection) {
    Abundants abs(50);
    abs.find_abundants();
    abs.find_sum_two_abundants();

    ASSERT_EQ(891, abs.collect_difference());
}

TEST(Euler023, FinalAnswer) {
    Abundants abs(MAX_ABUNDANT);
    abs.find_abundants();
    abs.find_sum_two_abundants();

    unsigned long sum = abs.collect_difference();
    cout << "The sum was: " << sum << endl;
    ASSERT_EQ(4179871, sum);
}

