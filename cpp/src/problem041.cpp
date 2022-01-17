/**
 *
 * We shall say that an n-digit number is pandigital if it makes use of all the digits 1 to n exactly once.
 * For example, 2143 is a 4-digit pandigital and is also prime.
 *
 * What is the largest n-digit pandigital prime that exists?
 *
 * Solution Approach:
 * - Generate all pandigitals, starting with 10 digits and working down while keeping largest possible set.
 *   i.e. start with 0123456789. then search every space where 1 removed, then recurse.
 * - Test numbers as we go from prime, if they are, largest one found should be max.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream>
#include <set>
#include <algorithm>
#include <initializer_list>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
typedef std::uint32_t val_t;

class PandigitalGenerator {
public:
    PandigitalGenerator(const std::vector<val_t> &to_append) {
        digits.insert(digits.end(), to_append.cbegin(), to_append.cend());
        std::sort(digits.begin(), digits.end());
    }

    inline bool has_more_perms() const {
        return this->has_next;
    };
    bool is_current_prime() const {
        return util::is_prime(current());
    }
    val_t num_digits() const {
        return digits.size();
    }
    val_t current() const;
    val_t next();

    friend std::ostream& operator<<(std::ostream &os, const PandigitalGenerator &gen);
private:
    std::vector<val_t> digits;
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

val_t PandigitalGenerator::next() {
    this->has_next = std::next_permutation(this->digits.begin(), this->digits.end());
    return this->current();
}

// Provide the current pandigital as an integer.
// Leading zeroes are naturally dropped as not significant.
val_t PandigitalGenerator::current() const {
    auto iter = digits.cbegin();
    val_t num = *iter;
    while (++iter != digits.cend()) {
        num = num * 10 + *iter;
    }

    return num;
}

// Search a single pandigital space by permuting.
val_t search_pandigital_space(std::vector<val_t> &digits) {
    // Search current space
    PandigitalGenerator gen(digits);
    val_t largest_seen = 0;
    while (gen.has_more_perms()) {
        if (gen.is_current_prime() && gen.current() > largest_seen) {
            largest_seen = gen.current();
        }

        gen.next();
    }

    return largest_seen;
}

// Search the pandigital space.
// - Start with 9 digit pandigital numbers.
// - Check every permutation for primality.
// - If none found, remove last digit and search new space.
// - First prime encountered in this way must be the largest.
val_t largest_pandigital() {
    std::vector<val_t> digits = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    val_t largest_seen = 0;
    while (digits.size() != 0 && largest_seen == 0) {
        largest_seen = search_pandigital_space(digits);
        digits.pop_back();
    }

    return largest_seen;
}

TEST(Euler041, PandigitalGeneratorNext) {
    std::vector<val_t> digits = {1, 2, 3};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 123);
    gen.next();
    ASSERT_EQ(gen.current(), 132);
}

TEST(Euler041, PandigitalGeneratorHasMorePerms) {
    std::vector<val_t> digits = {1, 2};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 12);
    ASSERT_TRUE(gen.has_more_perms());
    gen.next();
    ASSERT_EQ(gen.current(), 21);
    gen.next();
    ASSERT_FALSE(gen.has_more_perms());
}

TEST(Euler041, PandigitalGeneratorCurrent) {
    std::vector<val_t> digits = {1, 2, 3, 4};
    PandigitalGenerator gen(digits);
    ASSERT_EQ(gen.current(), 1234);
}

TEST(Euler041, GeneratePrimes) {
    val_t result = largest_pandigital();
    cout << "The largest pandigital prime (base 10) is: " << result << endl;
    ASSERT_EQ(result, 7652413);
}
