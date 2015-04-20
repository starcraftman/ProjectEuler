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
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>
#include <algorithm>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;

/************** Global Vars & Functions *******************/
/* Struct like class, quite simple. */
class Triplet {
public:
    Triplet(int a, int b, int c) : a(a), b(b), c(c), sum(a + b + c) {};

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

    const int a;
    const int b;
    const int c;
    const int sum;
private:
    friend std::ostream & operator<<(std::ostream &out, const Triplet &trip);
};

std::ostream & operator<<(std::ostream &out, const Triplet &trip) {
    return out << "Triplet: " << trip.a << " " << trip.b << " "
        << trip.c << endl;
}

class TripletGen {
public:
    TripletGen(int max) : max(max) {};

    /* Shouldn't be used. */
    bool static is_triplet(int a, int b, int c) {
        return (a * a + b * b) == c * c;
    }

    void find_triplets() {
        /* TODO: Use euclids algorthim for generating triplets.
         * TODO: Requires separate generator for coprime pairs,
         * probably in util::.*/
    }

    /* total = a + b + c */
    std::vector<Triplet> candidates(int total) {
        std::vector<Triplet> res;

        for (std::vector<Triplet>::const_iterator i = trips.begin();
                i != trips.end(); ++i) {
            if ((total / i->sum) == 0) {
                int factor = total / i->sum;
                res.push_back(*i * factor);
            }
        }

        return res;
    }
private:
    const int max;
    /* List of UNIQUE primitive triplets,
     * includes {3,4,5} not {6,8,10} */
    std::vector<Triplet> trips;
};

/**************** Global Functions & Data ******************/
TEST(Euler039, Triplet) {
    ASSERT_TRUE(TripletGen::is_triplet(20, 48, 52));
    ASSERT_TRUE(TripletGen::is_triplet(30, 40, 50));
}

TEST(Euler039, DISABLED_FindTriplets) {
    std::vector<Triplet> expected, res;
    expected.push_back(Triplet(20, 48, 52));
    expected.push_back(Triplet(24, 51, 51));
    expected.push_back(Triplet(30, 40, 50));

    TripletGen tg(200);
    res = tg.candidates(120);

    for (std::vector<Triplet>::const_iterator i = res.begin();
            i != res.end(); ++i) {
        cout << i->a << " " << i->b << " " << i->c << endl;
    }

    ASSERT_EQ(3, res.size());
    for (std::vector<Triplet>::size_type i = 0; i < expected.size(); ++i) {
        ASSERT_TRUE(res[i] == expected[i]);
    }
}
