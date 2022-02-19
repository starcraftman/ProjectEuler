/**
The fraction 49/98 is a curious fraction, as an inexperienced mathematician in attempting to simplify
it may incorrectly believe that 49/98 = 4/8, which is correct, is obtained by cancelling the 9s.

We shall consider fractions like, 30/50 = 3/5, to be trivial examples.

There are exactly four non-trivial examples of this type of fraction,
less than one in value, and containing two digits in the numerator and denominator.

If the product of these four fractions is given in its lowest common terms,
find the value of the denominator.
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
typedef std::uint64_t num_t;
static const num_t STOP_VAL = 100'00; // Can no longer have at least 2 multiples <= 9 digits

class NumGen {
public:
    NumGen(num_t initial = 1) : integer(initial) {};

    std::string current_str() {
        if (cache_str == "") {
            for (auto val : values) {
                cache_str += std::to_string(val);
            }
        }

        return cache_str;
    }
    num_t current_val() {
        if (cache_val == 0) {
            for (char c: current_str()) {
                cache_val *= 10;
                cache_val += int(c) - int('0');
            }
        }

        return cache_val;
    }
    num_t num_digits() {
        return current_str().size();
    }
    bool is_valid() {
        return util::is_pandigital(current_val(), false) && num_digits() == 9;
    }
    // Incrementally add the value to the number and string form.
    void add_multiple() {
        values.push_back((values.size() + 1) * integer);
        std::string temp = std::to_string(values.back());
        cache_str += temp;
        for (char c: temp) {
            cache_val *= 10;
            cache_val += int(c) - int('0');
        }
    }
    // Reset completely the stored multiple, base integer and caches.
    void reset() {
        values.clear();
        ++integer;
        cache_str = "";
        cache_val = 0;
    }
    num_t next_candidate() {
        while (integer < STOP_VAL) {
            if (num_digits() > 9) {
                reset();
            }

            add_multiple();
            if (is_valid()) {
                break;
            }
        };

        return current_val();
    }

    friend std::ostream & operator<<(std::ostream &os, NumGen &gen) {
        std::stringstream ss;
        os << gen.values.size() << " multiples of " << gen.integer
            << " produce " << gen.current_str() << " => (";
        for (num_t val: gen.values) {
            ss << val << ", ";
        }
        std::string fix(ss.str());
        if (fix.size() > 2) {
            fix.pop_back();
            fix.pop_back();
        }
        os << fix << ")";

        return os;
    }

    // Data
    num_t integer = 0; // Numbers are a multiple of this.
    std::vector<num_t> values;
    std::string cache_str = "";
    num_t cache_val = 0;
};

num_t find_largest_pandigital() {
    num_t pandigit = 0;
    NumGen gen(1);
    while (gen.integer < STOP_VAL) {
        num_t next = gen.next_candidate();
        cout << "Found: " << gen << endl;
        if (next > pandigit) {
            pandigit = next;
        }
    }

    return pandigit;
}

TEST(Euler038_NumGen, Constructor) {
    NumGen gen(9);
    ASSERT_EQ(gen.integer, 9);
    ASSERT_EQ(gen.values.size(), 0);
}

TEST(Euler038_NumGen, CurrentStr) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    ASSERT_EQ(gen.current_str(), std::string("918273645"));
}

TEST(Euler038_NumGen, CurrentVal) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    ASSERT_EQ(gen.current_val(), 918273645);
}

TEST(Euler038_NumGen, NumDigits) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    ASSERT_EQ(gen.num_digits(), 9);
}

TEST(Euler038_NumGen, IsValid) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    ASSERT_EQ(gen.is_valid(), true);
}

TEST(Euler038_NumGen, Reset) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    gen.reset();
    ASSERT_TRUE(gen.values.empty());
    ASSERT_EQ(gen.cache_str, "");
    ASSERT_EQ(gen.cache_val, 0);
    ASSERT_EQ(gen.integer, 10);
}

TEST(Euler038_NumGen, AddMultiple) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    gen.current_str(); // Needed to reset string.
    gen.add_multiple();
    ASSERT_EQ(gen.values.back(), 54);
    ASSERT_EQ(gen.current_str(), "91827364554");
    ASSERT_EQ(gen.num_digits(), 11);
}

TEST(Euler038_NumGen, OutputOperator) {
    NumGen gen(9);
    gen.values = {9, 18, 27, 36, 45};
    std::stringstream ss;
    ss << gen;
    std::string expect = "5 multiples of 9 produce 918273645 => (9, 18, 27, 36, 45)";
    ASSERT_EQ(ss.str(), expect);
}

TEST(Euler038_NumGen, Next) {
    NumGen gen(9);
    ASSERT_EQ(gen.next_candidate(), 918273645);
    ASSERT_EQ(gen.next_candidate(), 192384576);
}

TEST(Euler033, GenerateFractions) {
    num_t pandigit = find_largest_pandigital();

    cout << "The largest 9 digit pandigital produced by integer multiplication: " << pandigit << endl;
    ASSERT_EQ(pandigit, 932718654);
}
