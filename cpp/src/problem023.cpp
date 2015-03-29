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

/* STL Headers */
#include <vector>
#include <set> // multiset for multiple keys allowed.
#include <algorithm>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
typedef unsigned int u_int;
typedef std::vector<u_int> abs_t;

u_int sum_divisors(u_int);

/***************** Constants & Macros *********************/
const u_int MAX_ABUNDANT = 28124;

/****************** Class Definitions *********************/
class Abundants {
public:
    Abundants(u_int max): max(max) {};

    void populate(void) {
        for (u_int i = 1; i < max; ++i) {
            if (sum_divisors(i) > i) {
                abundants.push_back(i);
            }
        }
    }

    void populate_sums(void) {
        for (abs_t::const_iterator i = abundants.begin(); i != abundants.end(); ++i) {
            if (*i >= max) {
                break;
            }

            for (abs_t::const_iterator j = abundants.begin(); j != abundants.end(); ++j) {
                if ((*i +*j) > max) {
                    break;
                }

                sum_of_two_abundants.insert(*i + *j);
            }
        }
    }

    unsigned long collect_difference(void) {
        std::vector<u_int> all;

        for (u_int i = 1; i < max; ++i) {
            all.push_back(i);
        }

        std::vector<u_int> diff;
        std::set_difference(all.begin(), all.end(), sum_of_two_abundants.begin(),
                sum_of_two_abundants.end(), std::back_inserter(diff));

        unsigned long sum = 0;
        for (std::vector<u_int>::const_iterator itr = diff.begin(); itr != diff.end(); ++itr) {
            sum += *itr;
        }

        return sum;
    }

    bool is_abundant(u_int val) {
        return std::find(abundants.begin(), abundants.end(), val) != abundants.end();
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

    for (u_int div = 1; div < dividend; ++div) {
        if ((dividend % div) == 0) {
            sum += div;
        }
    }

    return sum;
}

TEST(Euler023, FindDivisors) {
    ASSERT_EQ(284, sum_divisors(220));
    ASSERT_EQ(220, sum_divisors(284));
}

TEST(Euler023, FindFirstAbundants) {
    Abundants abs(13);
    abs.populate();

    ASSERT_FALSE(abs.is_abundant(1));
    ASSERT_TRUE(abs.is_abundant(12));
}

TEST(Euler023, CheckCollection) {
    Abundants abs(50);
    abs.populate();
    abs.populate_sums();

    ASSERT_EQ(891, abs.collect_difference());
}

TEST(Euler023, FinalAnswer) {
    Abundants abs(MAX_ABUNDANT);
    abs.populate();
    abs.populate_sums();

    unsigned long sum = abs.collect_difference();
    ASSERT_EQ(4179871, sum);

    cout << "The sum was: " << sum << endl;
}

