/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <string> /* C++ String class. */
#include <vector>

/* Local */
#include "gtest/gtest.h"
#include "boost/assign/list_of.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/************** Class *******************/
class AmicableNumbers {
public:
    AmicableNumbers(int max) : max_number(max) {};

    void search(void) {
        int sum = 0, pair_i = 0, pair_sum = 0;

        for (int i = 1; i < max_number; ++i) {
            sum = sum_divisors(i);
            pair_i = sum;

            /* Only look at first occurences. */
            if (i > sum)
                continue;

            pair_sum = sum_divisors(pair_i);
            if (is_amicable(i, sum, pair_sum)) {
                amicables.push_back(i);
                amicables.push_back(sum);
            }
        }
    }

    bool is_amicable(int first, int first_sum, int second_sum) {
        return first != first_sum && first == second_sum;
    }

    int sum_divisors(int dividend) {
        int sum = 0;

        for (int div = 1; div < dividend; ++div) {
            if ((dividend % div) == 0) {
                sum += div;
            }
        }

        return sum;
    }

    std::vector<int> results(void) {
        return amicables;
    }

private:
    int max_number;
    std::vector<int> amicables;
};


/************** Global Vars & Functions *******************/
TEST(Euler021, FindFirstAmicables) {
    std::vector<int> actual, expect = boost::assign::list_of (220) (284);
    AmicableNumbers amic(300);
    amic.search();
    actual = amic.results();

    ASSERT_EQ(expect.size(), actual.size());
    for (std::vector<int>::size_type i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(expect[i], actual[i]);
    }
}

TEST(Euler021, FindDivisors) {
    AmicableNumbers amic(1);
    ASSERT_EQ(284, amic.sum_divisors(220));
    ASSERT_EQ(220, amic.sum_divisors(284));
}

TEST(Euler021, FindAnswer) {
    std::vector<int> actual, expect = boost::assign::list_of (220) (284) (1184) (1210)
        (2620) (2924) (5020) (5564) (6232) (6368);
    AmicableNumbers amic(10000);
    amic.search();
    actual = amic.results();

    ASSERT_EQ(expect.size(), actual.size());
    for (std::vector<int>::size_type i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(expect[i], actual[i]);
    }

    int sum_amicables = 0;
    for (std::vector<int>::const_iterator itr = actual.begin(); itr != actual.end(); ++itr) {
        sum_amicables += *itr;
    }

    ASSERT_EQ(31626, sum_amicables);
    cout << "Sum of amicables under 10000 is: " << sum_amicables << endl;
}

