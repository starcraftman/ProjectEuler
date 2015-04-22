/**
We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once;
for example, the 5-digit number, 15234, is 1 through 5 pandigital.
The product 7254 is unusual, as the identity, 39 × 186 = 7254,
containing multiplicand, multiplier, and product is 1 through 9 pandigital.

Find the sum of all products whose multiplicand/multiplier/product
identity can be written as a 1 through 9 pandigital.
HINT: Some products can be obtained in more than one way so be sure
to only include it once in your sum.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream>
#include <set>
#include <algorithm>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
int places(u_long num) {
    u_long places = 0;
    while (num != 0) {
        ++places;
        num /= 10;
    }

    return places;
}

bool is_pandigital(std::string num) {
    std::sort(num.begin(), num.end());

    std::stringstream ss2;
    for (std::string::size_type i = 1; i <= num.size(); ++i) {
        ss2 << i;
    }

    return num == ss2.str();
}

bool is_pandigital(u_long num) {
    std::stringstream ss;
    ss << num;
    std::string temp = ss.str();

    return is_pandigital(temp);
}

TEST(Euler032, IsPanDigital) {
    std::string num("15324");
    u_long num2 = 15324;
    ASSERT_TRUE(is_pandigital(num));
    ASSERT_TRUE(is_pandigital(num2));
}

// Notes on limits:
// * Pandigital number must have 1-9 & be exactly 9 digits.
//
// * Since i | j | i * j, where | is concat must be 9, it is reasonable
// to say that i & j can't be more than 4 digits either, since 1 * 4 = 4 and
// 1 + 4 + 4 = 9 digits.
//
// * So possible combinations are 1 4 4, 2 3 4 (digits).
//
// Starting Val i:
// * 1 not considered, since 1 * n = n, missing digits.
// * 2 not considered, since 2 * n = 2n. Impossible to have the 5 odds.
//
// * Highest val then, is 9999/3 = 3333. Must be less 3000 since can't have 2 3s.
TEST(Euler032, FinalAnswer) {
    u_long sum = 0;
    std::set<u_long> products;
    for (int i = 3; i < 2987; ++i) {
        for (int j = 3; j < 2997; ++j) {
            u_long prod = i * j;
            if (prod > 9999) {
                break;
            }

            std::stringstream ss;
            ss << i << j << i * j;
            if (ss.str().size() == 9 && is_pandigital(ss.str())) {
                products.insert(prod);
            }
        }
    }

    for (std::set<u_long>::const_iterator i = products.begin();
            i != products.end(); ++i) {
        sum += *i;
    }

    cout << "The sum of the pandigital products: " << sum << endl;
    ASSERT_EQ(45228, sum);
}

