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
template<class T>
inline int max(std::initializer_list<T> list) {
    if (list.size() < 1) {
        return 0;
    }

    auto iter = list.begin();
    T current = *iter;
    iter++;
    while (iter != list.end()) {
        if (*iter > current) {
            current = *iter;
        }
        iter++;
    }

    return current;
}

template<class T>
inline int min(std::initializer_list<T> list) {
    if (list.size() < 1) {
        return 0;
    }

    auto iter = list.begin();
    T current = *iter;
    iter++;
    while (iter != list.end()) {
        if (*iter < current) {
            current = *iter;
        }
        iter++;
    }

    return current;
}

template<class T>
void gcd(T first, T second, std::vector<T> &factors) {
    T low = min({first, second});
    T high = max({first, second});

    T num = 1;
    while (num != low + 1) {
        if ((low % num) == 0 && (high % num) == 0) {
            factors.push_back(num);
        }
        num += 1;
    }
}

// Class represents a simple idea of a fraction.
class Fraction
{
public:
    Fraction(int numerator = 1, int denomenator = 1) :
        initial_numerator(numerator),
        initial_denomenator(denomenator),
        numerator(numerator),
        denomenator(denomenator) {}
    Fraction(const Fraction &other) :
        initial_numerator(other.numerator),
        initial_denomenator(other.denomenator),
        numerator(other.numerator),
        denomenator(other.denomenator),
        factors(other.factors){
        }
    bool cancel_digits();
    bool is_reduced();
    void reduce();

    // Operators
    Fraction& operator*=(const Fraction &rhs) {
        this->numerator *= rhs.numerator;
        this->denomenator *= rhs.denomenator;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream &os, const Fraction &fraction);
    friend Fraction operator*(Fraction &lhs, const Fraction &rhs) {
        Fraction n_lhs(lhs.numerator, lhs.denomenator);
        n_lhs.numerator *= rhs.numerator;
        n_lhs.denomenator *= rhs.denomenator;
        return n_lhs;
    }

    // Data
    const int initial_numerator;
    const int initial_denomenator;
    int numerator;
    int denomenator;
    std::vector<int> factors;
};

bool Fraction::cancel_digits() {
    int num_tens = this->initial_numerator / 10;
    int num_ones = this-> initial_numerator % 10;
    int dnum_tens = this->initial_denomenator / 10;
    int dnum_ones = this->initial_denomenator % 10;

    bool cancelled = false;
    // Check if "naieve" cancellation is possible
    if (num_ones == dnum_tens &&
        dnum_ones != 0 && dnum_ones != 1) {

        // check if new fraction equivalent
        float old_val = float(this->initial_numerator) / float(this->initial_denomenator);
        float new_val = float(num_tens) / float(dnum_ones);
        if (old_val == new_val) {
            this->numerator = num_tens;
            this->denomenator = dnum_ones;
            cancelled = true;
        }
    }

    return cancelled;
}

bool Fraction::is_reduced() {
    return this->numerator != this->initial_numerator &&
        this->denomenator != this->initial_denomenator;
}

// Reduce fraction based on initial values.
void Fraction::reduce() {
    factors.clear();
    gcd(this->numerator, this->denomenator, this->factors);

    int greatest = *factors.rbegin();
    this->numerator = this->numerator / greatest;
    this->denomenator = this->denomenator / greatest;
}

std::ostream& operator<<(std::ostream &os, const Fraction &fraction) {
    os << "Fraction was: " << fraction.initial_numerator << "/" << fraction.initial_denomenator << " reduced to: "
        << fraction.numerator << "/" << fraction.denomenator << endl
        << "Common factors: (";

    for (auto ele : fraction.factors) {
        os << ele << ", ";
    }
    os << "\b\b)";

    return os;
}

// Use this to generate all fractions to search
Fraction generate_fractions() {
    std::vector<Fraction> found;
    bool dnum_ten = false;

    /* Improvement:
     * It is possible to be more efficient and  generate ONLY the numbers that can cancel,
     * but that isn't needed in this small of a problem.
     */
    for (int dnum = 11; dnum < 100; ++dnum) {
        // cout << "Generating fractions for denomenator: " << dnum << endl;
        if ( (dnum % 10) == 0 ) {
            dnum_ten = true;
        }

        for (int num = 10; num < dnum; ++num) {
            // Skip all trivial reductions (i.e. cancel 0s).
            if (dnum_ten && (num % 10) != 0) {
                continue;
            } else {
                // Verify non-trivial cancel is equivalent fraction & reduces initial fraction
                Fraction fraction(num, dnum);
                if (fraction.cancel_digits()) {
                    fraction.reduce();
                    cout << "Cancelled & reducable fraction: " << fraction << endl;
                    found.push_back(fraction);
                }
            }
        }

        dnum_ten = false;
    }

    Fraction final_result(1, 1);
    for (auto ele : found) {
        final_result *= ele;
    }

    final_result.reduce();
    return final_result;
}


// Tests follow.
TEST(Euler033, MaxFunc) {
    ASSERT_EQ(max({44, 75, 2, 44, 99, 5, 33}), 99);
}

TEST(Euler033, MinFunc) {
    ASSERT_EQ(min({44, 75, 2, 44, 99, 5, 33}), 2);
}

TEST(Euler033, GCD) {
    std::vector<int> results;
    int first = 49;
    int second = 98;
    gcd(first, second, results);

    // ASSERT_EQ(results.size(), 4);
    ASSERT_EQ(*results.rbegin(), 49);
}

TEST(Euler033, FractionCreate) {
    Fraction value(30,  50);

    ASSERT_EQ(value.numerator, 30);
    ASSERT_EQ(value.denomenator, 50);
}

TEST(Euler033, FractionReduce) {
    Fraction value(30,  50);
    value.reduce();

    ASSERT_EQ(value.numerator, 3);
    ASSERT_EQ(value.denomenator, 5);
}

TEST(Euler033, FractionCancelDigits) {
    Fraction value(49, 98);

    ASSERT_TRUE(value.cancel_digits());
    ASSERT_EQ(value.numerator, 4);
    ASSERT_EQ(value.denomenator, 8);
}

TEST(Euler033, FractionIsReduced) {
    Fraction value(30,  50);
    value.reduce();

    ASSERT_TRUE(value.is_reduced());
}

TEST(Euler033, FractionOperatorMulti) {
    Fraction value(5, 10);
    Fraction value2(2, 6);

    value *= value2;
    ASSERT_EQ(value.numerator, 10);
    ASSERT_EQ(value.denomenator, 60);
}


TEST(Euler033, GenerateFractions) {
    Fraction result = generate_fractions();

    ASSERT_EQ(result.numerator, 1);
    ASSERT_EQ(result.denomenator, 100);
    cout << "Value of the denomenator of the fractions multiplied and then reduced is: " << result.denomenator << endl;
}
