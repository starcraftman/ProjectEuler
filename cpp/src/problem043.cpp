/**
The number, 1406357289, is a 0 to 9 pandigital number because it is made up of each of the digits 0 to 9 in some order, but it also has a rather interesting sub-string divisibility property.

Let d1 be the 1st digit, d2 be the 2nd digit, and so on. In this way, we note the following:

    d2d3d4=406 is divisible by 2
    d3d4d5=063 is divisible by 3
    d4d5d6=635 is divisible by 5
    d5d6d7=357 is divisible by 7
    d6d7d8=572 is divisible by 11
    d7d8d9=728 is divisible by 13
    d8d9d10=289 is divisible by 17

Find the sum of all 0 to 9 pandigital numbers with this property.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <climits>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
typedef std::uint64_t num_t;
const static std::initializer_list<num_t> INITIAL_DIVS = {17, 13, 11, 7, 5, 3, 2};
const static std::initializer_list<num_t> INITIAL_PANDIGITAL = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// Check divisability per the problem statement
bool check_sub_parts(num_t value) {
    bool parts_divisable = true;

    // Capture the 3 last digits from the value passed in initially.
    num_t last = value % 10;
    value /= 10;
    num_t second_last = value % 10;
    value /= 10;
    num_t third_last = value % 10;
    value /= 10;
    std::deque<num_t> divs = INITIAL_DIVS;  // The divisors to apply in order
    while (value && divs.size() != 0) {
        num_t divisor = divs.front();
        divs.pop_front();
        num_t number = 100 * third_last + 10 * second_last + last;
        if (number % divisor != 0)  {
            parts_divisable = false;
            break;
        }

        // Keep shifting down the values.
        last = second_last;
        second_last = third_last;
        third_last = value % 10;
        value /= 10;
    }

    return parts_divisable;
}

class PandigitalGenerator {
public:
    explicit PandigitalGenerator(const std::vector<num_t> &to_append) {
        digits.insert(digits.end(), to_append.cbegin(), to_append.cend());
        std::sort(digits.begin(), digits.end());
    }

    inline bool has_more_perms() const {
        return this->has_next;
    };
    num_t num_digits() const {
        return digits.size();
    }
    num_t next() {
        this->has_next = std::next_permutation(this->digits.begin(), this->digits.end());
        return this->current();
    }
    // Provide the current pandigital as an integer.
    // Leading zeroes are naturally dropped as not significant.
    num_t current() const {
        auto iter = digits.cbegin();
        num_t num = *iter;
        while (++iter != digits.cend()) {
            num = num * 10 + *iter;
        }

        return num;
    }

    friend std::ostream& operator<<(std::ostream &os, const PandigitalGenerator &gen);
private:
    std::vector<num_t> digits;
    bool has_next = true;
};

std::ostream& operator<<(std::ostream &os, const PandigitalGenerator &gen) {
    os << "PandigitalGenerator for digits: " << gen.num_digits() << endl
        << "Current Permutation: ";

    for (auto ele : gen.digits) {
        os << ele << " ";
    }
    os << "\b";

    return os;
}

// Search for all pandigitals that satisfy the divisibilty requirements
num_t search_pandigital_space(const std::initializer_list<num_t> &digits) {
    // Search current space
    PandigitalGenerator gen(digits);
    num_t sum = 0;
    while (gen.has_more_perms()) {
        if (check_sub_parts(gen.current())) {
            cout << "Divisable: " << gen.current() << endl;
            sum += gen.current();
        }

        gen.next();
    }

    return sum;
}

TEST(Euler043, CheckSubParts) {
    ASSERT_EQ(check_sub_parts(1406357289), true);
}

TEST(Euler043, PandigitalGeneratorConstructor) {
    std::initializer_list<num_t> digits = {1, 2, 3};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 123);
}

TEST(Euler043, PandigitalGeneratorCurrent) {
    std::initializer_list<num_t> digits = {1, 2, 3, 4};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 1234);
}

TEST(Euler043, PandigitalGeneratorNext) {
    std::initializer_list<num_t> digits = {1, 2, 3};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 123);
    gen.next();
    ASSERT_EQ(gen.current(), 132);
}

TEST(Euler043, PandigitalGeneratorHasMorePerms) {
    std::initializer_list<num_t> digits = {1, 2};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 12);
    ASSERT_TRUE(gen.has_more_perms());
    gen.next();
    ASSERT_EQ(gen.current(), 21);
    gen.next();
    ASSERT_FALSE(gen.has_more_perms());
}

TEST(Euler043, PandigitalGeneratorNumDigits) {
    std::initializer_list<num_t> digits = {1, 2};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.num_digits(), 2);
}

TEST(Euler043, PandigitalGeneratorOutputOperator) {
    std::initializer_list<num_t> digits = {1, 2, 3};
    PandigitalGenerator gen(digits);
    gen.next();
    std::stringstream ss;
    ss << gen;
    std::string expect = "PandigitalGenerator for digits: 3\nCurrent Permutation: 1 3 2 \b";
    ASSERT_EQ(ss.str(), expect);
}

TEST(Euler043, SearchPandigitalSpace) {
    num_t sum = search_pandigital_space(INITIAL_PANDIGITAL);
    ASSERT_EQ(sum, 16695334890);

    cout << "Sum of pandigitals that divide is: " << sum << endl;
}
