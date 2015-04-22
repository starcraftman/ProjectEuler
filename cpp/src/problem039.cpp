/**
If p is the perimeter of a right angle triangle with integral length sides,
{a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

Note: This problem is just an alias to which number under 1000 generates
most pythagorean triplets.

To solve, I will:
1) Generate all unique triplets efficiently.
2) Make function that identifies multiple of triplets, such a + b + c = p.
3) Iterate 1 - 1000 to get triplets.
NB: Optimization noticed after, only need to search even p since sums
of triplets always even.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>
#include <algorithm>

#include "gtest/gtest.h"
#include "gens.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::gens::Coprimes;

/************** Global Vars & Functions *******************/
/* Struct like class, quite simple. */
class Triplet {
public:
    Triplet() : a(0), b(0), c(0), sum(0) {};
    Triplet(int _a, int _b, int _c) : sum(_a + _b + _c) {
        // a < b < c
        a = std::min(std::min(_a, _b), _c);
        c = std::max(std::max(_a, _b), _c);
        if (_a > a && _a < c) {
            b = _a;
        } else if (_b > a && _b < c) {
            b = _b;
        } else {
            b = _c;
        }
    };

    /* Compare */
    inline bool operator==(const Triplet &other) const {
        return a == other.a && b == other.b && c == other.c;
    }
    inline bool operator!=(const Triplet &other) const { return !operator==(other); }
    inline bool operator<(const Triplet &other)  const { return sum < other.sum; }
    inline bool operator>(const Triplet &other)  const { return other.operator<(*this); }
    inline bool operator<=(const Triplet &other) const { return !operator>(other); }
    inline bool operator>=(const Triplet &other) const { return !operator<(other); }
    Triplet operator*(int k) const {
        return Triplet(a * k, b * k, c * k);
    }

    int a;
    int b;
    int c;
    int sum;
private:
    friend std::ostream & operator<<(std::ostream &out, const Triplet &trip);
};

std::ostream & operator<<(std::ostream &out, const Triplet &trip) {
    return out << "Triplet: " << trip.a << " " << trip.b << " "
        << trip.c;
}

std::vector<Triplet> find_triplets(int limit) {
    Coprimes cGen(limit);
    Coprimes::CoPairs pairs = cGen.collect();
    std::vector<Triplet> trips;

    for (Coprimes::CoPairs::const_iterator i = pairs.begin(); i != pairs.end(); ++i) {
        if (((i->first - i->second) % 2) == 0) {
            continue;
        }

        Triplet trip(std::abs(i->first * i->first - i->second * i->second),
                    2 * i->first * i->second,
                    i->first * i->first + i->second * i->second);
        trips.push_back(trip);
    }
    std::sort(trips.begin(), trips.end());

    return trips;
}

/* total = a + b + c */
template <class Iter>
std::vector<Triplet> candidates_for_total(Iter itr, Iter end, int total) {
    std::vector<Triplet> res;

    for ( ; itr != end; ++itr) {
        if (itr->sum > total) {
            break;
        }

        if ((total % itr->sum) == 0) {
            int factor = total / itr->sum;
            res.push_back(*itr * factor);
        }
    }

    return res;
}

/**************** Global Functions & Data ******************/
TEST(Euler039, FirstTriplet) {
    Triplet expect(3, 4, 5);
    std::vector<Triplet> trips = find_triplets(20);
    ASSERT_EQ(expect, *trips.begin());
}

bool comp(const Triplet &l, const Triplet &r) {
    return l.a < r.a;
}

TEST(Euler039, TripletsForP) {
    std::vector<Triplet> expected;
    expected.push_back(Triplet(20, 48, 52));
    expected.push_back(Triplet(24, 45, 51));
    expected.push_back(Triplet(30, 40, 50));

    std::vector<Triplet> trips = find_triplets(20);
    std::vector<Triplet> cands = candidates_for_total(trips.begin(),
            trips.end(), 120);
    std::sort(cands.begin(), cands.end(), comp);

    ASSERT_EQ(3, cands.size());
    for (std::vector<Triplet>::size_type i = 0; i < expected.size(); ++i) {
        ASSERT_TRUE(cands[i] == expected[i]);
    }
}

TEST(Euler039, FinalAnswer) {
    std::vector<Triplet> trips = find_triplets(20);
    int big_p = 0;
    unsigned int big_num_trips = 0;

    for (int p = 12; p <= 1000; p += 2) {
        std::vector<Triplet> cands = candidates_for_total(trips.begin(),
                trips.end(), p);
        if (cands.size() > big_num_trips) {
            big_num_trips = cands.size();
            big_p = p;
        }
    }

    cout << "The p value " <<  big_p << " generates " << big_num_trips
        << " triplets." << endl;
    std::vector<Triplet> cands = candidates_for_total(trips.begin(),
            trips.end(), big_p);
    std::sort(cands.begin(), cands.end(), comp);
    for (std::vector<Triplet>::const_iterator i = cands.begin();
            i != cands.end(); ++i) {
        cout << *i << endl;
    }

    ASSERT_EQ(840, big_p);
    ASSERT_EQ(8, big_num_trips);
}

